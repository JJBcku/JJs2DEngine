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
		uint8_t padding1;
		uint16_t padding2;
		uint32_t padding3;

		WindowInitializationData();
	};
}