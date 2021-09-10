#version 440 core

layout(location = 0) in vec2 in_position;

layout(binding = 1) uniform transform
{
    vec2 u_scene_size;
};


void main()
{
    gl_PointSize = 5.0f;

    //transform to [-1,1]
    float x = (in_position.x / u_scene_size.x) * 2.0f - 1.0f;
    float y = (in_position.y / u_scene_size.y) * 2.0f - 1.0f;
    gl_Position = vec4(x,y, 0.0, 1.0);
}