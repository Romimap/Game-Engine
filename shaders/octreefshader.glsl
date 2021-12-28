#version 150

uniform sampler3D colorlod8;
uniform sampler3D colorlod64;
uniform sampler3D colorlod1024;


in vec2 v_texcoord;
in vec3 v_position;
in mat4 v_camera_matrix;

//VALUES
const float EPSILON = 0.000001;
const float INFINITY = 67108864; //2^26

//VOXELS
//const int GRIDSIZE = 1024;
//const float VOXELSIZE = 0.01;
const float VOXEL8 = 0.25;
const float VOXEL64 = 0.03125;
const float VOXEL1024 = 0.001953125;
const int VOXEL8I = 8;
const int VOXEL64I = 8;
const int VOXEL1024I = 16;

//LIGHTING
const vec3 SUNDIR = vec3(0.534, 0.801, 0.267);
const float FOGINTENSITY = 0.1;
const vec3 AMBIENTCOLOR = vec3(0.5,0.6,0.7);
const float AMBIENTFORCE = 0.3;
const vec3 SUNCOLOR = vec3(1, 0.97, 0.75);
const float SUNFORCE = 3;
const int NULL = 0;

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

CollisionData gridDF1024 (vec3 O, vec3 D, vec3 gridPos, float voxelsize, int gridsize, vec3 offset) {
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

    X = floor((hitPos - gridPos) / voxelsize).x;
    Y = floor((hitPos - gridPos) / voxelsize).y;
    Z = floor((hitPos - gridPos) / voxelsize).z;

    float tx, ty, tz;

    tx = length(D.xyz / D.x);
    ty = length(D.xyz / D.y);
    tz = length(D.xyz / D.z);

    float tmaxx, tmaxy, tmaxz;

    tmaxx = ((hitPos - gridPos) / voxelsize).x - X;
    if (D.x > 0) tmaxx = 1 - tmaxx;
    tmaxy = ((hitPos - gridPos) / voxelsize).y - Y;
    if (D.y > 0) tmaxy = 1 - tmaxy;
    tmaxz = ((hitPos - gridPos) / voxelsize).z - Z;
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

    do {
        if (tmaxx < tmaxy && tmaxx < tmaxz) {
            X += Xstep;
            if (X >= 0 && X < VOXEL1024I) {
                cdata.color = texture3D(colorlod1024, clamp((vec3(X, Y, Z) + offset) / 1024, vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O.xyz - hitPos.xyz) + (tmaxx * voxelsize);
                    cdata.normal = vec3(normalize(D.x), 0, 0);
                    return cdata;
                }
            } else {
                break;
            }
            tmaxx += tx;
        } else if (tmaxy < tmaxz) {
            Y += Ystep;
            if (Y >= 0 && Y < VOXEL1024I) {
                cdata.color = texture3D(colorlod1024, clamp((vec3(X, Y, Z) + offset) / 1024, vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxy * voxelsize);
                    cdata.normal = vec3(0, normalize(D.y), 0);
                    return cdata;
                }
            } else {
                break;
            }
            tmaxy += ty;
        } else {
            Z += Zstep;
            if (Z >= 0 && Z < VOXEL1024I) {
                cdata.color = texture3D(colorlod1024, clamp((vec3(X, Y, Z) + offset) / 1024, vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxz * voxelsize);
                    cdata.normal = vec3(0, 0, normalize(D.z));
                    return cdata;
                }
            } else {
                break;
            }
            tmaxz += tz;
        }
    } while (   X >= 0 && X < VOXEL1024I
                && Y >= 0 && Y < VOXEL1024I
                && Z >= 0 && Z < VOXEL1024I);

    cdata.distance = INFINITY;
    return cdata;
}


