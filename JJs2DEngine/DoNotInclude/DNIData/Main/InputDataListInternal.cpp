#include "MainDNIpch.h"
#include "InputDataListInternal.h"

#include "../../../Include/Main/InputEvent.h"

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
		RegisterMouseMotionEventHandler();
		RegisterMouseButtonEventHandler();
		RegisterMouseWheelEventHandler();
	}

	InputDataListInternal::~InputDataListInternal()
	{
		UnregisterWindowEventHandler();
		UnregisterKeyboardEventHandler();
		UnregisterMouseMotionEventHandler();
		UnregisterMouseButtonEventHandler();
		UnregisterMouseWheelEventHandler();
	}

	void InputDataListInternal::UpdateCurrentTime(std::chrono::high_resolution_clock::time_point currentTime)
	{
		_currentTime = currentTime;
	}

	const std::vector<InputEvent>& InputDataListInternal::GetEventList() const
	{
		return _eventList;
	}

	void InputDataListInternal::ClearEventList()
	{
		_eventList.clear();
	}

	bool InputDataListInternal::FocusWasLost() const
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
		KeyEventData keyAdd;

		keyAdd.scanCode = eventData.keysym.scancode;
		keyAdd.keyCode = eventData.keysym.sym;
		keyAdd.keyMods = eventData.keysym.mod;
		keyAdd.keyPressed = eventData.state > 0;
		keyAdd.keyRepeat = eventData.repeat > 0;

		if (_eventList.size() == _eventList.capacity())
			_eventList.reserve(_eventList.capacity() << 1);

		_eventList.emplace_back(keyAdd);

		return true;
	}

	bool InputDataListInternal::HandleKeyboardEventStatic(const VS::SdlKeyboardEventData& eventData, void* pointer)
	{
		return static_cast<InputDataListInternal*>(pointer)->HandleKeyboardEvent(eventData);
	}

	void InputDataListInternal::RegisterMouseMotionEventHandler()
	{
		_mouseMotionEventHandlerID = _eventHandler.RegisterMouseMotionEventCallback(HandleMouseMotionEventStatic, this, 0x10);
	}

	void InputDataListInternal::UnregisterMouseMotionEventHandler()
	{
		_eventHandler.UnRegisterMouseMotionEventCallback(_mouseMotionEventHandlerID, true);
	}

	bool InputDataListInternal::HandleMouseMotionEvent(const VS::SdlMouseMotionEventData& eventData)
	{
		MouseMotionEvent mouseAdd;
		mouseAdd.positionX = eventData.x;
		mouseAdd.positionY = eventData.y;
		mouseAdd.buttonState = eventData.state;

		if (_eventList.size() == _eventList.capacity())
			_eventList.reserve(_eventList.capacity() << 1);

		_eventList.emplace_back(mouseAdd);

		return true;
	}

	bool InputDataListInternal::HandleMouseMotionEventStatic(const VS::SdlMouseMotionEventData& eventData, void* pointer)
	{
		return static_cast<InputDataListInternal*>(pointer)->HandleMouseMotionEvent(eventData);
	}

	void InputDataListInternal::RegisterMouseButtonEventHandler()
	{
		_mouseButtonEventHandlerID = _eventHandler.RegisterMouseButtonEventCallback(HandleMouseButtonEventStatic, this, 0x10);
	}

	void InputDataListInternal::UnregisterMouseButtonEventHandler()
	{
		_eventHandler.UnRegisterMouseButtonEventCallback(_mouseButtonEventHandlerID, true);
	}

	bool InputDataListInternal::HandleMouseButtonEvent(const VS::SdlMouseButtonEventData& eventData)
	{
		MouseButtonEvent mouseAdd;
		mouseAdd.mousePositionX = eventData.x;
		mouseAdd.mousePositionY = eventData.y;
		mouseAdd.buttonIndex = eventData.button;
		mouseAdd.buttonPressed = eventData.state > 0;
		mouseAdd.doubleClick = eventData.clicks > 1;

		if (_eventList.size() == _eventList.capacity())
			_eventList.reserve(_eventList.capacity() << 1);

		_eventList.emplace_back(mouseAdd);

		return true;
	}

	bool InputDataListInternal::HandleMouseButtonEventStatic(const VS::SdlMouseButtonEventData& eventData, void* pointer)
	{
		return static_cast<InputDataListInternal*>(pointer)->HandleMouseButtonEvent(eventData);
	}

	void InputDataListInternal::RegisterMouseWheelEventHandler()
	{
		_mouseWheelEventHandlerID = _eventHandler.RegisterMouseWheelEventCallback(HandleMouseWheelEventStatic, this, 0x10);
	}

	void InputDataListInternal::UnregisterMouseWheelEventHandler()
	{
		_eventHandler.UnRegisterMouseWheelEventCallback(_mouseWheelEventHandlerID, true);
	}

	bool InputDataListInternal::HandleMouseWheelEvent(const VS::SdlMouseWheelEventData& eventData)
	{
		MouseWheelEvent mouseAdd;
		mouseAdd.scrollX = eventData.preciseX;
		mouseAdd.scrollY = eventData.preciseY;
		mouseAdd.directionFlipped = eventData.direction > 0;

		if (_eventList.size() == _eventList.capacity())
			_eventList.reserve(_eventList.capacity() << 1);

		_eventList.emplace_back(mouseAdd);

		return true;
	}

	bool InputDataListInternal::HandleMouseWheelEventStatic(const VS::SdlMouseWheelEventData& eventData, void* pointer)
	{
		return static_cast<InputDataListInternal*>(pointer)->HandleMouseWheelEvent(eventData);
	}

}