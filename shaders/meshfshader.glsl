#version 150

uniform sampler2D color;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 inv_projection_matrix;
uniform mat4 inv_view_matrix;
uniform mat4 inv_model_matrix;

uniform vec2 screen_ratios;
uniform vec2 screen_size;

in vec2 v_texcoord;

void main () {
    gl_FragColor = vec4(texture2D(color, v_texcoord).rgb, 1);
}

