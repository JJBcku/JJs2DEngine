#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    struct SdlAudioDeviceEventData
    {
        uint32_t type;        /**< 1 if device was added, or 0 if it was removed */
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t which;       /**< The audio device index for the ADDED event (valid until next SDL_GetNumAudioDevices() call), SDL_AudioDeviceID for the REMOVED event */
        uint8_t iscapture;    /**< zero if an output device, non-zero if a capture device. */
        uint8_t padding1;
        uint8_t padding2;
        uint8_t padding3;

        SdlAudioDeviceEventData();
        ~SdlAudioDeviceEventData();
    };
}