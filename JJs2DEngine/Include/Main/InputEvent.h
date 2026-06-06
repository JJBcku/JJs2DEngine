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

		uint16_t padding;

		KeyEventData();
	};

	struct MouseMotionEvent
	{
		uint32_t buttonState;
		float positionX;
		float positionY;

		MouseMotionEvent();
	};

	struct MouseButtonEvent
	{
		float mousePositionX;
		float mousePositionY;
		uint8_t buttonIndex;
		bool buttonPressed;
		bool doubleClick;
		bool padding;

		MouseButtonEvent();
	};

	struct MouseWheelEvent
	{
		float scrollX;
		float scrollY;
		bool directionFlipped;
		uint8_t padding1;
		uint16_t padding2;

		MouseWheelEvent();
	};

	union InputEvent
	{
		InputEventsType type;

		struct KeyEvent
		{
			InputEventsType type;
			KeyEventData data;

			KeyEvent(const KeyEventData& eventData) noexcept;
			~KeyEvent() = default;
		} keyEvent;

		struct MouseMoveEvent
		{
			InputEventsType type;
			MouseMotionEvent data;

			MouseMoveEvent(const MouseMotionEvent& eventData) noexcept;
			~MouseMoveEvent() = default;
		} mouseMotionEvent;

		struct MouseButtonPress
		{
			InputEventsType type;
			MouseButtonEvent data;

			MouseButtonPress(const MouseButtonEvent& eventData) noexcept;
			~MouseButtonPress() = default;
		} mouseButtonEvent;

		struct MouseScrollEvent
		{
			InputEventsType type;
			MouseWheelEvent data;

			MouseScrollEvent(const MouseWheelEvent& eventData) noexcept;
			~MouseScrollEvent() noexcept = default;
		} mouseWheelEvent;

		InputEvent() noexcept;
		InputEvent(const KeyEventData& eventData) noexcept;
		InputEvent(const MouseMotionEvent& eventData) noexcept;
		InputEvent(const MouseButtonEvent& eventData) noexcept;
		InputEvent(const MouseWheelEvent& eventData) noexcept;

		~InputEvent() = default;
	};
}
