#version 150

in vec2 v_texcoord;
in vec3 v_position;
in mat4 v_camera_matrix;

//VALUES
const float EPSILON = 0.000001;
const float INFINITY = 67108864; //2^26

//LIGHTING
const vec3 SUNDIR = vec3(-0.234, -0.801, -0.567);
const float FOGINTENSITY = 0.002;
const vec3 AMBIENTCOLOR = vec3(.5, .8, .9);
const float AMBIENTFORCE = 0.3;
const vec3 SUNCOLOR = vec3(1, 0.97, 0.75);
const float SUNFORCE = 3;



uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 inv_projection_matrix;
uniform mat4 inv_view_matrix;
uniform mat4 inv_model_matrix;

uniform vec2 screen_ratios;
uniform vec2 screen_size;


vec3 applyFog( in vec3  rgb,      // original color of the pixel
               in float distance, // camera to point distance
               in vec3  rayDir,   // camera to point vector
               in vec3  sunDir )  // sun light direction
{
    float fogAmount = 1.0 - exp( -distance * FOGINTENSITY );
    float sunAmount = max(dot( rayDir, sunDir ), 0.0 );
    vec3  fogColor  = mix(AMBIENTCOLOR, // bluish
                          SUNCOLOR * 4, // yellowish
                           pow(sunAmount, 16.0) );
    return mix( rgb, fogColor, fogAmount );
}



void main () {
    vec3 Color = vec3(1, 0, 0);

    vec3 D = vec3(-(((gl_FragCoord.x - 0.5) / (screen_size.x - 1)) - 0.5) * screen_ratios.x, -(((gl_FragCoord.y - 0.5) / (screen_size.y - 1)) - 0.5) * screen_ratios.y, 1);
    D = normalize(D);
    D = (camera_matrix * vec4(D, 0)).xyz;

    vec3 O = vec3(0, 0, 0);
    O = -(inv_model_matrix * camera_matrix * vec4(O, 1)).xyz;

    gl_FragDepth = 1 - EPSILON;
    Color = applyFog(Color, INFINITY, D, SUNDIR);

    gl_FragColor = vec4(tanh(Color), 1);
}

