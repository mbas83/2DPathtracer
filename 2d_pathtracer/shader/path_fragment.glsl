#version 440 core

out vec3 out_color; 

layout(location = 0) flat in vec2 start_position;
layout(location = 1) in vec2 position;
layout(location = 2) in vec3 ray_start_flux;


void main()
{
    //distance between start_position and current pos
    float d = distance(start_position,position);
    
    //d/=3;
    d = max(d,1.0f);

    out_color = ray_start_flux / d;
}