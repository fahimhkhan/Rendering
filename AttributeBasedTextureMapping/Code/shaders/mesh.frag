#version 430 core

out vec4 color;

in vec3 N;
in vec3 V;
in vec3 L;
in vec2 UV;

uniform sampler2D image;

uniform float r;


void main(void)
{
    float s = max(dot(N, L), 0.0);
    float t = max((pow(dot(N, V),r)), 0.0);
    //float s = dot(N, L);
    //float t = (pow(dot(N, V),r));
    vec2 ST = vec2(s,t);
    vec4 imageTex = texture2D(image, ST);

    color = imageTex;
}
