#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    enum SdlGamepadAxis : uint8_t
    {
        SDL_DATA_CONTROLLER_AXIS_INVALID = 0xFF,
        SDL_DATA_CONTROLLER_AXIS_LEFTX = 0,
        SDL_DATA_CONTROLLER_AXIS_LEFTY,
        SDL_DATA_CONTROLLER_AXIS_RIGHTX,
        SDL_DATA_CONTROLLER_AXIS_RIGHTY,
        SDL_DATA_CONTROLLER_AXIS_TRIGGERLEFT,
        SDL_DATA_CONTROLLER_AXIS_TRIGGERRIGHT,
        SDL_DATA_CONTROLLER_AXIS_MAX
    };

    struct SdlGamepadAxisEventData
    {
        uint64_t timestamp;     /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t reserved;
        uint32_t which;         /**< The joystick instance id */
        uint8_t axis;           /**< The controller axis (SdlGameControllerAxis) */
        uint8_t padding1;
        uint8_t padding2;
        uint8_t padding3;
        int16_t value;          /**< The axis value (range: -32768 to 32767) */
        uint16_t padding4;

        SdlGamepadAxisEventData();
        ~SdlGamepadAxisEventData();
    };

    enum SdlGamepadButtons : uint8_t
    {
        SDL_DATA_CONTROLLER_BUTTON_INVALID = 0xFF,
        SDL_DATA_CONTROLLER_BUTTON_A = 0,
        SDL_DATA_CONTROLLER_BUTTON_B,
        SDL_DATA_CONTROLLER_BUTTON_X,
        SDL_DATA_CONTROLLER_BUTTON_Y,
        SDL_DATA_CONTROLLER_BUTTON_BACK,
        SDL_DATA_CONTROLLER_BUTTON_GUIDE,
        SDL_DATA_CONTROLLER_BUTTON_START,
        SDL_DATA_CONTROLLER_BUTTON_LEFTSTICK,
        SDL_DATA_CONTROLLER_BUTTON_RIGHTSTICK,
        SDL_DATA_CONTROLLER_BUTTON_LEFTSHOULDER,
        SDL_DATA_CONTROLLER_BUTTON_RIGHTSHOULDER,
        SDL_DATA_CONTROLLER_BUTTON_DPAD_UP,
        SDL_DATA_CONTROLLER_BUTTON_DPAD_DOWN,
        SDL_DATA_CONTROLLER_BUTTON_DPAD_LEFT,
        SDL_DATA_CONTROLLER_BUTTON_DPAD_RIGHT,
        SDL_DATA_CONTROLLER_BUTTON_MISC1,    /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button */
        SDL_DATA_CONTROLLER_BUTTON_PADDLE1,  /* Xbox Elite paddle P1 (upper left, facing the back) */
        SDL_DATA_CONTROLLER_BUTTON_PADDLE2,  /* Xbox Elite paddle P3 (upper right, facing the back) */
        SDL_DATA_CONTROLLER_BUTTON_PADDLE3,  /* Xbox Elite paddle P2 (lower left, facing the back) */
        SDL_DATA_CONTROLLER_BUTTON_PADDLE4,  /* Xbox Elite paddle P4 (lower right, facing the back) */
        SDL_DATA_CONTROLLER_BUTTON_TOUCHPAD, /* PS4/PS5 touchpad button */
        SDL_DATA_CONTROLLER_BUTTON_MAX
    };

    struct SdlGamepadButtonEventData
    {
        uint64_t timestamp;         /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t reserved;
        uint32_t which;             /**< The joystick instance id */
        uint8_t button;             /**< The controller button (SDL_GameControllerButton) */
        bool down;
        uint8_t padding1;
        uint8_t padding2;
        uint32_t padding3;

        SdlGamepadButtonEventData();
        ~SdlGamepadButtonEventData();
    };

    enum class SdlGamepadDeviceSubEventType : uint32_t
    {
        DEVICE_ADDED = 0,
        DEVICE_REMOVED = 1,
        DEVICE_REMAPPED = 2,
        DEVICE_UPDATE_COMPLETE = 4,
        DEVICE_STEAM_HANDLE_UPDATED = 8,
        DEVICE_UNKNOWN = std::numeric_limits<uint32_t>::max()
    };

    struct SdlGamepadDeviceEventData
    {
        SdlGamepadDeviceSubEventType type;
        uint32_t reserved;
        uint64_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t which;       /**< The joystick device index for the ADDED event, instance id for the REMOVED or REMAPPED event */
        uint32_t padding;

        SdlGamepadDeviceEventData();
        ~SdlGamepadDeviceEventData();
    };

    enum class SdlGamepadTouchpadEventType : uint32_t
    {
        TOUCHPAD_DOWN = 0,
        TOUCHPAD_MOTION = 1,
        TOUCHPAD_UP = 2,

        TOUCHPAD_UNKNOWN = std::numeric_limits<uint32_t>::max()
    };

    struct SdlGamepadTouchpadEventData
    {
        SdlGamepadTouchpadEventType type;
        uint32_t reserved;
        uint64_t timestamp;     /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t which;         /**< The joystick instance id */
        int32_t touchpad;       /**< The index of the touchpad */
        int32_t finger;         /**< The index of the finger on the touchpad */
        float x;                /**< Normalized in the range 0...1 with 0 being on the left */
        float y;                /**< Normalized in the range 0...1 with 0 being at the top */
        float pressure;         /**< Normalized in the range 0...1 */

        SdlGamepadTouchpadEventData();
        ~SdlGamepadTouchpadEventData();
    };

    struct SdlGamepadSensorEventData
    {
        uint64_t timestamp;         /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t reserved;
        uint32_t which;             /**< The joystick instance id */
        int32_t sensor;             /**< The type of the sensor, one of the values of ::SDL_SensorType */
        float data[3];              /**< Up to 3 values from the sensor, as defined in SDL_sensor.h */
        uint64_t sensor_timestamp;  /**< The timestamp of the sensor reading in microseconds, if the hardware provides this information. */

        SdlGamepadSensorEventData();
        ~SdlGamepadSensorEventData();
    };
}
