#version 430

uniform sampler3D colorlod4;
uniform sampler3D colorlod16;
uniform sampler3D colorlod64;
uniform sampler2D materials;

uniform sampler3D chunks[];

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 inv_projection_matrix;
uniform mat4 inv_view_matrix;
uniform mat4 inv_model_matrix;

uniform vec2 screen_ratios;
uniform vec2 screen_size;

uniform int lod;

out vec4 fragColor;

//VALUES
const float EPSILON = 0.0001;
const float INFINITY = 67108864; //2^26
const int NULL = 0;
const float FARPLANE = 1000;

const int MAX_MATERIAL_ID = 3;
const int AIR_ID = 0;
const int STONE_ID = 1;
const int DIRT_ID = 2;
const int GRASS_ID = 3;

//VOXELS
const float VOXEL4 = 16;
const int VOXEL4X = 4;
const int VOXEL4Y = 16;
const int VOXEL4Z = 4;

const float VOXEL16 = 4;
const int VOXEL16X = 4;
const int VOXEL16Y = 4;
const int VOXEL16Z = 4;

const float VOXEL64 = 1;
const int VOXEL64X = 4;
const int VOXEL64Y = 4;
const int VOXEL64Z = 4;

//LIGHTING
const vec3 SUNDIR = vec3(-0.234, -0.801, -0.567);
const float FOGINTENSITY = 0.005;
const vec3 AMBIENTCOLOR = vec3(.5, .8, .9);
const float AMBIENTFORCE = 0.3;
const vec3 SUNCOLOR = vec3(1, 0.97, 0.75);
const float SUNFORCE = 3;

struct CollisionData {
    float distance;
    vec3 normal;
    vec4 color;
    bool valid;
};

struct TMinMax {
    float tmin;
    float tmax;
};

TMinMax boxFarDistance (vec3 O, vec3 D, vec3 min, vec3 max) {
    TMinMax tminmax;
    tminmax.tmin = INFINITY;
    tminmax.tmax = INFINITY;

    float tmin = (min.x - O.x) / D.x;
    float tmax = (max.x - O.x) / D.x;

    if (tmin > tmax) {
        float tmp = tmin;
        tmin = tmax;
        tmax = tmp;
    }

    float tymin = (min.y - O.y) / D.y;
    float tymax = (max.y - O.y) / D.y;

    if (tymin > tymax) {
        float tmp = tymin;
        tymin = tymax;
        tymax = tmp;
    }

    if ((tmin > tymax) || (tymin > tmax)) {
        return tminmax;
    }

    if (tymin > tmin) {
        tmin = tymin;
    }

    if (tymax < tmax) {
        tmax = tymax;
    }

    float tzmin = (min.z - O.z) / D.z;
    float tzmax = (max.z - O.z) / D.z;

    if (tzmin > tzmax) {
        float tmp = tzmin;
        tzmin = tzmax;
        tzmax = tmp;
    }

    if ((tmin > tzmax) || (tzmin > tmax)) {
        return tminmax;
    }
    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    tminmax.tmin = tmin;
    tminmax.tmax = tmax;
    return tminmax;
}

vec4 getTexture3D(sampler3D sampler, vec3 coords) {
    return texture(sampler, clamp(vec3(1 - EPSILON) - coords, vec3(0), vec3(1)));
}

vec3 getColor(int material, vec3 coord, vec3 n) {
    material--; // [1, N] to [0, N-1]
    float shading;
    vec2 uv;
    if (n.x != 0) {
        uv =  mod((coord.yz * 0.015625 * vec2(0.25, 1)), vec2(.25, 1)) + vec2(0.25 * material, 0);
        shading = max(AMBIENTFORCE, n.x * 0.5);
    } else if (n.y != 0) {
        uv =  mod((coord.xz * 0.015625 * vec2(0.25, 1)), vec2(1, 1)) + vec2(0.25 * material, 0);
        shading = max(AMBIENTFORCE, n.y);
    } else {
        uv =  mod((coord.yx * 0.015625 * vec2(0.25, 1)), vec2(.25, 1)) + vec2(0.25 * material, 0);
        shading = max(AMBIENTFORCE, n.z * 0.7);
    }
    return texture(materials,uv).rgb * shading;
}

