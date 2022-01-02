#version 150

uniform sampler3D colorlod4;
uniform sampler3D colorlod16;
uniform sampler3D colorlod64;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 inv_projection_matrix;
uniform mat4 inv_view_matrix;
uniform mat4 inv_model_matrix;

uniform vec2 screen_ratios;
uniform vec2 screen_size;

//VALUES
const float EPSILON = 0.0001;
const float INFINITY = 67108864; //2^26
const int NULL = 0;

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
const vec3 SUNDIR = vec3(0.534, -0.801, 0.267);
const float FOGINTENSITY = 0.01;
const vec3 AMBIENTCOLOR = vec3(0.5,0.6,0.7);
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
                cdata.color = texture3D(colorlod64, clamp((vec3(X, Y, Z) + offset) / vec3(64, 512, 64), vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O.xyz - hitPos.xyz) + (tmaxx * VOXEL64);
                    cdata.normal = vec3(normalize(D.x), 0, 0);
                    return cdata;
                }
            } else {
                break;
            }
            tmaxx += tx;
        } else if (tmaxy < tmaxz) {
            Y += Ystep;
            if (Y >= 0 && Y < VOXEL64Y) {
                cdata.color = texture3D(colorlod64, clamp((vec3(X, Y, Z) + offset) / vec3(64, 512, 64), vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxy * VOXEL64);
                    cdata.normal = vec3(0, normalize(D.y), 0);
                    return cdata;
                }
            } else {
                break;
            }
            tmaxy += ty;
        } else {
            Z += Zstep;
            if (Z >= 0 && Z < VOXEL64Z) {
                cdata.color = texture3D(colorlod64, clamp((vec3(X, Y, Z) + offset) / vec3(64, 512, 64), vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxz * VOXEL64);
                    cdata.normal = vec3(0, 0, normalize(D.z));
                    return cdata;
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


CollisionData gridDF16 (vec3 O, vec3 D, vec3 gridPos, vec3 offset) {
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

    X = floor((hitPos - gridPos) / VOXEL16).x;
    Y = floor((hitPos - gridPos) / VOXEL16).y;
    Z = floor((hitPos - gridPos) / VOXEL16).z;

    cdata.color = texture3D(colorlod16, clamp((vec3(X, Y, Z) + offset) / vec3(16, 128, 16), vec3(0), vec3(1))); //NOTE: Dont forget to set 64 as vec3(16, 128, 16)
    if (cdata.color.r > 0) {
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
                cdata.color = texture3D(colorlod16, clamp((vec3(X, Y, Z) + offset) / vec3(16, 128, 16), vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O.xyz - hitPos.xyz) + (tmaxx * VOXEL16);
                    cdata.normal = vec3(normalize(D.x), 0, 0);

                    CollisionData rcdata = gridDF64(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X, Y, Z) * VOXEL16, offset * VOXEL64X + vec3(X, Y, Z) * VOXEL64X);
                    if (rcdata.distance < INFINITY) {
                        rcdata.distance += cdata.distance;
                        return rcdata;
                    }
                }
            } else {
                break;
            }
            tmaxx += tx;
        } else if (tmaxy < tmaxz) {
            Y += Ystep;
            if (Y >= 0 && Y < VOXEL16Y) {
                cdata.color = texture3D(colorlod16, clamp((vec3(X, Y, Z) + offset) / vec3(16, 128, 16), vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxy * VOXEL16);
                    cdata.normal = vec3(0, normalize(D.y), 0);

                    CollisionData rcdata = gridDF64(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X, Y, Z) * VOXEL16, offset * VOXEL64X + vec3(X, Y, Z) * VOXEL64X);
                    if (rcdata.distance < INFINITY) {
                        rcdata.distance += cdata.distance;
                        return rcdata;
                    }
                }
            } else {
                break;
            }
            tmaxy += ty;
        } else {
            Z += Zstep;
            if (Z >= 0 && Z < VOXEL16Z) {
                cdata.color = texture3D(colorlod16, clamp((vec3(X, Y, Z) + offset) / vec3(16, 128, 16), vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxz * VOXEL16);
                    cdata.normal = vec3(0, 0, normalize(D.z));

                    CollisionData rcdata = gridDF64(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X, Y, Z) * VOXEL16, offset * VOXEL64X + vec3(X, Y, Z) * VOXEL64X);
                    if (rcdata.distance < INFINITY) {
                        rcdata.distance += cdata.distance;
                        return rcdata;
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

    cdata.color = texture3D(colorlod4, clamp(vec3(X, Y, Z) / vec3(4, 32, 4), vec3(0), vec3(1)));
    if (cdata.color.r > 0) {
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
                cdata.color = texture3D(colorlod4, clamp(vec3(X, Y, Z) / vec3(4, 32, 4), vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O.xyz - hitPos.xyz) + (tmaxx * VOXEL4);
                    cdata.normal = vec3(normalize(D.x), 0, 0);

                    CollisionData rcdata = gridDF16(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X, Y, Z) * VOXEL4, vec3(X, Y, Z) * VOXEL16X);
                    if (rcdata.distance < INFINITY) {
                        rcdata.distance += cdata.distance;
                        return rcdata;
                    }
                }
            } else {
                break;
            }
            tmaxx += tx;
        } else if (tmaxy < tmaxz) {
            Y += Ystep;
            if (Y >= 0 && Y < VOXEL4Y) {
                cdata.color = texture3D(colorlod4, clamp(vec3(X, Y, Z) / vec3(4, 32, 4), vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxy * VOXEL4);
                    cdata.normal = vec3(0, normalize(D.y), 0);

                    CollisionData rcdata = gridDF16(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X, Y, Z) * VOXEL4, vec3(X, Y, Z) * VOXEL16X);
                    if (rcdata.distance < INFINITY) {
                        rcdata.distance += cdata.distance;
                        return rcdata;
                    }
                }
            } else {
                break;
            }
            tmaxy += ty;
        } else {
            Z += Zstep;
            if (Z >= 0 && Z < VOXEL4Z) {
                cdata.color = texture3D(colorlod4, clamp(vec3(X, Y, Z) / vec3(4, 32, 4), vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxz * VOXEL4);
                    cdata.normal = vec3(0, 0, normalize(D.z));

                    CollisionData rcdata = gridDF16(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X, Y, Z) * VOXEL4, vec3(X, Y, Z) * VOXEL16X);
                    if (rcdata.distance < INFINITY) {
                        rcdata.distance += cdata.distance;
                        return rcdata;
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

    cdata = gridDF4(O, D, vec3(0, -255, 0));

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
    vec3 D = vec3(-(((gl_FragCoord.x) / (screen_size.x)) - 0.5) * screen_ratios.x, -(((gl_FragCoord.y) / (screen_size.y)) - 0.5) * screen_ratios.y, 1);
    D = normalize(D);
    D = (camera_matrix * vec4(D, 0)).xyz;

    vec3 O = vec3(0, 0, 0);
    O = -(camera_matrix * vec4(O, 1)).xyz;

    vec3 Color = vec3(0, 0, 0); //NOTE: set that as 0

    CollisionData cdata = sceneSDF(O, D);
    if (cdata.distance < INFINITY) {
        vec3 hitPoint = O + D * cdata.distance;
        vec3 light = vec3(1);
        //light *= shadows(hitPoint);
        light *= shading(hitPoint, cdata.normal);
        light += ambient();
        Color.rgb += light;
        gl_FragDepth = cdata.distance / INFINITY;
        Color = applyFog(Color.rgb, cdata.distance, D, SUNDIR);
    } else {
       gl_FragDepth = 1.;
    }


    gl_FragColor = vec4(tanh(Color), 1);
}

