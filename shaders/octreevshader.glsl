#version 150

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

in vec3 a_position;
in vec2 a_texcoord;

out vec3 v_position;
out vec2 v_texcoord;
out mat4 v_camera_matrix;

const mat4 plane_transform = mat4(1.0, 0.0, 0.0, 0.0,
                                  0.0, 0.0, 1.0, 0.0,
                                  0.0,-1.0, 0.0, 0.0,
                                  0.0, 0.0, 0.0, 1.0);

void main() {


    gl_Position = plane_transform * vec4(a_position, 1);

    v_camera_matrix = view_matrix;

    v_position = a_position;
    v_texcoord = a_texcoord;
}
