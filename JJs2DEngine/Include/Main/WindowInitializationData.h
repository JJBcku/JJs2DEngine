#pragma once

#include "AspectRatioDef.h"

namespace JJs2DEngine
{
	struct WindowInitializationData
	{
		std::string windowTitle;
		uint32_t windowWidth;
		AspectRatio aspectRatio;
		bool fullscreenWindow;

		WindowInitializationData();
	};
}