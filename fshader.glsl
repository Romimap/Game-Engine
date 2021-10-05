#version 150

uniform sampler2D texture;

in vec2 v_texcoord;
in vec3 v_position;
//! [0]
void main()
{
    // Set fragment color from texture
    gl_FragColor = texture2D(texture, v_texcoord);
}
//! [0]

