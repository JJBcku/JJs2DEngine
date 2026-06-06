#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    enum SdlDisplayEventID : uint32_t
    {
        SDL_DATA_DISPLAYEVENT_ORIENTATION,                 /**< Display orientation has changed to data1 */
        SDL_DATA_DISPLAYEVENT_ADDED,                       /**< Display has been added to the system */
        SDL_DATA_DISPLAYEVENT_REMOVED,                     /**< Display has been removed from the system */
        SDL_DATA_DISPLAYEVENT_MOVED,                       /**< Display has changed position */
        SDL_DATA_DISPLAYEVENT_DESKTOP_MODE_CHANGED,        /**< Display has changed desktop mode */
        SDL_DATA_DISPLAYEVENT_CURRENT_MODE_CHANGED,        /**< Display has changed current mode */
        SDL_DATA_DISPLAYEVENT_CONTENT_SCALE_CHANGED,       /**< Display has changed content scale */
        SDL_DATA_DISPLAYEVENT_USABLE_BOUNDS_CHANGED,       /**< Display has changed usable bounds */
    };

    struct SdlDisplayEventData
    {
        uint64_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        uint64_t reserved;
        uint32_t display;     /**< The associated display index */
        uint32_t event;        /**< Event ID */
        int32_t data1;       /**< event dependent data */
        int32_t data2;       /**< event dependent data */

        SdlDisplayEventData();
        ~SdlDisplayEventData();
    };
}
