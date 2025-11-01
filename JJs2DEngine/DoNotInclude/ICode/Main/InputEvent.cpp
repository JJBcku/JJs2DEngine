#include "MainIpch.h"
#include "../../../Include/Main/InputEvent.h"

namespace JJs2DEngine
{
	KeyEventData::KeyEventData()
	{
		scanCode = 0;
		keyCode = 0;
		keyMods = 0;

		keyPressed = false;
		keyRepeat = false;
	}

	KeyEventData::~KeyEventData()
	{
	}

	MouseMotionEvent::MouseMotionEvent()
	{
		buttonState = 0;
		positionX = 0;
		positionY = 0;
	}

	MouseMotionEvent::~MouseMotionEvent()
	{
	}

	MouseButtonEvent::MouseButtonEvent()
	{
		mousePositionX = 0;
		mousePositionY = 0;
		buttonIndex = 0;
		buttonPressed = false;
		doubleClick = false;
	}

	MouseButtonEvent::~MouseButtonEvent()
	{
	}

	MouseWheelEvent::MouseWheelEvent()
	{
		scrollX = 0.0f;
		scrollY = 0.0f;
		directionFlipped = false;
	}

	MouseWheelEvent::~MouseWheelEvent()
	{
	}

	InputEvent::InputEvent()
	{
		type = InputEventsType::UNSET;
	}

	InputEvent::InputEvent(const KeyEventData& eventData)
	{
		keyEvent.type = InputEventsType::KEY_EVENT;
		keyEvent.data = eventData;
	}

	InputEvent::InputEvent(const MouseMotionEvent& eventData)
	{
		mouseMotionEvent.type = InputEventsType::MOUSE_MOTION_EVENT;
		mouseMotionEvent.data = eventData;
	}

	InputEvent::InputEvent(const MouseButtonEvent& eventData)
	{
		mouseButtonEvent.type = InputEventsType::MOUSE_BUTTON_EVENT;
		mouseButtonEvent.data = eventData;
	}

	InputEvent::InputEvent(const MouseWheelEvent& eventData)
	{
		mouseWheelEvent.type = InputEventsType::MOUSE_WHEEL_EVENT;
		mouseWheelEvent.data = eventData;
	}

	InputEvent::~InputEvent()
	{
	}

}