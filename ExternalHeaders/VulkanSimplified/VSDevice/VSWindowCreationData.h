#pragma once

#include <stdint.h>
#include <string>

namespace VulkanSimplified
{
	enum class WindowSettings : uint64_t
	{
		STANDARD = 0x0,
		RESIZABLE = 0x1,
		BORDERLESS = 0x2,
		FULLSCREEN_NONEXCLUSIVE = 0x3,
	};

	struct WindowCreationData
	{
		std::string windowTitle;
		uint32_t width, height;
		WindowSettings settings;

		WindowCreationData();
	};
}
