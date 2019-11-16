#version 430 core

//Gooch

out vec4 color;

in vec3 N;
in vec3 V;
in vec3 L;


// Material properties
uniform vec3 kd; //Color of the object containing (rc, gc, bc)

uniform vec3 k_blue; //k_blue
uniform vec3 k_yellow; //k_yellow

uniform float alpha; //alpha
uniform float beta; //beta

void main(void)
{
    vec3 k_cool = k_blue + alpha * kd;
    vec3 k_warm = k_yellow + beta * kd;

    vec3 c1 = ((1 - dot(N, L))/2)*k_cool;
    vec3 c2 = (1 - (1 - dot(N, L))/2)*k_warm;

    color = vec4(normalize(c1+c2), 1.0);
}

