#version 150

uniform sampler3D colorlod1;
uniform sampler3D colorlod4;
uniform sampler3D colorlod16;
uniform sampler3D colorlod64;
uniform sampler3D colorlod256;
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
const float SUBDIV = 4;

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

CollisionData gridDF16 (vec3 O, vec3 D, vec3 gridPos, float voxelsize, int gridsize, vec3 offset) {
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

    //cdata.color = texture3D(colorlod16, vec3(X, Y, Z) / gridsize);
    //if (cdata.color.r > 0) {
    //    cdata.distance = 0;
    //    cdata.normal = vec3(normalize(D.x), 0, 0);
    //    return cdata;
    //}

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
            if (X >= 0 && X < SUBDIV) {
                cdata.color = texture3D(colorlod16, clamp(vec3(X, Y, Z) / gridsize, vec3(0.001), vec3(0.999)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O.xyz - hitPos.xyz) + (tmaxx * voxelsize);
                    cdata.normal = vec3(normalize(D.x), 0, 0);

                    //CollisionData rcdata = gridDF64(O + D * cdata.distance, D, gridPos + vec3(X * tx, Y * ty, Z * tz), level + 1, voxelsize / SUBDIV, int(gridsize * SUBDIV));
                    //if (rcdata.valid) return rcdata;

                    return cdata;
                }
            } else {
                break;
            }
            tmaxx += tx;
        } else if (tmaxy < tmaxz) {
            Y += Ystep;
            if (Y >= 0 && Y < SUBDIV) {
                cdata.color = texture3D(colorlod16, clamp(vec3(X, Y, Z) / gridsize, vec3(0.001), vec3(0.999)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxy * voxelsize);
                    cdata.normal = vec3(0, normalize(D.y), 0);

                    //CollisionData rcdata = gridDF64(O + D * cdata.distance, D, gridPos + vec3(X * tx, Y * ty, Z * tz), level + 1, voxelsize / SUBDIV, int(gridsize * SUBDIV));
                    //if (rcdata.valid) return rcdata;

                    return cdata;
                }
            } else {
                break;
            }
            tmaxy += ty;
        } else {
            Z += Zstep;
            if (Z >= 0 && Z < SUBDIV) {
                cdata.color = texture3D(colorlod16, clamp(vec3(X, Y, Z) / gridsize, vec3(0.001), vec3(0.999)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxz * voxelsize);
                    cdata.normal = vec3(0, 0, normalize(D.z));

                    //CollisionData rcdata = gridDF64(O + D * cdata.distance, D, gridPos + vec3(X * tx, Y * ty, Z * tz), level + 1, voxelsize / SUBDIV, int(gridsize * SUBDIV));
                    //if (rcdata.valid) return rcdata;

                    return cdata;
                }
            } else {
                break;
            }
            tmaxz += tz;
        }
    } while (   X >= 0 && X < SUBDIV
                && Y >= 0 && Y < SUBDIV
                && Z >= 0 && Z < SUBDIV);

    cdata.distance = INFINITY;
    return cdata;
}


CollisionData gridDF4 (vec3 O, vec3 D, vec3 gridPos, float voxelsize, int gridsize) {
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

    cdata.color = texture3D(colorlod4, clamp(vec3(X, Y, Z) / gridsize, vec3(0.001), vec3(0.999)));
    if (cdata.color.r > 0) {
        cdata.distance = 0;
        cdata.normal = vec3(normalize(D.x), 0, 0);
        CollisionData rcdata = gridDF16(hitPos, D, gridPos + vec3(X * voxelsize, Y * voxelsize, Z * voxelsize), voxelsize / SUBDIV, int(gridsize * SUBDIV), vec3(X, Y, Z) * SUBDIV);
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
            if (X >= 0 && X < SUBDIV) {
                cdata.color = texture3D(colorlod4, clamp(vec3(X, Y, Z) / gridsize, vec3(0.001), vec3(0.999)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O.xyz - hitPos.xyz) + (tmaxx * voxelsize);
                    cdata.normal = vec3(normalize(D.x), 0, 0);

                    CollisionData rcdata = gridDF16(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X * voxelsize, Y * voxelsize, Z * voxelsize), voxelsize / SUBDIV, int(gridsize * SUBDIV), vec3(X, Y, Z) * SUBDIV);
                    if (rcdata.distance < INFINITY) return rcdata;
                }
            } else {
                break;
            }
            tmaxx += tx;
        } else if (tmaxy < tmaxz) {
            Y += Ystep;
            if (Y >= 0 && Y < SUBDIV) {
                cdata.color = texture3D(colorlod4, clamp(vec3(X, Y, Z) / gridsize, vec3(0.001), vec3(0.999)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxy * voxelsize);
                    cdata.normal = vec3(0, normalize(D.y), 0);

                    CollisionData rcdata = gridDF16(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X * voxelsize, Y * voxelsize, Z * voxelsize), voxelsize / SUBDIV, int(gridsize * SUBDIV), vec3(X, Y, Z) * SUBDIV);
                    if (rcdata.distance < INFINITY) return rcdata;
                }
            } else {
                break;
            }
            tmaxy += ty;
        } else {
            Z += Zstep;
            if (Z >= 0 && Z < SUBDIV) {
                cdata.color = texture3D(colorlod4, clamp(vec3(X, Y, Z) / gridsize, vec3(0.001), vec3(0.999)));
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxz * voxelsize);
                    cdata.normal = vec3(0, 0, normalize(D.z));

                    CollisionData rcdata = gridDF16(O + D * (cdata.distance - EPSILON), D, gridPos + vec3(X * voxelsize, Y * voxelsize, Z * voxelsize), voxelsize / SUBDIV, int(gridsize * SUBDIV),  vec3(X, Y, Z) * SUBDIV);
                    if (rcdata.distance < INFINITY) return rcdata;
                }
            } else {
                break;
            }
            tmaxz += tz;
        }
    } while (   X >= 0 && X < SUBDIV
                && Y >= 0 && Y < SUBDIV
                && Z >= 0 && Z < SUBDIV);

    cdata.distance = INFINITY;
    return cdata;
}



CollisionData sceneSDF (vec3 O, vec3 D) {
    CollisionData cdata;

    cdata = gridDF4(O, D, vec3(-.4, -.4, 0), .1, 4);

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
        light *= shadows(hitPoint);
        light *= shading(hitPoint, cdata.normal);
        light += ambient();

        Color.rgb += light;
    } else {
        vec3 light = vec3(1);
    }

    Color = applyFog(Color.rgb, cdata.distance, D, SUNDIR);

    gl_FragColor = vec4(tanh(Color), 1);
}

