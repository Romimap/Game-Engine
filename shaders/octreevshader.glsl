#version 130

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 inv_projection_matrix;
uniform mat4 inv_view_matrix;
uniform mat4 inv_model_matrix;

in vec3 a_position;
in vec2 a_texcoord;

const mat4 plane_transform = mat4(1.0, 0.0, 0.0, 0.0,
                                  0.0, 0.0, 1.0, 0.0,
                                  0.0,-1.0, 0.0, 0.0,
                                  0.0, 0.0, 0.0, 1.0);

void main() {
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(a_position, 1);
}
