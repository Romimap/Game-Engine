#version 150

uniform sampler2D color;

in vec2 v_texcoord;
in vec3 v_position;
in vec3 v_normal;
in vec3 v_world_normal;
in vec3 v_world_position;

void main()
{
    gl_FragColor = texture2D(color, v_texcoord);
}

