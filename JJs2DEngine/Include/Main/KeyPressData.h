#pragma once

#include <stdint.h>
#include <optional>
#include <chrono>

namespace JJs2DEngine
{
	struct KeyPressData
	{
		uint64_t keyPressCount;
		std::optional<std::chrono::high_resolution_clock::time_point> currentKeyPressBeginning;

		KeyPressData();
		~KeyPressData();
	};
}
