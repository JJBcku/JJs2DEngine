#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    struct SdlSensorEventData
    {
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        int32_t which;       /**< The instance ID of the sensor */
        float data[6];      /**< Up to 6 values from the sensor - additional values can be queried using SDL_SensorGetData() */
        uint64_t timestamp_us; /**< The timestamp of the sensor reading in microseconds, if the hardware provides this information. */

        SdlSensorEventData();
        ~SdlSensorEventData();
    };
}
