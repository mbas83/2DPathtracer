#version 440 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec3 color;

layout(location = 0) flat out vec2 start_position;
layout(location = 1) out vec2 position;
layout(location = 2) out vec3 ray_start_flux;

layout(binding = 1) uniform transform
{
    vec2 u_scene_size;
};

void main()
{
    //set position of start vertex in object space
    start_position = in_position;
    position = in_position;

    //set color
    ray_start_flux = color; 

    //transform to [-1,1]
    float x = (in_position.x / u_scene_size.x) * 2.0f - 1.0f;
    float y = (in_position.y / u_scene_size.y) * 2.0f - 1.0f;
    gl_Position = vec4(x,y, 0.0, 1.0);
}