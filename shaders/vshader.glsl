#version 150

uniform mat4 mvp_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

in vec3 a_position;
in vec2 a_texcoord;

out vec3 v_position;
out vec2 v_texcoord;
out mat4 v_camera_matrix;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * vec4(a_position, 1);

    v_camera_matrix = view_matrix;
    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_position = a_position;
    v_texcoord = a_texcoord;
}
