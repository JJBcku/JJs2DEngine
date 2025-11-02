#version 450

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec2 inTexSize;
layout(location = 2) in vec4 inPos;
layout(location = 3) in vec2 inSize;
layout(location = 4) in uint inTexLayer;
layout(location = 5) in uint inTexIndex;

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

layout (push_constant) uniform CameraData
{
	mat4 perspectiveRotation;
	vec2 cameraPosition;
	float zoom;
	float aspectRatio;
} camera;

void main()
{
	vec2 verticeOffset = vertexData[vertexIndexes[gl_VertexIndex]] * inSize * vec2(camera.zoom);
	vec2 verticePosition = verticeOffset + inPos.xy;
	vec4 worldPosition = vec4(verticePosition.x, verticePosition.y * camera.aspectRatio, inPos.z, 1.0) * camera.perspectiveRotation;
	vec4 inCameraPosition = vec4(worldPosition.xy - camera.cameraPosition, inPos.z, 1.0);

	gl_Position = vec4(inCameraPosition.x, inCameraPosition.y, inPos.z, 1.0);
	
	vec2 texPos = textureData[vertexIndexes[gl_VertexIndex]];
	vec2 texCoord = inTexCoord + (inTexSize * texPos);
	
	outTexCoord = texCoord;
	outTexLayer = inTexLayer;
	outTexIndex = inTexIndex;
}