#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    enum SdlWindowEventID : uint32_t
    {
        SDL_DATA_WINDOWEVENT_SHOWN,                     /**< Window has been shown */
        SDL_DATA_WINDOWEVENT_HIDDEN,                    /**< Window has been hidden */
        SDL_DATA_WINDOWEVENT_EXPOSED,                   /**< Window has been exposed and should be redrawn, and can be redrawn directly from event watchers for this event.
                                                             data1 is 1 for live-resize expose events, 0 otherwise. */
        SDL_DATA_WINDOWEVENT_MOVED,                     /**< Window has been moved to data1, data2 */
        SDL_DATA_WINDOWEVENT_RESIZED,                   /**< Window has been resized to data1xdata2 */
        SDL_DATA_WINDOWEVENT_PIXEL_SIZE_CHANGED,        /**< The pixel size of the window has changed to data1xdata2 */
        SDL_DATA_WINDOWEVENT_METAL_VIEW_RESIZED,        /**< The pixel size of a Metal view associated with the window has changed */
        SDL_DATA_WINDOWEVENT_MINIMIZED,                 /**< Window has been minimized */
        SDL_DATA_WINDOWEVENT_MAXIMIZED,                 /**< Window has been maximized */
        SDL_DATA_WINDOWEVENT_RESTORED,                  /**< Window has been restored to normal size and position */
        SDL_DATA_WINDOWEVENT_MOUSE_ENTER,               /**< Window has gained mouse focus */
        SDL_DATA_WINDOWEVENT_MOUSE_LEAVE,               /**< Window has lost mouse focus */
        SDL_DATA_WINDOWEVENT_FOCUS_GAINED,              /**< Window has gained keyboard focus */
        SDL_DATA_WINDOWEVENT_FOCUS_LOST,                /**< Window has lost keyboard focus */
        SDL_DATA_WINDOWEVENT_CLOSE_REQUESTED,           /**< The window manager requests that the window be closed */
        SDL_DATA_WINDOWEVENT_HIT_TEST,                  /**< Window had a hit test that wasn't SDL_HITTEST_NORMAL. */
        SDL_DATA_WINDOWEVENT_ICCPROF_CHANGED,           /**< The ICC profile of the window's display has changed. */
        SDL_DATA_WINDOWEVENT_DISPLAY_CHANGED,           /**< Window has been moved to display data1. */
        SDL_DATA_WINDOWEVENT_DISPLAY_SCALE_CHANGED,     /**< Window display scale has been changed */
        SDL_DATA_WINDOWEVENT_SAFE_AREA_CHANGED,         /**< The window safe area has been changed */
        SDL_DATA_WINDOWEVENT_OCCLUDED,                  /**< The window has been occluded */
        SDL_DATA_WINDOWEVENT_ENTER_FULLSCREEN,          /**< The window has entered fullscreen mode */
        SDL_DATA_WINDOWEVENT_LEAVE_FULLSCREEN,          /**< The window has left fullscreen mode */
        SDL_DATA_WINDOWEVENT_WINDOW_DESTROYED,          /**< The window with the associated ID is being or has been destroyed. If this message is being handled
                                                             in an event watcher, the window handle is still valid and can still be used to retrieve any properties
                                                             associated with the window. Otherwise, the handle has already been destroyed and all resources
                                                             associated with it are invalid */
        SDL_DATA_WINDOWEVENT_HDR_STATE_CHANGED,         /**< Window HDR properties have changed */
    };

    struct SdlWindowEventData
    {
        uint64_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t reserved;    
        uint32_t windowID;    /**< The associated window */
        uint32_t event;        /**< Event ID */
        int32_t data1;       /**< event dependent data */
        int32_t data2;       /**< event dependent data */
        int32_t padding;       /**< event dependent data */

        SdlWindowEventData();
        ~SdlWindowEventData();
    };
}
