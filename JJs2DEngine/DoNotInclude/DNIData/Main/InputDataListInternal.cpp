#include "MainDNIpch.h"
#include "InputDataListInternal.h"

#include <VulkanSimplified/VSMain/EventHandler/SdlWindowEventData.h>

namespace JJs2DEngine
{
	InputDataListInternal::InputDataListInternal(std::chrono::high_resolution_clock::time_point currentTime, VS::SdlEventHandler eventHandler) : _eventHandler(eventHandler),
		_currentTime(currentTime)
	{
		RegisterWindowEventHandler();
	}

	InputDataListInternal::~InputDataListInternal()
	{
		UnregisterWindowEventHandler();
	}

	void InputDataListInternal::UpdateCurrentTime(std::chrono::high_resolution_clock::time_point currentTime)
	{
		_currentTime = currentTime;
	}

	void InputDataListInternal::RegisterWindowEventHandler()
	{
		_windowEventHandlerID = _eventHandler.RegisterWindowEventCallback(HandleWindowEventStatic, this, 0x10);
	}

	void InputDataListInternal::UnregisterWindowEventHandler()
	{
		_eventHandler.UnRegisterWindowEventCallback(_windowEventHandlerID, true);
	}

	bool InputDataListInternal::HandleWindowEvent(const VS::SdlWindowEventData& eventData)
	{
		if (eventData.event == VS::SDL_DATA_WINDOWEVENT_FOCUS_LOST)
		{
			return false;
		}

		return true;
	}

	bool InputDataListInternal::HandleWindowEventStatic(const VS::SdlWindowEventData& eventData, void* pointer)
	{
		return static_cast<InputDataListInternal*>(pointer)->HandleWindowEvent(eventData);
	}

	void InputDataListInternal::RegisterKeyboardEventHandler()
	{
		_keyboardEventHandlerID = _eventHandler.RegisterKeyboardEventCallback(HandleKeyboardEventStatic, this, 0x10);
	}

	void InputDataListInternal::UnregisterKeyboardEventHandler()
	{
		_eventHandler.UnRegisterKeyboardEventCallback(_keyboardEventHandlerID, true);
	}

	bool InputDataListInternal::HandleKeyboardEvent(const VS::SdlKeyboardEventData& eventData)
	{
		return true;
	}

	bool InputDataListInternal::HandleKeyboardEventStatic(const VS::SdlKeyboardEventData& eventData, void* pointer)
	{
		return static_cast<InputDataListInternal*>(pointer)->HandleKeyboardEvent(eventData);
	}

}