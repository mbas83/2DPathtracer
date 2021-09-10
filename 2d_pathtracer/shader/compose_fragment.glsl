#version 440 core

//RGB32F texture from path tracing (divide this by N)
layout (binding = 0) uniform sampler2D sampleTexture;


layout(binding = 1) uniform ComposeUniform
{
    //number of samples
    int N;
    //exposure to scale brightness
    float exposure;
};

out vec3 out_color; 

void main()
{
    ivec2 center = ivec2(gl_FragCoord.xy);
	vec3 texel = texelFetch(sampleTexture, center, 0).rgb;

    //multiply with exposure before gamma correction
    texel *= exposure;

	out_color = pow( texel / vec3(N),  vec3(1.0/2.2) );
}