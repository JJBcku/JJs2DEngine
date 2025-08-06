#version 450

layout(location = 0) in vec4 inTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2DArray texSampler[28];

void main()
{
	outColor = texture(texSampler[uint(floor(inTexCoord.w))], inTexCoord.xyz);
}