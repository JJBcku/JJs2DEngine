#include "MainDNIpch.h"
#include "InputDataListInternal.h"

#include <VulkanSimplified/VSMain/EventHandler/SdlWindowEventData.h>
#include <VulkanSimplified/VSMain/EventHandler/SdlKeyboardEventsData.h>

namespace JJs2DEngine
{
	InputDataListInternal::InputDataListInternal(std::chrono::high_resolution_clock::time_point currentTime, VS::SdlEventHandler eventHandler) : _eventHandler(eventHandler),
		_currentTime(currentTime)
	{
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

	const KeyPressData& InputDataListInternal::GetKeyPressData(size_t scanCode) const
	{
		if (scanCode >= _keyList.size())
			throw std::runtime_error("InputDataListInternal::GetKeyPressData Error: Erroneous scan code!");

		return _keyList[scanCode];
	}

	void InputDataListInternal::ClearKeyPressesLists()
	{
		for (size_t i = 0; i < _keyList.size(); ++i)
		{
			_keyList[i].ClearKeyPressList();
		}
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
			for (size_t i = 0; i < _keyList.size(); ++i)
				_keyList[i].OnFocusLost();
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
		auto& key = eventData.keysym.scancode;

		if (key >= _keyList.size() || key < 0)
			throw std::runtime_error("InputDataListInternal::HandleKeyboardEvent Error: Event has an erroneous key scan code value!");

		if (eventData.state > 0)
		{
			_keyList[key].PressKey(eventData.keysym.mod, _currentTime);
		}
		else
		{
			_keyList[key].ReleaseKey();
		}

		return true;
	}

	bool InputDataListInternal::HandleKeyboardEventStatic(const VS::SdlKeyboardEventData& eventData, void* pointer)
	{
		return static_cast<InputDataListInternal*>(pointer)->HandleKeyboardEvent(eventData);
	}

}