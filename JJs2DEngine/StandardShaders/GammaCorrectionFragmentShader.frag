#version 450

layout (location = 0) out vec4 outColor;

layout (input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput inColor;

layout (push_constant) uniform GammaData
{
	float gammaValue;
} gData;

void main()
{
	vec4 color = subpassLoad(inColor);
	vec3 transitionColor = vec3(color.rgb);
	transitionColor = (vec3(1.) * pow(transitionColor, vec3(1.0/gData.gammaValue)));
	outColor = vec4(transitionColor, color.a);
}