CollisionData gridDF64 (vec3 O, vec3 D, vec3 gridPos, vec3 offset) {
    CollisionData cdata;
    cdata.distance = INFINITY;

    //TMinMax tminmax = boxFarDistance(O, D, gridPos, gridPos + (vec3(1) * gridsize * voxelsize));
    //if (tminmax.tmax == INFINITY) return cdata;

    vec3 hitPos = O;
    //if (tminmax.tmin > 0) {
    //    hitPos += D * (tminmax.tmin);
    //    hitPos -= D * EPSILON;
    //}


    float X, Y, Z;

    X = floor((hitPos - gridPos) / VOXEL64).x;
    Y = floor((hitPos - gridPos) / VOXEL64).y;
    Z = floor((hitPos - gridPos) / VOXEL64).z;

    float tx, ty, tz;

    tx = length(D.xyz / D.x);
    ty = length(D.xyz / D.y);
    tz = length(D.xyz / D.z);

    float tmaxx, tmaxy, tmaxz;

    tmaxx = ((hitPos - gridPos) / VOXEL64).x - X;
    if (D.x > 0) tmaxx = 1 - tmaxx;
    tmaxy = ((hitPos - gridPos) / VOXEL64).y - Y;
    if (D.y > 0) tmaxy = 1 - tmaxy;
    tmaxz = ((hitPos - gridPos) / VOXEL64).z - Z;
    if (D.z > 0) tmaxz = 1 - tmaxz;
    tmaxx = tmaxx * tx;
    tmaxy = tmaxy * ty;
    tmaxz = tmaxz * tz;

    float Xstep, Ystep, Zstep;

    if (D.x > 0) Xstep = 1;
    else Xstep = -1;
    if (D.y > 0) Ystep = 1;
    else Ystep = -1;
    if (D.z > 0) Zstep = 1;
    else Zstep = -1;

    while (true) {
        if (tmaxx < tmaxy && tmaxx < tmaxz) {
            X += Xstep;
            if (X >= 0 && X < VOXEL64X) {
                cdata.color = getTexture3D(colorlod64, (vec3(X, Y, Z) + offset) / vec3(64, 256, 64));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O.xyz - hitPos.xyz) + (tmaxx * VOXEL64);
                    cdata.normal = vec3(normalize(D.x), 0, 0);
                    cdata.color.rgb = getColor(int(cdata.color.r * 255), vec3(X, Y, Z) + offset, cdata.normal);
                    return cdata;
                }
            } else {
                break;
            }
            tmaxx += tx;
        } else if (tmaxy < tmaxz) {
            Y += Ystep;
            if (Y >= 0 && Y < VOXEL64Y) {
                cdata.color = getTexture3D(colorlod64, (vec3(X, Y, Z) + offset) / vec3(64, 256, 64));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxy * VOXEL64);
                    cdata.normal = vec3(0, normalize(D.y), 0);
                    cdata.color.rgb = getColor(int(cdata.color.r * 255), vec3(X, Y, Z) + offset, cdata.normal);
                    return cdata;
                }
            } else {
                break;
            }
            tmaxy += ty;
        } else {
            Z += Zstep;
            if (Z >= 0 && Z < VOXEL64Z) {
                cdata.color = getTexture3D(colorlod64, (vec3(X, Y, Z) + offset) / vec3(64, 256, 64));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxz * VOXEL64);
                    cdata.normal = vec3(0, 0, normalize(D.z));
                    cdata.color.rgb = getColor(int(cdata.color.r * 255), vec3(X, Y, Z) + offset, cdata.normal);
                    return cdata;
                }
            } else {
                break;
            }
            tmaxz += tz;
        }
    }

    return cdata;
}


