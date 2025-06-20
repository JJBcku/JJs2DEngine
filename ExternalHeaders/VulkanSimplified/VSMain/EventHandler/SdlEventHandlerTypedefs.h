#pragma once

#include <functional>

namespace VulkanSimplified
{
	struct SdlEventHandlerInitData;

	struct SdlAppTerminatingEventData;
	struct SdlAppLowMemoryEventData;
	struct SdlAppWillEnterBackgroundEventData;
	struct SdlAppDidEnterBackgroundEventData;
	struct SdlAppWillEnterForegroundEventData;
	struct SdlAppDidEnterForegroundEventData;

	struct SdlAudioDeviceEventData;

	struct SdlClipboardEventData;

	struct SdlControllerAxisEventData;
	struct SdlControllerButtonEventData;
	struct SdlControllerDeviceEventData;
	struct SdlControllerTouchpadEventData;
	struct SdlControllerSensorEventData;

	struct SdlDisplayEventData;

	struct SdlDropEventData;

	struct SdlJoyAxisEventData;
	struct SdlJoyBallEventData;
	struct SdlJoyHatEventData;
	struct SdlJoyButtonEventData;
	struct SdlJoyDeviceEventData;
	struct SdlJoyBatteryEventData;

	struct SdlKeyboardEventData;
	struct SdlKeymapChangedEventData;

	struct SdlLocaleChangeEventData;

	struct SdlMouseMotionEventData;
	struct SdlMouseButtonEventData;
	struct SdlMouseWheelEventData;

	struct SdlQuitEventData;

	struct SdlRenderTargetsResetEventData;
	struct SdlRenderDeviceResetEventData;

	struct SdlSensorEventData;

	struct SdlTextEditingEventData;
	struct SdlTextEditingExtendedEventData;
	struct SdlTextInputEventData;

	struct SdlTouchFingerEventData;
	struct SdlMultiGestureEventData;
	struct SdlDollarGestureEventData;

	struct SdlUserEventData;

	struct SdlWindowEventData;

	typedef std::function<bool(const SdlAppTerminatingEventData&, void*)> AppTerminatingEventFunction;
	typedef std::function<bool(const SdlAppLowMemoryEventData&, void*)> AppLowMemoryEventFunction;
	typedef std::function<bool(const SdlAppWillEnterBackgroundEventData&, void*)> AppWillEnterBackgroundEventFunction;
	typedef std::function<bool(const SdlAppDidEnterBackgroundEventData&, void*)> AppDidEnterBackgroundEventFunction;
	typedef std::function<bool(const SdlAppWillEnterForegroundEventData&, void*)> AppWillEnterForegroundEventFunction;
	typedef std::function<bool(const SdlAppDidEnterForegroundEventData&, void*)> AppDidEnterForegroundEventFunction;

	typedef std::function<bool(const SdlAudioDeviceEventData&, void*)> AudioDeviceEventFunction;

	typedef std::function<bool(const SdlClipboardEventData&, void*)> ClipboardEventFunction;

	typedef std::function<bool(const SdlControllerAxisEventData&, void*)> ControllerAxisEventFunction;
	typedef std::function<bool(const SdlControllerButtonEventData&, void*)> ControllerButtonEventFunction;
	typedef std::function<bool(const SdlControllerDeviceEventData&, void*)> ControllerDeviceEventFunction;
	typedef std::function<bool(const SdlControllerTouchpadEventData&, void*)> ControllerTouchpadEventFunction;
	typedef std::function<bool(const SdlControllerSensorEventData&, void*)> ControllerSensorEventFunction;

	typedef std::function<bool(const SdlDisplayEventData&, void*)> DisplayEventFunction;

	typedef std::function<bool(const SdlDropEventData&, void*)> DropEventFunction;

	typedef std::function<bool(const SdlJoyAxisEventData&, void*)> JoyAxisEventFunction;
	typedef std::function<bool(const SdlJoyBallEventData&, void*)> JoyBallEventFunction;
	typedef std::function<bool(const SdlJoyHatEventData&, void*)> JoyHatEventFunction;
	typedef std::function<bool(const SdlJoyButtonEventData&, void*)> JoyButtonEventFunction;
	typedef std::function<bool(const SdlJoyDeviceEventData&, void*)> JoyDeviceEventFunction;
	typedef std::function<bool(const SdlJoyBatteryEventData&, void*)> JoyBatteryEventFunction;

	typedef std::function<bool(const SdlKeyboardEventData&, void*)> KeyboardEventFunction;
	typedef std::function<bool(const SdlKeymapChangedEventData&, void*)> KeymapChangeEventFunction;

	typedef std::function<bool(const SdlLocaleChangeEventData&, void*)> LocaleChangedEventFunction;

	typedef std::function<bool(const SdlMouseMotionEventData&, void*)> MouseMotionEventFunction;
	typedef std::function<bool(const SdlMouseButtonEventData&, void*)> MouseButtonEventFunction;
	typedef std::function<bool(const SdlMouseWheelEventData&, void*)> MouseWheelEventFunction;

	typedef std::function<bool(const SdlQuitEventData&, void*)> QuitEventFunction;

	typedef std::function<bool(const SdlRenderTargetsResetEventData&, void*)> RenderTargetsResetEventFunction;
	typedef std::function<bool(const SdlRenderDeviceResetEventData&, void*)> RenderDeviceResetEventFunction;

	typedef std::function<bool(const SdlSensorEventData&, void*)> SensorEventFunction;

	typedef std::function<bool(const SdlTextEditingEventData&, void*)> TextEditingEventFunction;
	typedef std::function<bool(const SdlTextEditingExtendedEventData&, void*)> TextEditingExtendedEventFunction;
	typedef std::function<bool(const SdlTextInputEventData&, void*)> TextInputEventFunction;

	typedef std::function<bool(const SdlTouchFingerEventData&, void*)> TouchFingerEventFunction;
	typedef std::function<bool(const SdlMultiGestureEventData&, void*)> MultiGestureEventFunction;
	typedef std::function<bool(const SdlDollarGestureEventData&, void*)> DollarGestureEventFunction;

	typedef std::function<bool(const SdlUserEventData&, void*)> UserEventFunction;

	typedef std::function<bool(const SdlWindowEventData&, void*)> WindowEventFunction;
}