#pragma once

#include "AspectRatioDef.h"

#include <Miscellaneous/Bool64Def.h>

#include <compare>

namespace JJs2DEngine
{
	struct WindowInitializationData
	{
		std::string windowTitle;
		uint32_t windowWidth;
		AspectRatio aspectRatio;
		Misc::Bool64 fullscreenWindow;

		WindowInitializationData();

		std::strong_ordering operator<=>(const WindowInitializationData& rhs) const noexcept = default;
		bool operator==(const WindowInitializationData& rhs) const noexcept = default;
	};
}