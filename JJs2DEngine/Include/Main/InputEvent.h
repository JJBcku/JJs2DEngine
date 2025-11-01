#pragma once

#include <VulkanSimplified/VSMain/EventHandler/SdlKeyboardEventsData.h>
#include <VulkanSimplified/VSMain/EventHandler/SdlMouseEventsData.h>

namespace JJs2DEngine
{
	typedef VulkanSimplified::SdlKeycode SdlKeycode;
	typedef VulkanSimplified::SdlModuleKeymod SdlKeymod;
	typedef VulkanSimplified::SdlScancode SdlScancode;

	typedef VulkanSimplified::SdlMouseButtonIndex SdlMouseButtonIndex;
	typedef VulkanSimplified::SdlMouseButtonState SdlMouseButtonState;

	enum class InputEventsType
	{
		UNSET = 0,
		KEY_EVENT = 1,
		MOUSE_MOTION_EVENT = 2,
		MOUSE_BUTTON_EVENT = 3,
		MOUSE_WHEEL_EVENT = 4,
	};

	struct KeyEventData
	{
		uint32_t scanCode;
		uint32_t keyCode;
		uint32_t keyMods;

		bool keyPressed;
		bool keyRepeat;

		KeyEventData();
		~KeyEventData();
	};

	struct MouseMotionEvent
	{
		uint32_t buttonState;
		int32_t positionX;
		int32_t positionY;

		MouseMotionEvent();
		~MouseMotionEvent();
	};

	struct MouseButtonEvent
	{
		uint32_t mousePositionX;
		uint32_t mousePositionY;
		uint8_t buttonIndex;
		bool buttonPressed;
		bool doubleClick;

		MouseButtonEvent();
		~MouseButtonEvent();
	};

	struct MouseWheelEvent
	{
		float scrollX;
		float scrollY;
		bool directionFlipped;

		MouseWheelEvent();
		~MouseWheelEvent();
	};

	union InputEvent
	{
		InputEventsType type;

		struct
		{
			InputEventsType type;
			KeyEventData data;
		} keyEvent;

		struct
		{
			InputEventsType type;
			MouseMotionEvent data;
		} mouseMotionEvent;

		struct
		{
			InputEventsType type;
			MouseButtonEvent data;
		} mouseButtonEvent;

		struct
		{
			InputEventsType type;
			MouseWheelEvent data;
		} mouseWheelEvent;

		InputEvent();
		InputEvent(const KeyEventData& eventData);
		InputEvent(const MouseMotionEvent& eventData);
		InputEvent(const MouseButtonEvent& eventData);
		InputEvent(const MouseWheelEvent& eventData);

		~InputEvent();
	};
}
