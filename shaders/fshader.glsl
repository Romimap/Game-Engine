#version 150

uniform sampler3D colorlod0;

in vec2 v_texcoord;
in vec3 v_position;
in mat4 v_camera_matrix;


const int MAXTRACE = 100;
const float MINDIST = 0.01;
const float INFINITY = 9999999;
const float MAXDIST = 1000;
const int GRIDSIZE = 8;
const float VOXELSIZE = 0.1;
const vec3 light = vec3(0, 1, 1);

struct CollisionData {
    float distance;
    vec4 color;
};

float cubeSDF (vec3 p, vec3 b, vec4 color) {
    vec3 q = abs(p) - b;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

CollisionData sceneSDF (vec3 O, vec3 D) {
    CollisionData cdata;
    cdata.color = vec4(1, 1, 1, 1);
    cdata.distance = INFINITY;

    for (int x = 0; x < GRIDSIZE; x++) {
        for (int y = 0; y < GRIDSIZE; y++) {
            for (int z = 0; z < GRIDSIZE; z++) {
                float r = texture3D(colorlod0, vec3(x, y, z) / GRIDSIZE).r;
                if (r > 0) {
                    float d = cubeSDF(O - vec3(x, y, z), vec3(0.5, 0.5, 0.5), vec4(x, y, z, 1) / float(GRIDSIZE));
                    if (d < cdata.distance) cdata.distance = d;
                }
            }
        }
    }

    return cdata;
}

void main () {
    vec4 Color = vec4(0.1, 0.3, 0.4, 1);
    vec3 O = vec3(0, 0, -10);
    vec3 D = vec3(v_texcoord.x - 0.5, v_texcoord.y - 0.5, 0.5);
    D = normalize(D);

    //O = v_camera_matrix * O;
    //D = v_camera_matrix * D;

    for(int i = 0; i < MAXTRACE; i++) {
        CollisionData cdata = sceneSDF(O, D);
        if (cdata.distance < MINDIST) {
            Color = cdata.color;
            break;
        }
        O = O + D * cdata.distance;
    }

    gl_FragColor = Color;
}

