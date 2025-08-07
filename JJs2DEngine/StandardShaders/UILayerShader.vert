#version 450

layout(location = 0) in vec4 inTexCoord;
layout(location = 1) in vec3 inSize;

layout(location = 0) out vec4 outTexCoord;

vec4 vertexData[4] = vec4[](
	vec4( -1.0, -1.0, 0.0, 1.0 ),
	vec4( -1.0,  1.0, 0.0, 1.0 ),
	vec4(  1.0, -1.0, 0.0, 1.0 ),
	vec4(  1.0,  1.0, 0.0, 1.0 )
);

vec2 textureData[4] = vec2[](
	vec2(0, 0),
	vec2(0, 1),
	vec2(1, 0),
	vec2(1, 1)
);

uint vertexIndexes[6] = uint[]( 0, 1, 2, 2, 1, 3);

void main()
{
	vec4 verticePos = vertexData[vertexIndexes[gl_VertexIndex]];
	vec2 texPos = textureData[vertexIndexes[gl_VertexIndex]];

	gl_Position = vec4(verticePos.xy * inSize.xy, inSize.z, verticePos.w);
	outTexCoord = vec4(inTexCoord.xy * texPos, inTexCoord.zw);
}