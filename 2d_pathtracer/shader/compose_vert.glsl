#version 440 core


void main()
{
    gl_Position.xy = vec2(float(gl_VertexID & 1) * 4.0 - 1.0,
							  float(gl_VertexID / 2) *-4.0 + 1.0);
	gl_Position.zw = vec2(1.0, 1.0);
}