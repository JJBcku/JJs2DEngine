#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
    enum class SdlTouchFingerEventType : uint32_t
    {
        FINGER_DOWN,
        FINGER_UP,
        FINGER_MOTION,
    };

    struct SdlTouchFingerEventData
    {
        SdlTouchFingerEventType type;
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        int64_t touchId; /**< The touch device id */
        int64_t fingerId;
        float x;            /**< Normalized in the range 0...1 */
        float y;            /**< Normalized in the range 0...1 */
        float dx;           /**< Normalized in the range -1...1 */
        float dy;           /**< Normalized in the range -1...1 */
        float pressure;     /**< Normalized in the range 0...1 */
        uint32_t windowID;    /**< The window underneath the finger, if any */

        SdlTouchFingerEventData();
        ~SdlTouchFingerEventData();
    };

    struct SdlMultiGestureEventData
    {
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        uint32_t padding2;
        int64_t touchId; /**< The touch device id */
        float dTheta;
        float dDist;
        float x;
        float y;
        uint16_t numFingers;
        uint16_t padding;
        uint32_t padding3;

        SdlMultiGestureEventData();
        ~SdlMultiGestureEventData();
    };

    struct SdlDollarGestureEventData
    {
        uint32_t type;        /**< 1 for gesture performed or 0 for gesture recorded */
        uint32_t timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
        int64_t touchId; /**< The touch device id */
        int64_t gestureId;
        uint32_t numFingers;
        float error;
        float x;            /**< Normalized center of gesture */
        float y;            /**< Normalized center of gesture */

        SdlDollarGestureEventData();
        ~SdlDollarGestureEventData();
    };
}
