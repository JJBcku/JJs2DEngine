#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    struct SdlUserEventData
    {
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t windowID;    /**< The associated window if any */
        int32_t code;        /**< User defined event code */
        uint32_t padding;
        void* data1;        /**< User defined data pointer */
        void* data2;        /**< User defined data pointer */

        SdlUserEventData();
        ~SdlUserEventData();
    };
}
