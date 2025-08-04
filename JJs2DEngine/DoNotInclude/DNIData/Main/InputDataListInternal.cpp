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

	const SpecialKeysDataList& InputDataListInternal::GetSpecialKeyList() const
	{
		return _specialKeys;
	}

	void InputDataListInternal::ResetKeyPresses()
	{
		_specialKeys.ESCkey.ClearKeyPressList();

		_specialKeys.lAltkey.ClearKeyPressList();
		_specialKeys.lCrtlkey.ClearKeyPressList();
		_specialKeys.lShiftkey.ClearKeyPressList();
		_specialKeys.lGuikey.ClearKeyPressList();

		_specialKeys.rAltkey.ClearKeyPressList();
		_specialKeys.rCrtlkey.ClearKeyPressList();
		_specialKeys.rShiftkey.ClearKeyPressList();
		_specialKeys.rGuikey.ClearKeyPressList();
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
			_specialKeys.OnFocusLost();
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
		auto& key = eventData.keysym.sym;

		KeyPressData* specialKey = nullptr;

		switch (key)
		{
		case VS::SDLK_DATA_ESCAPE:
			specialKey = &_specialKeys.ESCkey;
			break;
		case VS::SDLK_DATA_LCTRL:
			specialKey = &_specialKeys.lCrtlkey;
			break;
		case VS::SDLK_DATA_LSHIFT:
			specialKey = &_specialKeys.lShiftkey;
			break;
		case VS::SDLK_DATA_LALT:
			specialKey = &_specialKeys.lAltkey;
			break;
		case VS::SDLK_DATA_LGUI:
			specialKey = &_specialKeys.lGuikey;
			break;
		case VS::SDLK_DATA_RCTRL:
			specialKey = &_specialKeys.rCrtlkey;
			break;
		case VS::SDLK_DATA_RSHIFT:
			specialKey = &_specialKeys.rShiftkey;
			break;
		case VS::SDLK_DATA_RALT:
			specialKey = &_specialKeys.rAltkey;
			break;
		case VS::SDLK_DATA_RGUI:
			specialKey = &_specialKeys.rGuikey;
			break;
		default:
			break;
		}

		if (specialKey != nullptr)
		{
			if (eventData.state > 0)
			{
				specialKey->PressKey(eventData.keysym.mod, _currentTime);
			}
			else
			{
				specialKey->ReleaseKey();
			}
		}

		return true;
	}

	bool InputDataListInternal::HandleKeyboardEventStatic(const VS::SdlKeyboardEventData& eventData, void* pointer)
	{
		return static_cast<InputDataListInternal*>(pointer)->HandleKeyboardEvent(eventData);
	}

}