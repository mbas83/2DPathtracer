#version 440 core

out vec3 out_color; 

layout(binding = 2) uniform color
{
    vec3 u_color;
};

void main()
{
    out_color = u_color;
}