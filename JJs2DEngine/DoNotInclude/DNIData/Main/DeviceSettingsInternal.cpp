#include "MainDNIpch.h"
#include "DeviceSettingsInternal.h"

#include "../../../Include/Main/AspectRatio.h"

namespace JJs2DEngine
{
	VS::DataFormatSetIndependentID TranslateToFormat(SwapchainFormat format)
	{
		VS::DataFormatSetIndependentID ret;

		switch (format)
		{
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_UNSET:
			break;
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_RGBA16:
			ret = VS::DataFormatSetIndependentID(VS::DATA_FORMAT_RGBA16_UNORM);
			break;
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_A2RGB10:
			ret = VS::DataFormatSetIndependentID(VS::DATA_FORMAT_A2_RGB10_UNORM_PACK32);
			break;
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_A2BGR10:
			ret = VS::DataFormatSetIndependentID(VS::DATA_FORMAT_A2_BGR10_UNORM_PACK32);
			break;
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_RGBA8:
			ret = VS::DataFormatSetIndependentID(VS::DATA_FORMAT_RGBA8_UNORM);
			break;
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_BGRA8:
			ret = VS::DataFormatSetIndependentID(VS::DATA_FORMAT_BGRA8_UNORM);
			break;
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_ABGR8:
			ret = VS::DataFormatSetIndependentID(VS::DATA_FORMAT_ABGR8_UNORM_PACK32);
			break;
		default:
			break;
		}

		return ret;
	}

	VS::DataFormatSetIndependentID TranslateToFormat(TextureFormat format)
	{
		VS::DataFormatSetIndependentID ret;

		switch (format)
		{
		case JJs2DEngine::TextureFormat::TEXTURE_FORMAT_UNSET:
			break;
		case JJs2DEngine::TextureFormat::TEXTURE_FORMAT_RGBA16:
			ret = VS::DataFormatSetIndependentID(VS::DATA_FORMAT_RGBA16_UNORM);
			break;
		case JJs2DEngine::TextureFormat::TEXTURE_FORMAT_RGBA8:
			ret = VS::DataFormatSetIndependentID(VS::DATA_FORMAT_RGBA8_SRGB);
			break;
		case JJs2DEngine::TextureFormat::TEXTURE_FORMAT_BGRA8:
			ret = VS::DataFormatSetIndependentID(VS::DATA_FORMAT_BGRA8_SRGB);
			break;
		default:
			break;
		}

		return ret;
	}

	VS::DataFormatSetIndependentID TranslateToFormat(DepthFormat format)
	{
		VS::DataFormatSetIndependentID ret;

		switch (format)
		{
		case JJs2DEngine::DepthFormat::DEPTH_FORMAT_UNSET:
			break;
		case JJs2DEngine::DepthFormat::DEPTH_FORMAT_D32:
			ret = VS::DataFormatSetIndependentID(VS::DATA_FORMAT_D32_SFLOAT);
			break;
		case JJs2DEngine::DepthFormat::DEPTH_FORMAT_D32_S8:
			ret = VS::DataFormatSetIndependentID(VS::DATA_FORMAT_D32_SFLOAT_S8_UINT);
			break;
		default:
			break;
		}

		return ret;
	}

	uint32_t GetHeight(uint32_t width, AspectRatio aspectRatio)
	{
		uint32_t ret = 0;

		double ratio = 0.0;

		switch (aspectRatio)
		{
		case JJs2DEngine::AspectRatio::UNSET:
			break;
		case JJs2DEngine::AspectRatio::ASPECT_RATIO_16_9:
			ratio = 9.0 / 16.0;
			break;
		default:
			break;
		}

		double height = static_cast<double>(width) * ratio;
		ret = static_cast<uint32_t>(height);

		return ret;
	}

}