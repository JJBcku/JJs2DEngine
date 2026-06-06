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

		padding = 0;
	}

	MouseMotionEvent::MouseMotionEvent()
	{
		buttonState = 0;
		positionX = 0.0f;
		positionY = 0.0f;
	}

	MouseButtonEvent::MouseButtonEvent()
	{
		mousePositionX = 0.0f;
		mousePositionY = 0.0f;
		buttonIndex = 0;
		buttonPressed = false;
		doubleClick = false;
		padding = false;
	}

	MouseWheelEvent::MouseWheelEvent()
	{
		scrollX = 0.0f;
		scrollY = 0.0f;
		directionFlipped = false;

		padding1 = 0;
		padding2 = 0;
	}

	InputEvent::InputEvent() noexcept
	{
		type = InputEventsType::UNSET;
	}

	InputEvent::InputEvent(const KeyEventData& eventData) noexcept : keyEvent(eventData)
	{
	}

	InputEvent::InputEvent(const MouseMotionEvent& eventData) noexcept : mouseMotionEvent(eventData)
	{
	}

	InputEvent::InputEvent(const MouseButtonEvent& eventData) noexcept : mouseButtonEvent(eventData)
	{
	}

	InputEvent::InputEvent(const MouseWheelEvent& eventData) noexcept : mouseWheelEvent(eventData)
	{
	}

	InputEvent::KeyEvent::KeyEvent(const KeyEventData& eventData) noexcept : data(eventData)
	{
		type = InputEventsType::KEY_EVENT;
	}

	InputEvent::MouseMoveEvent::MouseMoveEvent(const MouseMotionEvent& eventData) noexcept : data(eventData)
	{
		type = InputEventsType::MOUSE_MOTION_EVENT;
	}

	InputEvent::MouseButtonPress::MouseButtonPress(const MouseButtonEvent& eventData) noexcept : data(eventData)
	{
		type = InputEventsType::MOUSE_BUTTON_EVENT;
	}

	InputEvent::MouseScrollEvent::MouseScrollEvent(const MouseWheelEvent& eventData) noexcept : data(eventData)
	{
		type = InputEventsType::MOUSE_WHEEL_EVENT;
	}

}