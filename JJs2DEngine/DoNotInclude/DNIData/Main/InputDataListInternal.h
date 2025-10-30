#pragma once

#include "../../../Include/Common/SdlScancode.h"

#include "../../../Include/Main/KeyPressData.h"

#include <VulkanSimplified/VSMain/EventHandler/SdlEventHandler.h>
#include <CustomLists/IDObject.h>

#include <chrono>
#include <array>
#include <utility>

namespace JJs2DEngine
{
	class InputDataListInternal
	{
	public:
		InputDataListInternal(std::chrono::high_resolution_clock::time_point currentTime, VS::SdlEventHandler eventHandler);
		~InputDataListInternal();

		void UpdateCurrentTime(std::chrono::high_resolution_clock::time_point currentTime);

		const KeyPressData& GetKeyPressData(size_t scanCode) const;

		void ClearKeyPressesLists();

	private:
		VS::SdlEventHandler _eventHandler;

		std::chrono::high_resolution_clock::time_point _currentTime;

		IDObject<std::pair<VS::WindowEventFunction, void*>> _windowEventHandlerID;
		IDObject<std::pair<VS::KeyboardEventFunction, void*>> _keyboardEventHandlerID;

		std::array<KeyPressData, SdlScancode::SDL_DATA_NUM_SCANCODES> _keyList;

		void RegisterWindowEventHandler();
		void UnregisterWindowEventHandler();

		bool HandleWindowEvent(const VS::SdlWindowEventData& eventData);
		static bool HandleWindowEventStatic(const VS::SdlWindowEventData& eventData, void* pointer);

		void RegisterKeyboardEventHandler();
		void UnregisterKeyboardEventHandler();

		bool HandleKeyboardEvent(const VS::SdlKeyboardEventData& eventData);
		static bool HandleKeyboardEventStatic(const VS::SdlKeyboardEventData& eventData, void* pointer);
	};
}