CollisionData gridDF16 (vec3 O, vec3 D, vec3 gridPos, vec3 offset) {
    CollisionData cdata;
    cdata.distance = INFINITY;
    vec3 hitPos = O;

    float X, Y, Z;

    X = floor((hitPos - gridPos) / VOXEL16).x;
    Y = floor((hitPos - gridPos) / VOXEL16).y;
    Z = floor((hitPos - gridPos) / VOXEL16).z;

    cdata.color = getTexture3D(colorlod16, (vec3(X, Y, Z) + offset) / vec3(16, 64, 16)); //NOTE: Dont forget to set 64 as vec3(16, 128, 16)
    if (cdata.color.r > 0 && lod < 1) {
        cdata.distance = 0;
        cdata.normal = vec3(normalize(D.x), 0, 0);

        CollisionData rcdata = gridDF64(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X * VOXEL16, Y * VOXEL16, Z * VOXEL16), offset * VOXEL64X + vec3(X, Y, Z) * VOXEL64X);
        if (rcdata.distance < INFINITY) return rcdata;
    }

    float tx, ty, tz;

    tx = length(D.xyz / D.x);
    ty = length(D.xyz / D.y);
    tz = length(D.xyz / D.z);

    float tmaxx, tmaxy, tmaxz;

    tmaxx = ((hitPos - gridPos) / VOXEL16).x - X;
    if (D.x > 0) tmaxx = 1 - tmaxx;
    tmaxy = ((hitPos - gridPos) / VOXEL16).y - Y;
    if (D.y > 0) tmaxy = 1 - tmaxy;
    tmaxz = ((hitPos - gridPos) / VOXEL16).z - Z;
    if (D.z > 0) tmaxz = 1 - tmaxz;
    tmaxx = tmaxx * tx;
    tmaxy = tmaxy * ty;
    tmaxz = tmaxz * tz;

    float Xstep, Ystep, Zstep;

    if (D.x > 0) Xstep = 1;
    else Xstep = -1;
    if (D.y > 0) Ystep = 1;
    else Ystep = -1;
    if (D.z > 0) Zstep = 1;
    else Zstep = -1;

    while (true) {
        if (tmaxx < tmaxy && tmaxx < tmaxz) {
            X += Xstep;
            if (X >= 0 && X < VOXEL16X) {
                cdata.color = getTexture3D(colorlod16, (vec3(X, Y, Z) + offset) / vec3(16, 64, 16));
                if (cdata.color.r > 0) {
                    cdata.distance = tmaxx * VOXEL16;
                    cdata.normal = vec3(normalize(D.x), 0, 0);

                    if (lod < 1) {
                        CollisionData rcdata = gridDF64(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X, Y, Z) * VOXEL16, offset * VOXEL64X + vec3(X, Y, Z) * VOXEL64X);
                        if (rcdata.distance < INFINITY) {
                            rcdata.distance += cdata.distance;
                            return rcdata;
                        }
                    } else {
                        cdata.color.rgb = getColor(int(cdata.color.r * 255), vec3(X, Y, Z), cdata.normal);
                        return cdata;
                    }
                }
            } else {
                break;
            }
            tmaxx += tx;
        } else if (tmaxy < tmaxz) {
            Y += Ystep;
            if (Y >= 0 && Y < VOXEL16Y) {
                cdata.color = getTexture3D(colorlod16, (vec3(X, Y, Z) + offset) / vec3(16, 64, 16));
                if (cdata.color.r > 0) {
                    cdata.distance = tmaxy * VOXEL16;
                    cdata.normal = vec3(0, normalize(D.y), 0);

                    if (lod < 1) {
                        CollisionData rcdata = gridDF64(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X, Y, Z) * VOXEL16, offset * VOXEL64X + vec3(X, Y, Z) * VOXEL64X);
                        if (rcdata.distance < INFINITY) {
                            rcdata.distance += cdata.distance;
                            return rcdata;
                        }
                    } else {
                        cdata.color.rgb = getColor(int(cdata.color.r * 255), vec3(X, Y, Z), cdata.normal);
                        return cdata;
                    }
                }
            } else {
                break;
            }
            tmaxy += ty;
        } else {
            Z += Zstep;
            if (Z >= 0 && Z < VOXEL16Z) {
                cdata.color = getTexture3D(colorlod16, (vec3(X, Y, Z) + offset) / vec3(16, 64, 16));
                if (cdata.color.r > 0) {
                    cdata.distance = tmaxz * VOXEL16;
                    cdata.normal = vec3(0, 0, normalize(D.z));

                    if (lod < 1) {
                        CollisionData rcdata = gridDF64(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X, Y, Z) * VOXEL16, offset * VOXEL64X + vec3(X, Y, Z) * VOXEL64X);
                        if (rcdata.distance < INFINITY) {
                            rcdata.distance += cdata.distance;
                            return rcdata;
                        }
                    } else {
                        cdata.color.rgb = getColor(int(cdata.color.r * 255), vec3(X, Y, Z), cdata.normal);
                        return cdata;
                    }
                }
            } else {
                break;
            }
            tmaxz += tz;
        }
    }

    cdata.distance = INFINITY;
    return cdata;
}