CollisionData gridDF64 (vec3 O, vec3 D, vec3 gridPos, float voxelsize, int gridsize, vec3 offset) {
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

    X = floor((hitPos - gridPos) / voxelsize).x;
    Y = floor((hitPos - gridPos) / voxelsize).y;
    Z = floor((hitPos - gridPos) / voxelsize).z;

    cdata.color = texture3D(colorlod64, clamp((vec3(X, Y, Z) + offset) / 64, vec3(0), vec3(1)));
    if (cdata.color.r > 0) {
        cdata.distance = 0;
        cdata.normal = vec3(normalize(D.x), 0, 0);

        CollisionData rcdata = gridDF1024(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X * voxelsize, Y * voxelsize, Z * voxelsize), VOXEL1024, VOXEL1024I, offset * VOXEL1024I + vec3(X, Y, Z) * VOXEL1024I);
        if (rcdata.distance < INFINITY) return rcdata;
    }

    float tx, ty, tz;

    tx = length(D.xyz / D.x);
    ty = length(D.xyz / D.y);
    tz = length(D.xyz / D.z);

    float tmaxx, tmaxy, tmaxz;

    tmaxx = ((hitPos - gridPos) / voxelsize).x - X;
    if (D.x > 0) tmaxx = 1 - tmaxx;
    tmaxy = ((hitPos - gridPos) / voxelsize).y - Y;
    if (D.y > 0) tmaxy = 1 - tmaxy;
    tmaxz = ((hitPos - gridPos) / voxelsize).z - Z;
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

    do {
        if (tmaxx < tmaxy && tmaxx < tmaxz) {
            X += Xstep;
            if (X >= 0 && X < VOXEL64I) {
                cdata.color = texture3D(colorlod64, clamp((vec3(X, Y, Z) + offset) / 64, vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O.xyz - hitPos.xyz) + (tmaxx * voxelsize);
                    cdata.normal = vec3(normalize(D.x), 0, 0);

                    CollisionData rcdata = gridDF1024(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X * voxelsize, Y * voxelsize, Z * voxelsize), VOXEL1024, VOXEL1024I, offset * VOXEL1024I + vec3(X, Y, Z) * VOXEL1024I);
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
            if (Y >= 0 && Y < VOXEL64I) {
                cdata.color = texture3D(colorlod64, clamp((vec3(X, Y, Z) + offset) / 64, vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxy * voxelsize);
                    cdata.normal = vec3(0, normalize(D.y), 0);

                    CollisionData rcdata = gridDF1024(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X * voxelsize, Y * voxelsize, Z * voxelsize), VOXEL1024, VOXEL1024I, offset * VOXEL1024I + vec3(X, Y, Z) * VOXEL1024I);
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
            if (Z >= 0 && Z < VOXEL64I) {
                cdata.color = texture3D(colorlod64, clamp((vec3(X, Y, Z) + offset) / 64, vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxz * voxelsize);
                    cdata.normal = vec3(0, 0, normalize(D.z));

                    CollisionData rcdata = gridDF1024(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X * voxelsize, Y * voxelsize, Z * voxelsize), VOXEL1024, VOXEL1024I, offset * VOXEL1024I + vec3(X, Y, Z) * VOXEL1024I);
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
    } while (   X >= 0 && X < VOXEL64I
                && Y >= 0 && Y < VOXEL64I
                && Z >= 0 && Z < VOXEL64I);

    cdata.distance = INFINITY;
    return cdata;
}



CollisionData gridDF8 (vec3 O, vec3 D, vec3 gridPos, float voxelsize, int gridsize) {
    CollisionData cdata;
    cdata.distance = INFINITY;

    TMinMax tminmax = boxFarDistance(O, D, gridPos, gridPos + (vec3(1) * gridsize * voxelsize));
    if (tminmax.tmax == INFINITY) return cdata;

    vec3 hitPos = O;
    if (tminmax.tmin > 0) {
        hitPos += D * (tminmax.tmin);
        hitPos -= D * EPSILON;
    }


    float X, Y, Z;

    X = floor((hitPos - gridPos) / voxelsize).x;
    Y = floor((hitPos - gridPos) / voxelsize).y;
    Z = floor((hitPos - gridPos) / voxelsize).z;

    cdata.color = texture3D(colorlod8, clamp(vec3(X, Y, Z) / gridsize, vec3(0), vec3(1)));
    if (cdata.color.r > 0) {
        cdata.distance = 0;
        cdata.normal = vec3(normalize(D.x), 0, 0);
        CollisionData rcdata = gridDF64(hitPos, D, gridPos + vec3(X * voxelsize, Y * voxelsize, Z * voxelsize), VOXEL64, VOXEL64I, vec3(X, Y, Z) * VOXEL64I);
        if (rcdata.distance < INFINITY) return rcdata;
    }

    float tx, ty, tz;

    tx = length(D.xyz / D.x);
    ty = length(D.xyz / D.y);
    tz = length(D.xyz / D.z);

    float tmaxx, tmaxy, tmaxz;

    tmaxx = ((hitPos - gridPos) / voxelsize).x - X;
    if (D.x > 0) tmaxx = 1 - tmaxx;
    tmaxy = ((hitPos - gridPos) / voxelsize).y - Y;
    if (D.y > 0) tmaxy = 1 - tmaxy;
    tmaxz = ((hitPos - gridPos) / voxelsize).z - Z;
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

    do {
        if (tmaxx < tmaxy && tmaxx < tmaxz) {
            X += Xstep;
            if (X >= 0 && X < VOXEL8I) {
                cdata.color = texture3D(colorlod8, clamp(vec3(X, Y, Z) / gridsize, vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O.xyz - hitPos.xyz) + (tmaxx * voxelsize);
                    cdata.normal = vec3(normalize(D.x), 0, 0);

                    CollisionData rcdata = gridDF64(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X * voxelsize, Y * voxelsize, Z * voxelsize), VOXEL64, VOXEL64I, vec3(X, Y, Z) * VOXEL64I);
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
            if (Y >= 0 && Y < VOXEL8I) {
                cdata.color = texture3D(colorlod8, clamp(vec3(X, Y, Z) / gridsize, vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxy * voxelsize);
                    cdata.normal = vec3(0, normalize(D.y), 0);

                    CollisionData rcdata = gridDF64(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X * voxelsize, Y * voxelsize, Z * voxelsize), VOXEL64, VOXEL64I, vec3(X, Y, Z) * VOXEL64I);
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
            if (Z >= 0 && Z < VOXEL8I) {
                cdata.color = texture3D(colorlod8, clamp(vec3(X, Y, Z) / gridsize, vec3(0), vec3(1)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxz * voxelsize);
                    cdata.normal = vec3(0, 0, normalize(D.z));

                    CollisionData rcdata = gridDF64(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X * voxelsize, Y * voxelsize, Z * voxelsize), VOXEL64, VOXEL64I, vec3(X, Y, Z) * VOXEL64I);
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
    } while (   X >= 0 && X < VOXEL8I
                && Y >= 0 && Y < VOXEL8I
                && Z >= 0 && Z < VOXEL8I);

    cdata.distance = INFINITY;
    return cdata;
}


CollisionData sceneSDF (vec3 O, vec3 D) {
    CollisionData cdata;

    cdata = gridDF8(O, D, vec3(-1, -1, -1), VOXEL8, VOXEL8I);

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
    vec3 Color = vec3(0, 0, 0);
    vec3 O = vec3(0, 0, 0);
    vec3 D = vec3(v_texcoord.x - 0.5, v_texcoord.y - 0.5, 0.5);
    D = normalize(D);

    O = (v_camera_matrix * vec4(O, 1)).xyz;
    D = (v_camera_matrix * vec4(D, 0)).xyz;

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

