#pragma once

#include <VulkanSimplified/VSMain/EventHandler/SdlEventHandler.h>

#include <chrono>

namespace JJs2DEngine
{
	class InputDataListInternal
	{
	public:
		InputDataListInternal(std::chrono::high_resolution_clock::time_point currentTime, VS::SdlEventHandler eventHandler);
		~InputDataListInternal();

		void UpdateCurrentTime(std::chrono::high_resolution_clock::time_point currentTime);

	private:
		VS::SdlEventHandler _eventHandler;

		std::chrono::high_resolution_clock::time_point _currentTime;
	};
}