CollisionData gridDF4 (vec3 O, vec3 D, vec3 gridPos) {
    CollisionData cdata;
    cdata.distance = INFINITY;

    TMinMax tminmax = boxFarDistance(O, D, gridPos, gridPos + (vec3(VOXEL4X, VOXEL4Y, VOXEL4Z) * VOXEL4));
    if (tminmax.tmax == INFINITY) return cdata;

    vec3 hitPos = O;
    if (tminmax.tmin > 0) {
        hitPos += D * (tminmax.tmin - EPSILON);
    }

    float X, Y, Z;

    X = floor((hitPos - gridPos) / VOXEL4).x;
    Y = floor((hitPos - gridPos) / VOXEL4).y;
    Z = floor((hitPos - gridPos) / VOXEL4).z;

    cdata.color = getTexture3D(colorlod4, vec3(X, Y, Z) / vec3(4, 16, 4));
    if (cdata.color.r > 0 && lod < 2) {
        cdata.distance = 0;
        cdata.normal = vec3(normalize(D.x), 0, 0);

        CollisionData rcdata = gridDF16(hitPos, D, gridPos + vec3(X * VOXEL4, Y * VOXEL4, Z * VOXEL4), vec3(X, Y, Z) * VOXEL16X);
        if (rcdata.distance < INFINITY) return rcdata;
    }

    float tx, ty, tz;

    tx = length(D.xyz / D.x);
    ty = length(D.xyz / D.y);
    tz = length(D.xyz / D.z);

    float tmaxx, tmaxy, tmaxz;

    tmaxx = ((hitPos - gridPos) / VOXEL4).x - X;
    if (D.x > 0) tmaxx = 1 - tmaxx;
    tmaxy = ((hitPos - gridPos) / VOXEL4).y - Y;
    if (D.y > 0) tmaxy = 1 - tmaxy;
    tmaxz = ((hitPos - gridPos) / VOXEL4).z - Z;
    if (D.z > 0) tmaxz = 1 - tmaxz;
    tmaxx = tmaxx * tx;
    tmaxy = tmaxy * ty;
    tmaxz = tmaxz * tz;

    float Xstep, Ystep, Zstep;

    if (D.x > 0) Xstep = 1;
    else Xstep = -1;
    if (D.y > 0) Ystep = 1;
    else Ystep = -1;
    if (D.z > 0) Zstep = 1;
    else Zstep = -1;

    while (true) {
        if (tmaxx < tmaxy && tmaxx < tmaxz) {
            X += Xstep;
            if (X >= 0 && X < VOXEL4X) {
                cdata.color = getTexture3D(colorlod4, vec3(X, Y, Z) / vec3(4, 16, 4));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O.xyz - hitPos.xyz) + (tmaxx * VOXEL4);
                    cdata.normal = vec3(normalize(D.x), 0, 0);

                    if (lod < 2) {
                        CollisionData rcdata = gridDF16(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X, Y, Z) * VOXEL4, vec3(X, Y, Z) * VOXEL16X);
                        if (rcdata.distance < INFINITY) {
                            rcdata.distance += cdata.distance;
                            return rcdata;
                        }
                    } else {
                        cdata.color.rgb = getColor(int(cdata.color.r * 255), vec3(X, Y, Z), cdata.normal);
                        return cdata;
                    }

                }
            } else {
                break;
            }
            tmaxx += tx;
        } else if (tmaxy < tmaxz) {
            Y += Ystep;
            if (Y >= 0 && Y < VOXEL4Y) {
                cdata.color = getTexture3D(colorlod4, vec3(X, Y, Z) / vec3(4, 16, 4));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxy * VOXEL4);
                    cdata.normal = vec3(0, normalize(D.y), 0);

                    if (lod < 2) {
                        CollisionData rcdata = gridDF16(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X, Y, Z) * VOXEL4, vec3(X, Y, Z) * VOXEL16X);
                        if (rcdata.distance < INFINITY) {
                            rcdata.distance += cdata.distance;
                            return rcdata;
                        }
                    } else {
                        cdata.color.rgb = getColor(int(cdata.color.r * 255), vec3(X, Y, Z), cdata.normal);
                        return cdata;
                    }
                }
            } else {
                break;
            }
            tmaxy += ty;
        } else {
            Z += Zstep;
            if (Z >= 0 && Z < VOXEL4Z) {
                cdata.color = getTexture3D(colorlod4, vec3(X, Y, Z) / vec3(4, 16, 4));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxz * VOXEL4);
                    cdata.normal = vec3(0, 0, normalize(D.z));

                    if (lod < 2) {
                        CollisionData rcdata = gridDF16(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X, Y, Z) * VOXEL4, vec3(X, Y, Z) * VOXEL16X);
                        if (rcdata.distance < INFINITY) {
                            rcdata.distance += cdata.distance;
                            return rcdata;
                        }
                    } else {
                        cdata.color.rgb = getColor(int(cdata.color.r * 255), vec3(X, Y, Z), cdata.normal);
                        return cdata;
                    }
                }
            } else {
                break;
            }
            tmaxz += tz;
        }
    }

    cdata.distance = INFINITY;
    return cdata;
}


