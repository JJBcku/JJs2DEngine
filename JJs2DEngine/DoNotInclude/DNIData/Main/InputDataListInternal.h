#pragma once

#include <VulkanSimplified/VSMain/EventHandler/SdlEventHandler.h>
#include <CustomLists/IDObject.h>

#include <chrono>
#include <utility>

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

		IDObject<std::pair<VS::WindowEventFunction, void*>> _windowEventID;
		IDObject<std::pair<VS::KeyboardEventFunction, void*>> _keyboardEventID;

		void RegisterWindowEvent();
		void UnregisterWindowEvent();

		bool HandleWindowEvent(const VS::SdlWindowEventData& eventData);
		static bool HandleWindowEventStatic(const VS::SdlWindowEventData& eventData, void* pointer);

		void RegisterKeyboardEvent();
		void UnregisterKeyboardEvent();

		bool HandleKeyboardEvent(const VS::SdlKeyboardEventData& eventData);
		static bool HandleKeyboardEventStatic(const VS::SdlKeyboardEventData& eventData, void* pointer);
	};
}
