#version 450

vec4 vertexData[4] = vec4[](
	vec4( -1.0, -1.0, 0.0, 1.0 ),
	vec4( -1.0,  1.0, 0.0, 1.0 ),
	vec4(  1.0, -1.0, 0.0, 1.0 ),
	vec4(  1.0,  1.0, 0.0, 1.0 )
);

uint vertexIndexes[6] = uint[]( 0, 1, 2, 2, 1, 3);

void main()
{
	gl_Position = vertexData[vertexIndexes[gl_VertexIndex]];
}