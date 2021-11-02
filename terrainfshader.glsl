#version 150

uniform sampler2D heightmap;
uniform sampler2D grassTex;
uniform sampler2D rockTex;
uniform sampler2D snowTex;

in vec2 v_texcoord;
in vec3 v_position;
in float v_height;
in vec3 v_normal;
//! [0]

float remap (float value, float low1, float high1, float low2, float high2) {
   return clamp(low2 + (value - low1) * (high2 - low2) / (high1 - low1), low2, high2);
}

void main() {
    vec3 normal;
    normal.y = 0.015;
    normal.x = texture2D(heightmap, v_texcoord).r - texture2D(heightmap, v_texcoord + vec2(0.01, 0)).r;
    normal.z = texture2D(heightmap, v_texcoord).r - texture2D(heightmap, v_texcoord + vec2(0, 0.01)).r;
    normal = normalize(normal);

    float light = dot(vec3(0, 1, 0), normal);
    float rockfactor = remap(normal.y,  0.65, 0.70, 0, 1);
    float grassfactor = 1 - remap(v_height, 0.6, 0.65, 0, 1);

    vec4 grass = texture2D(grassTex, v_texcoord);
    vec4 rock = texture2D(rockTex, v_texcoord);
    vec4 snow = texture2D(snowTex, v_texcoord);

    gl_FragColor = mix(rock, mix(grass, snow, grassfactor), rockfactor) * light;
}
//! [0]

