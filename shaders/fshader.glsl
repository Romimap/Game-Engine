#version 150

uniform sampler3D colorlod0;

in vec2 v_texcoord;
in vec3 v_position;
in mat4 v_camera_matrix;


const int MAXTRACE = 3;
const float MINDIST = 0.0001;
const float INFINITY = 99999999;
const float MAXDIST = 1000;
const int GRIDSIZE = 8;
const float VOXELSIZE = 0.1;
const vec4 light = vec4(0, 1, 1, 1);

struct CollisionData {
    float distance;
    vec4 normal;
    vec4 color;
};

struct TMinMax {
    float tmin;
    float tmax;
};

TMinMax boxFarDistance (vec4 O, vec4 D, vec4 min, vec4 max) {
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

CollisionData gridDF (vec4 O, vec4 D, vec4 gridPos, sampler3D grid) {
    CollisionData cdata;
    cdata.distance = INFINITY;

    TMinMax tminmax = boxFarDistance(O, D, gridPos, gridPos + vec4((vec3(1) * GRIDSIZE * VOXELSIZE), 1));
    if (tminmax.tmin == INFINITY) return cdata;

    vec4 hitPos = O;
    if (tminmax.tmin > 0) hitPos += D * (tminmax.tmin + 0.01);

    float X, Y, Z;

    X = floor((hitPos - gridPos) / VOXELSIZE).x;
    Y = floor((hitPos - gridPos) / VOXELSIZE).y;
    Z = floor((hitPos - gridPos) / VOXELSIZE).z;

    cdata.color = texture3D(colorlod0, vec3(X, Y, Z) / GRIDSIZE);
    if (cdata.color.r > 0) {
        cdata.distance = 0;
        cdata.normal = vec4(normalize(D.x), 0, 0, 1);
        return cdata;
    }

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
            tmaxx += tx;
            X += Xstep;
            cdata.color = texture3D(colorlod0, vec3(X, Y, Z) / GRIDSIZE);
            if (X >= 0 && X < GRIDSIZE) {
                cdata.color = texture3D(colorlod0, vec3(X, Y, Z) / GRIDSIZE);
                if (cdata.color.r > 0) {
                    cdata.distance = length(O.xyz - hitPos.xyz) + (tmaxx * VOXELSIZE);
                    cdata.normal = vec4(normalize(D.x), 0, 0, 1);
                    return cdata;
                }
            } else {
                break;
            }
        } else if (tmaxy < tmaxz) {
            tmaxy += ty;
            Y += Ystep;
            cdata.color = texture3D(colorlod0, vec3(X, Y, Z) / GRIDSIZE);
            if (Y >= 0 && Y < GRIDSIZE) {
                cdata.color = texture3D(colorlod0, vec3(X, Y, Z) / GRIDSIZE);
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxy * VOXELSIZE);
                    cdata.normal = vec4(0, normalize(D.y), 0, 1);
                    return cdata;
                }
            } else {
                break;
            }
        } else {
            tmaxz += tz;
            Z += Zstep;
            if (Z >= 0 && Z < GRIDSIZE) {
                cdata.color = texture3D(colorlod0, vec3(X, Y, Z) / GRIDSIZE);
                if (cdata.color.r > 0) {
                    cdata.distance = length(O - hitPos) + (tmaxz * VOXELSIZE);
                    cdata.normal = vec4(0, 0, normalize(D.z), 1);
                    return cdata;
                }
            } else {
                break;
            }
        }
    } while (   X >= 0 && X < GRIDSIZE
                && Y >= 0 && Y < GRIDSIZE
                && Z >= 0 && Z < GRIDSIZE);

    cdata.distance = INFINITY;
    return cdata;
}

CollisionData sceneSDF (vec4 O, vec4 D) {
    CollisionData cdata;

    cdata = gridDF(O, D, vec4(-.4, -.4, 0, 1), colorlod0);

    return cdata;
}

float shadows (vec4 O) {
    vec4 D = light - O;
    float len = length(D.xyz);
    D = normalize(D);

    CollisionData cdata = sceneSDF(O + (D * 0.001), D);

    if (cdata.distance < INFINITY) return 0.;
    return 1.0;
}

float shading (vec4 O, vec4 N) {
    vec4 D = light - O;
    float len = length(D.xyz);
    D = normalize(D);

    return clamp(dot(N, -D), 0, 1);
}

float lightforce (vec4 O) {
    vec4 D = light - O;
    float len = length(D.xyz);

    return 1.0 / pow(len, 3);
}

float ambient() {
    return 0.1;
}

void main () {
    vec4 Color = vec4(0, 0, 0, 0);
    vec4 O = vec4(0, 0, 0, 1);
    vec4 D = vec4(v_texcoord.x - 0.5, v_texcoord.y - 0.5, 0.5, 0);
    D = normalize(D);

    //O = v_camera_matrix * O;
    //D = v_camera_matrix * D;

    CollisionData cdata = sceneSDF(O, D);
    if (cdata.distance < INFINITY) {
        Color.a = 1;

        vec3 light = vec3(1);
        light *= shadows(O + D * cdata.distance);
        light *= shading(O + D * cdata.distance, cdata.normal);
        light *= lightforce(O + D * cdata.distance);
        light += ambient();

        Color.rgb += light;
    }

    if (Color.a == 0) Color = vec4(0);
    gl_FragColor = Color;
}

