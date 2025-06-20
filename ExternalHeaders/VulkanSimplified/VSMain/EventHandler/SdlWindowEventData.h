#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    enum SdlWindowEventID : uint8_t
    {
        SDL_DATA_WINDOWEVENT_NONE,           /**< Never used */
        SDL_DATA_WINDOWEVENT_SHOWN,          /**< Window has been shown */
        SDL_DATA_WINDOWEVENT_HIDDEN,         /**< Window has been hidden */
        SDL_DATA_WINDOWEVENT_EXPOSED,        /**< Window has been exposed and should be
                                             redrawn */
        SDL_DATA_WINDOWEVENT_MOVED,          /**< Window has been moved to data1, data2
                                         */
        SDL_DATA_WINDOWEVENT_RESIZED,        /**< Window has been resized to data1xdata2 */
        SDL_DATA_WINDOWEVENT_SIZE_CHANGED,   /**< The window size has changed, either as
                                             a result of an API call or through the
                                             system or user changing the window size. */
        SDL_DATA_WINDOWEVENT_MINIMIZED,      /**< Window has been minimized */
        SDL_DATA_WINDOWEVENT_MAXIMIZED,      /**< Window has been maximized */
        SDL_DATA_WINDOWEVENT_RESTORED,       /**< Window has been restored to normal size
                                             and position */
        SDL_DATA_WINDOWEVENT_ENTER,          /**< Window has gained mouse focus */
        SDL_DATA_WINDOWEVENT_LEAVE,          /**< Window has lost mouse focus */
        SDL_DATA_WINDOWEVENT_FOCUS_GAINED,   /**< Window has gained keyboard focus */
        SDL_DATA_WINDOWEVENT_FOCUS_LOST,     /**< Window has lost keyboard focus */
        SDL_DATA_WINDOWEVENT_CLOSE,          /**< The window manager requests that the window be closed */
        SDL_DATA_WINDOWEVENT_TAKE_FOCUS,     /**< Window is being offered a focus (should SetWindowInputFocus() on itself or a subwindow, or ignore) */
        SDL_DATA_WINDOWEVENT_HIT_TEST,       /**< Window had a hit test that wasn't SDL_HITTEST_NORMAL. */
        SDL_DATA_WINDOWEVENT_ICCPROF_CHANGED,/**< The ICC profile of the window's display has changed. */
        SDL_DATA_WINDOWEVENT_DISPLAY_CHANGED /**< Window has been moved to display data1. */
    };

    struct SdlWindowEventData
    {
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t windowID;    /**< The associated window */
        uint8_t event;        /**< SdlWindowEventID */
        uint8_t padding1;
        uint8_t padding2;
        uint8_t padding3;
        int32_t data1;       /**< event dependent data */
        int32_t data2;       /**< event dependent data */
        int32_t padding;       /**< event dependent data */

        SdlWindowEventData();
        ~SdlWindowEventData();
    };
}
