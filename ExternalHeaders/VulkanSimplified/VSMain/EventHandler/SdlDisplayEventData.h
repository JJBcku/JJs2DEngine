#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    enum SdlDisplayEventID : uint8_t
    {
        SDL_DATA_DISPLAYEVENT_NONE,          /**< Never used */
        SDL_DATA_DISPLAYEVENT_ORIENTATION,   /**< Display orientation has changed to data1 */
        SDL_DATA_DISPLAYEVENT_CONNECTED,     /**< Display has been added to the system */
        SDL_DATA_DISPLAYEVENT_DISCONNECTED,  /**< Display has been removed from the system */
        SDL_DATA_DISPLAYEVENT_MOVED          /**< Display has changed position */
    };

    struct SdlDisplayEventData
    {
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t display;     /**< The associated display index */
        uint8_t event;        /**< SdlDisplayEventID */
        uint8_t padding1;
        uint8_t padding2;
        uint8_t padding3;
        int32_t data1;       /**< event dependent data */

        SdlDisplayEventData();
        ~SdlDisplayEventData();
    };
}
