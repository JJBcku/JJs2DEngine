#pragma once

#include <CustomLists/IDObject.h>

#include "VSImageDataListsDef.h"

namespace VulkanSimplified
{
	enum class ImageIDType : uint64_t
	{
		UNKNOWN = 0,
		COLOR_RENDER_TARGET = 1,
		DEPTH_STENCIL_RENDER_TARGET = 2,
		RESOLVE_RENDER_TARGET = 4,
	};

	union RenderTargetImagesID
	{
		ImageIDType type;

		struct
		{
			ImageIDType type;
			IDObject<AutoCleanupColorRenderTargetImage> ID;
		} colorRenderTarget;

		struct
		{
			ImageIDType type;
			IDObject<AutoCleanupDepthStencilRenderTargetImage> ID;
		} depthStencilRenderTarget;

		struct
		{
			ImageIDType type;
			IDObject<AutoCleanupResolveRenderTargetImage> ID;
		} resolveRenderTarget;

		RenderTargetImagesID();
		RenderTargetImagesID(const IDObject<AutoCleanupColorRenderTargetImage>& ID);
		RenderTargetImagesID(const IDObject<AutoCleanupDepthStencilRenderTargetImage>& ID);
		RenderTargetImagesID(const IDObject<AutoCleanupResolveRenderTargetImage>& ID);
	};
}