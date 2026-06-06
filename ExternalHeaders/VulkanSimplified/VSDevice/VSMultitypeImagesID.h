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

		struct ColorRenderTargetID
		{
			ImageIDType type;
			IDObject<AutoCleanupColorRenderTargetImage> ID;

			ColorRenderTargetID(const IDObject<AutoCleanupColorRenderTargetImage>& ID) noexcept;
			~ColorRenderTargetID() = default;
		} colorRenderTarget;

		struct DepthStencilRenderTargetID
		{
			ImageIDType type;
			IDObject<AutoCleanupDepthStencilRenderTargetImage> ID;

			DepthStencilRenderTargetID(const IDObject<AutoCleanupDepthStencilRenderTargetImage>& ID) noexcept;
			~DepthStencilRenderTargetID() = default;
		} depthStencilRenderTarget;

		struct ResolveRenderTargetID
		{
			ImageIDType type;
			IDObject<AutoCleanupResolveRenderTargetImage> ID;

			ResolveRenderTargetID(const IDObject<AutoCleanupResolveRenderTargetImage>& ID) noexcept;
			~ResolveRenderTargetID() = default;
		} resolveRenderTarget;

		RenderTargetImagesID() noexcept;
		RenderTargetImagesID(const IDObject<AutoCleanupColorRenderTargetImage>& ID) noexcept;
		RenderTargetImagesID(const IDObject<AutoCleanupDepthStencilRenderTargetImage>& ID) noexcept;
		RenderTargetImagesID(const IDObject<AutoCleanupResolveRenderTargetImage>& ID) noexcept;

		RenderTargetImagesID(const RenderTargetImagesID& rhs) noexcept;
		~RenderTargetImagesID();

		RenderTargetImagesID& operator=(const RenderTargetImagesID& rhs) noexcept;
	};
}