#version 150

uniform sampler2D color;

in vec2 v_texcoord;
in vec3 v_position;
in vec3 v_normal;
in vec3 v_world_normal;
in vec3 v_world_position;

void main()
{
    // Set fragment color from texture
    vec3 n = normalize(v_world_normal);
    vec3 l = vec3(0, 10, 0);
    vec3 toLight = l - v_world_position;
    vec3 nToLight = normalize(toLight);
    float luminosity = clamp(dot(n, nToLight), 0.0, 1.0) * 1/length(v_world_position - toLight) * 7 + 0.1;
    gl_FragColor = texture2D(color, v_texcoord) * tanh(luminosity);
}

