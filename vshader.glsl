#version 150

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

uniform mat4 mvp_matrix;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texcoord;

out vec3 v_position;
out vec3 v_normal;
out vec3 v_world_normal;
out vec2 v_texcoord;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * vec4(a_position, 1);

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_position = a_position;
    v_normal = a_normal;
    v_texcoord = a_texcoord;

}
