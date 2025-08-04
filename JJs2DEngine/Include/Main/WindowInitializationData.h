#pragma once

#include "AspectRatioDef.h"

#include <Miscellaneous/Bool64Def.h>

namespace JJs2DEngine
{
	struct WindowInitializationData
	{
		std::string windowTitle;
		uint32_t windowWidth;
		AspectRatio aspectRatio;
		Misc::Bool64Values fullscreenWindow;

		WindowInitializationData();
	};
}