#pragma once

#include "../VSDevice/VSImageDataListsDef.h"

#include <CustomLists/IDObject.h>

namespace VulkanSimplified
{
	enum class ImageViewIDType : uint64_t
	{
		NONE = 0,
		COLOR = 1,
		DEPTH_STENCIL = 2,
		TEXTURE_2D = 4,
		ARRAY_TEXTURE_2D = 8,
	};

	union ImageViewGenericID
	{
		ImageViewIDType IDType;

		struct ImageColorViewID
		{
			ImageViewIDType IDType;
			IDObject<AutoCleanupColorRenderTargetImage> imageID;
			IDObject<AutoCleanupImageView> viewID;

			ImageColorViewID(const IDObject<AutoCleanupColorRenderTargetImage>& imageID, const IDObject<AutoCleanupImageView>& viewID) noexcept;
			~ImageColorViewID() = default;
		} colorViewID;

		struct ImageDepthStencilViewID
		{
			ImageViewIDType IDType;
			IDObject<AutoCleanupDepthStencilRenderTargetImage> imageID;
			IDObject<AutoCleanupImageView> viewID;

			ImageDepthStencilViewID(const IDObject<AutoCleanupDepthStencilRenderTargetImage>& imageID, const IDObject<AutoCleanupImageView>& viewID) noexcept;
			~ImageDepthStencilViewID() = default;
		} depthStencilViewID;

		struct Texture2DViewID
		{
			ImageViewIDType IDType;
			IDObject<AutoCleanup2DTexture> imageID;
			IDObject<AutoCleanupImageView> viewID;

			Texture2DViewID(const IDObject<AutoCleanup2DTexture>& imageID, const IDObject<AutoCleanupImageView>& viewID) noexcept;
			~Texture2DViewID() = default;
		} texture2DViewID;

		struct Texture2DArrayViewID
		{
			ImageViewIDType IDType;
			IDObject<AutoCleanup2DArrayTexture> imageID;
			IDObject<AutoCleanupImageView> viewID;

			Texture2DArrayViewID(const IDObject<AutoCleanup2DArrayTexture>& imageID, const IDObject<AutoCleanupImageView>& viewID) noexcept;
			~Texture2DArrayViewID() = default;
		} texture2DArrayViewID;

		ImageViewGenericID() noexcept;
		ImageViewGenericID(const IDObject<AutoCleanupColorRenderTargetImage>& imageID, const IDObject<AutoCleanupImageView>& viewID) noexcept;
		ImageViewGenericID(const IDObject<AutoCleanupDepthStencilRenderTargetImage>& imageID, const IDObject<AutoCleanupImageView>& viewID) noexcept;
		ImageViewGenericID(const IDObject<AutoCleanup2DTexture>& imageID, const IDObject<AutoCleanupImageView>& viewID) noexcept;
		ImageViewGenericID(const IDObject<AutoCleanup2DArrayTexture>& imageID, const IDObject<AutoCleanupImageView>& viewID) noexcept;

		ImageViewGenericID(const ImageViewGenericID& rhs) noexcept;
		ImageViewGenericID(ImageViewGenericID&& rhs) noexcept;
		~ImageViewGenericID();

		ImageViewGenericID& operator=(const ImageViewGenericID& rhs) noexcept;
		ImageViewGenericID& operator=(ImageViewGenericID&& rhs) noexcept;
	};
}