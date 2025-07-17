#pragma once

#include "../../../Include/Main/DeviceSettings.h"

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>

namespace JJs2DEngine
{
	VS::DataFormatSetIndependentID TranslateToFormat(SwapchainFormat format);
	VS::DataFormatSetIndependentID TranslateToFormat(TextureFormat format);
	VS::DataFormatSetIndependentID TranslateToFormat(DepthFormat format);

	uint32_t GetHeight(uint32_t width, AspectRatio aspectRatio);
}