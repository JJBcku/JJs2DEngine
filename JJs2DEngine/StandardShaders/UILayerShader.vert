#version 450

layout(location = 0) in vec2 inSize;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 outTexCoord;

vec4 vertexData[4] = vec4[](
	vec4( -1.0, -1.0, 0.0, 1.0 ),
	vec4( -1.0,  1.0, 0.0, 1.0 ),
	vec4(  1.0, -1.0, 0.0, 1.0 ),
	vec4(  1.0,  1.0, 0.0, 1.0 )
);

uint vertexIndexes[6] = uint[]( 0, 1, 2, 2, 1, 3);

void main()
{
	vec4 verticePos = vertexData[vertexIndexes[gl_VertexIndex]];

	gl_Position = vec4(verticePos.xy * inSize, verticePos.zw);
	outTexCoord = inTexCoord;
}