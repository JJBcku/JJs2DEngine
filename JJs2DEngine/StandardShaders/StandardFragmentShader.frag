#version 450

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) flat in uint inTexLayer;
layout(location = 2) flat in uint inTexIndex;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2DArray texSampler[28];

void main()
{
	vec3 texCoords = vec3(inTexCoord, inTexLayer);
	outColor = texture(texSampler[inTexIndex], texCoords);
}