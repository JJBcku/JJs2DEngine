#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    struct SdlJoyAxisEventData
    {
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        int32_t which; /**< The joystick instance id */
        uint8_t axis;         /**< The joystick axis index */
        uint8_t padding1;
        uint8_t padding2;
        uint8_t padding3;
        int16_t value;       /**< The axis value (range: -32768 to 32767) */
        uint16_t padding4;

        SdlJoyAxisEventData();
        ~SdlJoyAxisEventData();
    };

    struct SdlJoyBallEventData
    {
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        int32_t which; /**< The joystick instance id */
        uint8_t ball;         /**< The joystick trackball index */
        uint8_t padding1;
        uint8_t padding2;
        uint8_t padding3;
        int16_t xrel;        /**< The relative motion in the X direction */
        int16_t yrel;        /**< The relative motion in the Y direction */

        SdlJoyBallEventData();
        ~SdlJoyBallEventData();
    };

    enum SdlJoyHatPosition : uint8_t
    {
        SDL_DATA_HAT_CENTERED = 0x0,
        SDL_DATA_HAT_UP = 0x1,
        SDL_DATA_HAT_RIGHT = 0x2,
        SDL_DATA_HAT_DOWN = 0x4,
        SDL_DATA_HAT_LEFT = 0x8,
        SDL_DATA_HAT_RIGHTUP = SDL_DATA_HAT_UP | SDL_DATA_HAT_RIGHT,
        SDL_DATA_HAT_RIGHTDOWN = SDL_DATA_HAT_DOWN | SDL_DATA_HAT_RIGHT,
        SDL_DATA_HAT_LEFTUP = SDL_DATA_HAT_UP | SDL_DATA_HAT_LEFT,
        SDL_DATA_HAT_LEFTDOWN = SDL_DATA_HAT_DOWN | SDL_DATA_HAT_LEFT,
    };

    struct SdlJoyHatEventData
    {
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        int32_t which; /**< The joystick instance id */
        uint8_t hat;          /**< The joystick hat index */
        uint8_t value;        /**< The hat position value.
                             *   \sa ::SDL_DATA_HAT_LEFTUP ::SDL_DATA_HAT_UP ::SDL_DATA_HAT_RIGHTUP
                             *   \sa ::SDL_DATA_HAT_LEFT ::SDL_DATA_HAT_CENTERED ::SDL_DATA_HAT_RIGHT
                             *   \sa ::SDL_DATA_HAT_LEFTDOWN ::SDL_DATA_HAT_DOWN ::SDL_DATA_HAT_RIGHTDOWN
                             *
                             *   Note that zero means the POV is centered.
                             */
        uint8_t padding1;
        uint8_t padding2;
        uint32_t padding3;

        SdlJoyHatEventData();
        ~SdlJoyHatEventData();
    };

    struct SdlJoyButtonEventData
    {
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        int32_t which; /**< The joystick instance id */
        uint8_t button;       /**< The joystick button index */
        uint8_t state;        /**< 1 for pressed or 0 for released */
        uint8_t padding1;
        uint8_t padding2;
        uint32_t padding3;

        SdlJoyButtonEventData();
    };

    struct SdlJoyDeviceEventData
    {
        uint32_t type;        /**< 1 for device added or 0 for device removed */
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        int32_t which;       /**< The joystick device index for the ADDED event, instance id for the REMOVED event */

        SdlJoyDeviceEventData();
        ~SdlJoyDeviceEventData();
    };

    enum SdlJoystickPowerLevel : int32_t
    {
        SDL_DATA_JOYSTICK_POWER_UNKNOWN = -1,
        SDL_DATA_JOYSTICK_POWER_EMPTY,   /* <= 5% */
        SDL_DATA_JOYSTICK_POWER_LOW,     /* <= 20% */
        SDL_DATA_JOYSTICK_POWER_MEDIUM,  /* <= 70% */
        SDL_DATA_JOYSTICK_POWER_FULL,    /* <= 100% */
        SDL_DATA_JOYSTICK_POWER_WIRED,
        SDL_DATA_JOYSTICK_POWER_MAX
    };

    struct SdlJoyBatteryEventData
    {
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        int32_t which; /**< The joystick instance id */
        SdlJoystickPowerLevel level; /**< The joystick battery level */

        SdlJoyBatteryEventData();
        ~SdlJoyBatteryEventData();
    };
}
