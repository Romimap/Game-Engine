#version 150

uniform sampler3D colorlod0;

in vec2 v_texcoord;
in vec3 v_position;
in mat4 v_camera_matrix;


const int MAXTRACE = 3;
const float MINDIST = 0.0001;
const float EPSILON = 0.000001;
const float INFINITY = 99999999;
const float MAXDIST = 1000;
const int GRIDSIZE = 8;
const float VOXELSIZE = 0.1;
const vec3 light = vec3(0, 2, 0);
const float LIGHTFORCE = 5;
const float FOGSAMPLEDISTANCE = 0.02;

struct CollisionData {
    float distance;
    vec3 normal;
    vec4 color;
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

CollisionData gridDF (vec3 O, vec3 D, vec3 gridPos, sampler3D grid) {
    CollisionData cdata;
    cdata.distance = INFINITY;

    TMinMax tminmax = boxFarDistance(O, D, gridPos, gridPos + (vec3(1) * GRIDSIZE * VOXELSIZE));
    if (tminmax.tmin == INFINITY) return cdata;

    vec3 hitPos = O;
    if (tminmax.tmin > 0) {
        hitPos += D * (tminmax.tmin);
        hitPos -= D * EPSILON;
    }


    float X, Y, Z;

    X = floor((hitPos - gridPos) / VOXELSIZE).x;
    Y = floor((hitPos - gridPos) / VOXELSIZE).y;
    Z = floor((hitPos - gridPos) / VOXELSIZE).z;

    //cdata.color = texture3D(colorlod0, vec3(X, Y, Z) / GRIDSIZE);
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

    tmaxx = ((hitPos - gridPos) / VOXELSIZE).x - X;
    if (D.x > 0) tmaxx = 1 - tmaxx;
    tmaxy = ((hitPos - gridPos) / VOXELSIZE).y - Y;
    if (D.y > 0) tmaxy = 1 - tmaxy;
    tmaxz = ((hitPos - gridPos) / VOXELSIZE).z - Z;
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
            if (X >= 0 && X < GRIDSIZE) {
                cdata.color = texture3D(colorlod0, vec3(X, Y, Z) / GRIDSIZE);
                if (cdata.color.r > 0) {
                    cdata.distance = length(O.xyz - hitPos.xyz) + (tmaxx * VOXELSIZE);
                    cdata.normal = vec3(normalize(D.x), 0, 0);
                    return cdata;
                }
            } else {
                break;
            }
            tmaxx += tx;
        } else if (tmaxy < tmaxz) {
            Y += Ystep;
            if (Y >= 0 && Y < GRIDSIZE) {
                cdata.color = texture3D(colorlod0, vec3(X, Y, Z) / GRIDSIZE);
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxy * VOXELSIZE);
                    cdata.normal = vec3(0, normalize(D.y), 0);
                    return cdata;
                }
            } else {
                break;
            }
            tmaxy += ty;
        } else {
            Z += Zstep;
            if (Z >= 0 && Z <  GRIDSIZE) {
                cdata.color = texture3D(colorlod0, vec3(X, Y, Z) / GRIDSIZE);
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxz * VOXELSIZE);
                    cdata.normal = vec3(0, 0, normalize(D.z));
                    return cdata;
                }
            } else {
                break;
            }
            tmaxz += tz;
        }
    } while (   X >= 0 && X < GRIDSIZE
                && Y >= 0 && Y < GRIDSIZE
                && Z >= 0 && Z < GRIDSIZE);

    cdata.distance = INFINITY;
    return cdata;
}

CollisionData sceneSDF (vec3 O, vec3 D) {
    CollisionData cdata;

    cdata = gridDF(O, D, vec3(-.4, -.4, 0), colorlod0);

    return cdata;
}

float shadows (vec3 O) {
    vec3 D = light - O;
    float len = length(D);
    D = normalize(D);

    CollisionData cdata = sceneSDF(O + D * EPSILON, D);

    if (cdata.distance < len) return 0.;
    return 1.0;
}

float shading (vec3 O, vec3 N) {
    vec3 D = light - O;
    float len = length(D);
    D = normalize(D);

    return clamp(dot(N, -D), 0, 1);
}

float lightforce (vec3 O) {
    vec3 D = light - O;
    float len = length(D);

    return LIGHTFORCE / pow(len, 3);
}

float ambient() {
    return 0.1;
}

vec4 fog (vec3 O, vec3 D, float distance, vec4 color) {
    vec3 ans = vec3(0);
    for (float d = 0; d < distance; d += FOGSAMPLEDISTANCE) {
        O += D * FOGSAMPLEDISTANCE;
        if (shadows(O) > 0) ans += color.rgb * color.a * lightforce(O);
    }
    return vec4(ans, 0);
}



void main () {
    vec4 Color = vec4(0, 0, 0, 0);
    vec4 FogColor = vec4(1, 1, 1, 0.005);
    vec3 O = vec3(0, 0, 0);
    vec3 D = vec3(v_texcoord.x - 0.5, v_texcoord.y - 0.5, 0.5);
    D = normalize(D);

    O = (v_camera_matrix * vec4(O, 1)).xyz;
    D = (v_camera_matrix * vec4(D, 0)).xyz;

    CollisionData cdata = sceneSDF(O, D);
    if (cdata.distance < INFINITY) {
        Color.a = 1;
        vec3 hitPoint = O + D * cdata.distance;
        vec3 light = vec3(1);
        light *= shadows(hitPoint);
        light *= shading(hitPoint, cdata.normal);
        light *= lightforce(hitPoint);
        light += ambient();
        Color += fog(O, D, cdata.distance, FogColor);

        Color.rgb += light;
    } else {
        vec3 light = vec3(1);
        Color += fog(O, D, 10, FogColor);
    }

    gl_FragColor = Color;
}

