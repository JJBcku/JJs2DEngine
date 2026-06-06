#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    struct SdlSensorEventData
    {
        uint64_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t reserved;
        uint32_t which;       /**< The instance ID of the sensor */
        float data[6];      /**< Up to 6 values from the sensor - additional values can be queried using SDL_SensorGetData() */
        uint64_t sensor_timestamp; /**< The timestamp of the sensor reading in microseconds, if the hardware provides this information. */

        SdlSensorEventData();
        ~SdlSensorEventData();
    };
}
