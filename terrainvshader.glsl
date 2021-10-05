#version 150

uniform mat4 mvp_matrix;
uniform sampler2D heightmap;
uniform float heightMult = 1.;

in vec4 a_position;
in vec2 a_texcoord;
in vec3 a_normal;

out vec2 v_texcoord;
out float v_height;
out vec3 v_normal;

//! [0]
void main()
{
    float height = 1. - texture2D(heightmap, a_texcoord).r;
    v_height = height;

    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * (a_position + vec4(0, height * heightMult, 0, 0));
    v_normal = a_normal;
    v_texcoord = a_texcoord;
}
//! [0]
