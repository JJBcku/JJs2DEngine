#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    struct SdlTextEditingEventData
    {
        uint64_t reserved;
        uint64_t timestamp;                           /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t windowID;                            /**< The window with keyboard focus, if any */
        uint32_t padding;
        const char* text;                             /**< The editing text */
        int32_t start;                                /**< The start cursor of selected editing text */
        int32_t length;                               /**< The length of selected editing text */

        SdlTextEditingEventData();
        ~SdlTextEditingEventData();
    };

    constexpr uint32_t SDL_DATA_TEXTINPUTEVENT_TEXT_SIZE = 32;

    struct SdlTextInputEventData
    {
        uint64_t timestamp;                         /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t reserved;
        uint32_t windowID;                          /**< The window with keyboard focus, if any */
        const char* text;  /**< The input text */

        SdlTextInputEventData();
        ~SdlTextInputEventData();
    };
}
