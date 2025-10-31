#include "MainDNIpch.h"
#include "InputDataListInternal.h"

#include <VulkanSimplified/VSMain/EventHandler/SdlWindowEventData.h>
#include <VulkanSimplified/VSMain/EventHandler/SdlKeyboardEventsData.h>

namespace JJs2DEngine
{
	InputDataListInternal::InputDataListInternal(std::chrono::high_resolution_clock::time_point currentTime, VS::SdlEventHandler eventHandler) : _eventHandler(eventHandler),
		_currentTime(currentTime)
	{
		_eventList.reserve(0x100);
		_focusLost = false;

		RegisterWindowEventHandler();
		RegisterKeyboardEventHandler();
	}

	InputDataListInternal::~InputDataListInternal()
	{
		UnregisterWindowEventHandler();
		UnregisterKeyboardEventHandler();
	}

	void InputDataListInternal::UpdateCurrentTime(std::chrono::high_resolution_clock::time_point currentTime)
	{
		_currentTime = currentTime;
	}

	const std::vector<KeyPressData>& InputDataListInternal::GetEventList() const
	{
		return _eventList;
	}

	void InputDataListInternal::ClearEventList()
	{
		_eventList.clear();
	}

	bool InputDataListInternal::WasFocusLost() const
	{
		return _focusLost;
	}

	void InputDataListInternal::ResetFocusLost()
	{
		_focusLost = false;
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
			_focusLost = true;
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
		KeyPressData keyAdd;

		keyAdd.scanCode = eventData.keysym.scancode;
		keyAdd.keyCode = eventData.keysym.sym;
		keyAdd.keyMods = eventData.keysym.mod;
		keyAdd.keyPressed = eventData.state > 0;
		keyAdd.keyRepeat = eventData.repeat > 0;

		if (_eventList.size() == _eventList.capacity())
			_eventList.reserve(_eventList.capacity() << 1);

		_eventList.push_back(keyAdd);

		return true;
	}

	bool InputDataListInternal::HandleKeyboardEventStatic(const VS::SdlKeyboardEventData& eventData, void* pointer)
	{
		return static_cast<InputDataListInternal*>(pointer)->HandleKeyboardEvent(eventData);
	}

}