CollisionData sceneSDF (vec3 O, vec3 D) {
    CollisionData cdata;

    cdata = gridDF4(O, D, vec3(-64, -255, -64));

    return cdata;
}

float shadows (vec3 O) {
    CollisionData cdata = sceneSDF(O + SUNDIR * EPSILON, SUNDIR);

    if (cdata.distance < INFINITY) return 0.;
    return 1.0;
}

vec3 shading (vec3 O, vec3 N) {
    return SUNCOLOR * SUNFORCE * clamp(dot(N, -SUNDIR), 0, 1);
}

vec3 ambient() {
    return AMBIENTCOLOR * AMBIENTFORCE;
}

vec3 applyFog( in vec3  rgb,      // original color of the pixel
               in float distance, // camera to point distance
               in vec3  rayDir,   // camera to point vector
               in vec3  sunDir )  // sun light direction
{
    float fogAmount = 1.0 - exp( -distance * FOGINTENSITY );
    float sunAmount = max(dot( rayDir, sunDir ), 0.0 );
    vec3  fogColor  = mix(AMBIENTCOLOR, // bluish
                          SUNCOLOR * 4, // yellowish
                           pow(sunAmount, 16.0) );
    return mix( rgb, fogColor, fogAmount );
}

void main () {
    vec3 D = vec3(-(((gl_FragCoord.x - 0.5) / (screen_size.x - 1)) - 0.5) * screen_ratios.x, -(((gl_FragCoord.y - 0.5) / (screen_size.y - 1)) - 0.5) * screen_ratios.y, 1);
    D = normalize(D);
    D = (camera_matrix * vec4(D, 0)).xyz;

    vec3 O = vec3(0, 0, 0);
    O = -(inv_model_matrix * camera_matrix * vec4(O, 1)).xyz;

    vec3 Color = vec3(0, 0, 0);

    CollisionData cdata = sceneSDF(O, D);
    if (cdata.distance < INFINITY) {
        vec3 hitPoint = O + D * cdata.distance;
        //vec3 light = vec3(1);
        Color = cdata.color.rgb;
        //light *= shadows(hitPoint);
        //light *= shading(hitPoint, cdata.normal);
        //light += ambient();
        //Color *= light;
        Color = applyFog(Color.rgb, cdata.distance, D, SUNDIR);

        gl_FragDepth = distance(O, hitPoint) / FARPLANE;
    } else {
       gl_FragDepth = 1.;
    }

    fragColor = vec4(tanh(Color), 1);
}

