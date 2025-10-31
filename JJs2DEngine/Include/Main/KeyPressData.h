#pragma once

#include <Miscellaneous/Bool64Def.h>

#include <stdint.h>
#include <optional>
#include <chrono>
#include <compare>

namespace JJs2DEngine
{
	struct KeyPressData
	{
		uint32_t scanCode;
		uint32_t keyCode;
		uint32_t keyMods;

		bool keyPressed;
		bool keyRepeat;

		KeyPressData();
		~KeyPressData();
	};
}
