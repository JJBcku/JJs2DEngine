#pragma once

#include <VulkanSimplified/VSMain/EventHandler/SdlEventHandler.h>
#include <CustomLists/IDObject.h>

#include <chrono>
#include <array>
#include <utility>

namespace JJs2DEngine
{
	union InputEvent;

	class InputDataListInternal
	{
	public:
		InputDataListInternal(std::chrono::high_resolution_clock::time_point currentTime, VS::SdlEventHandler eventHandler);
		~InputDataListInternal();

		void UpdateCurrentTime(std::chrono::high_resolution_clock::time_point currentTime);

		const std::vector<InputEvent>& GetEventList() const;
		void ClearEventList();

		bool FocusWasLost() const;
		void ResetFocusLost();

	private:
		VS::SdlEventHandler _eventHandler;

		std::chrono::high_resolution_clock::time_point _currentTime;

		IDObject<std::pair<VS::WindowEventFunction, void*>> _windowEventHandlerID;
		IDObject<std::pair<VS::KeyboardEventFunction, void*>> _keyboardEventHandlerID;
		IDObject<std::pair<VS::MouseMotionEventFunction, void*>> _mouseMotionEventHandlerID;
		IDObject<std::pair<VS::MouseButtonEventFunction, void*>> _mouseButtonEventHandlerID;
		IDObject<std::pair<VS::MouseWheelEventFunction, void*>> _mouseWheelEventHandlerID;

		std::vector<InputEvent> _eventList;

		bool _focusLost;

		void RegisterWindowEventHandler();
		void UnregisterWindowEventHandler();

		bool HandleWindowEvent(const VS::SdlWindowEventData& eventData);
		static bool HandleWindowEventStatic(const VS::SdlWindowEventData& eventData, void* pointer);

		void RegisterKeyboardEventHandler();
		void UnregisterKeyboardEventHandler();

		bool HandleKeyboardEvent(const VS::SdlKeyboardEventData& eventData);
		static bool HandleKeyboardEventStatic(const VS::SdlKeyboardEventData& eventData, void* pointer);

		void RegisterMouseMotionEventHandler();
		void UnregisterMouseMotionEventHandler();

		bool HandleMouseMotionEvent(const VS::SdlMouseMotionEventData& eventData);
		static bool HandleMouseMotionEventStatic(const VS::SdlMouseMotionEventData& eventData, void* pointer);

		void RegisterMouseButtonEventHandler();
		void UnregisterMouseButtonEventHandler();

		bool HandleMouseButtonEvent(const VS::SdlMouseButtonEventData& eventData);
		static bool HandleMouseButtonEventStatic(const VS::SdlMouseButtonEventData& eventData, void* pointer);

		void RegisterMouseWheelEventHandler();
		void UnregisterMouseWheelEventHandler();

		bool HandleMouseWheelEvent(const VS::SdlMouseWheelEventData& eventData);
		static bool HandleMouseWheelEventStatic(const VS::SdlMouseWheelEventData& eventData, void* pointer);
	};
}
