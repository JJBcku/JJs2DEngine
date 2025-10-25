#pragma once

#include <glm\mat4x4.hpp>
#include <glm\vec2.hpp>

namespace JJs2DEngine
{
	struct CameraData
	{
		glm::mat4 rotation;
		glm::vec2 position;
		float zoom;
		float aspectRatio;

		CameraData();
		~CameraData();
	};
}