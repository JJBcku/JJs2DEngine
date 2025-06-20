#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    constexpr uint32_t SDL_DATA_TEXTEDITINGEVENT_TEXT_SIZE = 32;

    struct SdlTextEditingEventData
    {
        uint32_t timestamp;                           /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t windowID;                            /**< The window with keyboard focus, if any */
        char text[SDL_DATA_TEXTEDITINGEVENT_TEXT_SIZE];  /**< The editing text */
        int32_t start;                               /**< The start cursor of selected editing text */
        int32_t length;                              /**< The length of selected editing text */

        SdlTextEditingEventData();
        ~SdlTextEditingEventData();
    };

    struct SdlTextEditingExtendedEventData
    {
        uint32_t timestamp;                           /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t windowID;                            /**< The window with keyboard focus, if any */
        char* text;                                 /**< The editing text, which should be freed with SDL_free(), and will not be NULL */
        int32_t start;                               /**< The start cursor of selected editing text */
        int32_t length;

        SdlTextEditingExtendedEventData();
        ~SdlTextEditingExtendedEventData();
    };

    constexpr uint32_t SDL_DATA_TEXTINPUTEVENT_TEXT_SIZE = 32;

    struct SdlTextInputEventData
    {
        uint32_t timestamp;                         /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t windowID;                          /**< The window with keyboard focus, if any */
        char text[SDL_DATA_TEXTINPUTEVENT_TEXT_SIZE];  /**< The input text */

        SdlTextInputEventData();
        ~SdlTextInputEventData();
    };
}
