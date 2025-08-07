#version 450

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec2 inTexSize;
layout(location = 2) in uint inTexLayer;
layout(location = 3) in uint inTexIndex;
layout(location = 4) in vec4 inSize;

layout(location = 0) out vec2 outTexCoord;
layout(location = 1) out uint outTexLayer;
layout(location = 2) out uint outTexIndex;

vec2 vertexData[4] = vec2[](
	vec2( -1.0, -1.0 ),
	vec2( -1.0,  1.0 ),
	vec2(  1.0, -1.0 ),
	vec2(  1.0,  1.0 )
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
	vec2 verticePos = vertexData[vertexIndexes[gl_VertexIndex]];
	vec2 texPos = textureData[vertexIndexes[gl_VertexIndex]];

	gl_Position = vec4(verticePos * inSize.xy, inSize.z, 1.0);
	
	vec2 texCoord = inTexCoord + (inTexSize * texPos);
	
	outTexCoord = texCoord;
	outTexLayer = inTexLayer;
	outTexIndex = inTexIndex;
}