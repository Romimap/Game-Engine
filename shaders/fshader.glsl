#version 150

uniform sampler2D color;

in vec2 v_texcoord;
in vec3 v_position;
in vec3 v_normal;
in vec3 v_world_normal;

void main()
{
    // Set fragment color from texture
    gl_FragColor = vec4(v_world_normal, 1);
}

