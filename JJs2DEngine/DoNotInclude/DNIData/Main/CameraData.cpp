#include "MainDNIpch.h"
#include "CameraData.h"

namespace JJs2DEngine
{
	CameraData::CameraData()
	{
		rotation = glm::mat4(1.0f);
		position = glm::vec2(1.0f);
		zoom = 1.0f;
	}

	CameraData::~CameraData()
	{
	}

}