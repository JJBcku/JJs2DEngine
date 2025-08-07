#pragma once

#include "../VSCommon/VSImageUsageFlagsDef.h"
#include "../VSCommon/VSImageSampleFlagsDef.h"
#include "../VSCommon/VSMemorySuballocationFullID.h"
#include "../VSCommon/VSMemorySizeDef.h"

#include "VSImageDataListsDef.h"
#include "VSRenderPassDataListDef.h"
#include "VSMultitypeImagesIDDef.h"

#include <stdint.h>
#include <vector>
#include <optional>

namespace VulkanSimplified
{
	class ImageDataListsInternal;
}

namespace VulkanSimplified
{
	struct DataFormatSetIndependentID;

	class ImageDataLists
	{
	public:
		ImageDataLists(ImageDataListsInternal& ref);
		~ImageDataLists();

		IDObject<AutoCleanupColorRenderTargetImage> AddColorRenderTargetImage(uint32_t width, uint32_t height, DataFormatSetIndependentID format,
			ImageSampleFlagBits imageSamples, const std::vector<size_t>& queuesUsingImage, bool preInitialized, bool transient, size_t initialImageViewListCapacity,
			size_t addOnReserving = 0);
		IDObject<AutoCleanupDepthStencilRenderTargetImage> AddDepthStencilRenderTargetImage(uint32_t width, uint32_t height, DataFormatSetIndependentID format,
			ImageSampleFlagBits imageSamples, const std::vector<size_t>& queuesUsingImage, bool preInitialized, bool transient, size_t initialImageViewListCapacity,
			size_t addOnReserving = 0);
		IDObject<AutoCleanupResolveRenderTargetImage> AddResolveRenderTargetImage(uint32_t width, uint32_t height, DataFormatSetIndependentID format,
			const std::vector<size_t>& queuesUsingImage, bool preInitialized, bool transient, size_t initialImageViewListCapacity, size_t addOnReserving = 0);

		IDObject<AutoCleanup2DTexture> Add2DTextureImage(uint32_t width, uint32_t height, uint32_t mipLevel, DataFormatSetIndependentID format,
			const std::vector<size_t>& queuesUsingImage, bool preInitialized, size_t initialImageViewListCapacity, size_t addOnReserving = 0);
		IDObject<AutoCleanup2DArrayTexture> Add2DArrayTextureImage(uint32_t width, uint32_t height, uint32_t layerCount, uint32_t mipLevel, DataFormatSetIndependentID format,
			const std::vector<size_t>& queuesUsingImage, bool preInitialized, size_t initialImageViewListCapacity, size_t addOnReserving = 0);

		bool RemoveColorRenderTargetImage(IDObject<AutoCleanupColorRenderTargetImage> imageID, bool throwOnIDNotFound);
		bool RemoveDepthStencilRenderTargetImage(IDObject<AutoCleanupDepthStencilRenderTargetImage> imageID, bool throwOnIDNotFound);
		bool RemoveResolveRenderTargetImage(IDObject<AutoCleanupResolveRenderTargetImage> imageID, bool throwOnIDNotFound);

		bool Remove2DTextureImage(IDObject<AutoCleanup2DTexture> imageID, bool throwOnIDNotFound);
		bool Remove2DArrayTextureImage(IDObject<AutoCleanup2DArrayTexture> imageID, bool throwOnIDNotFound);

		uint32_t GetColorRenderTargetImagesWidth(IDObject<AutoCleanupColorRenderTargetImage> imageID) const;
		uint32_t GetColorRenderTargetImagesHeight(IDObject<AutoCleanupColorRenderTargetImage> imageID) const;

		uint32_t GetColorRenderTargetImagesMemoryTypeMask(IDObject<AutoCleanupColorRenderTargetImage> imageID) const;
		MemorySize GetColorRenderTargetImagesSize(IDObject<AutoCleanupColorRenderTargetImage> imageID) const;
		MemorySize GetColorRenderTargetImagesRequiredAligment(IDObject<AutoCleanupColorRenderTargetImage> imageID) const;

		uint32_t GetDepthStencilRenderTargetImagesWidth(IDObject<AutoCleanupDepthStencilRenderTargetImage> imageID) const;
		uint32_t GetDepthStencilRenderTargetImagesHeight(IDObject<AutoCleanupDepthStencilRenderTargetImage> imageID) const;

		uint32_t GetDepthStencilRenderTargetImagesMemoryTypeMask(IDObject<AutoCleanupDepthStencilRenderTargetImage> imageID) const;
		MemorySize GetDepthStencilRenderTargetImagesSize(IDObject<AutoCleanupDepthStencilRenderTargetImage> imageID) const;
		MemorySize GetDepthStencilRenderTargetImagesRequiredAligment(IDObject<AutoCleanupDepthStencilRenderTargetImage> imageID) const;

		uint32_t GetResolveRenderTargetImagesWidth(IDObject<AutoCleanupResolveRenderTargetImage> imageID) const;
		uint32_t GetResolveRenderTargetImagesHeight(IDObject<AutoCleanupResolveRenderTargetImage> imageID) const;

		uint32_t GetResolveRenderTargetImagesMemoryTypeMask(IDObject<AutoCleanupResolveRenderTargetImage> imageID) const;
		MemorySize GetResolveRenderTargetImagesSize(IDObject<AutoCleanupResolveRenderTargetImage> imageID) const;
		MemorySize GetResolveRenderTargetImagesRequiredAligment(IDObject<AutoCleanupResolveRenderTargetImage> imageID) const;

		uint32_t Get2DTextureImagesWidth(IDObject<AutoCleanup2DTexture> imageID) const;
		uint32_t Get2DTextureImagesHeight(IDObject<AutoCleanup2DTexture> imageID) const;

		uint32_t Get2DTextureImagesMemoryTypeMask(IDObject<AutoCleanup2DTexture> imageID) const;
		MemorySize Get2DTextureImagesSize(IDObject<AutoCleanup2DTexture> imageID) const;
		MemorySize Get2DTextureImagesRequiredAligment(IDObject<AutoCleanup2DTexture> imageID) const;

		uint32_t Get2DArrayTextureImagesWidth(IDObject<AutoCleanup2DArrayTexture> imageID) const;
		uint32_t Get2DArrayTextureImagesHeight(IDObject<AutoCleanup2DArrayTexture> imageID) const;

		uint32_t Get2DArrayTextureImagesMemoryTypeMask(IDObject<AutoCleanup2DArrayTexture> imageID) const;
		MemorySize Get2DArrayTextureImagesSize(IDObject<AutoCleanup2DArrayTexture> imageID) const;
		MemorySize Get2DArrayTextureImagesRequiredAligment(IDObject<AutoCleanup2DArrayTexture> imageID) const;

		void BindColorRenderTargetImage(IDObject<AutoCleanupColorRenderTargetImage> imageID, MemoryAllocationFullID allocationID,
			size_t addOnReserving = 0);
		void BindDepthStencilRenderTargetImage(IDObject<AutoCleanupDepthStencilRenderTargetImage> imageID, MemoryAllocationFullID allocationID,
			size_t addOnReserving = 0);
		void BindResolveRenderTargetImage(IDObject<AutoCleanupResolveRenderTargetImage> imageID, MemoryAllocationFullID allocationID,
			size_t addOnReserving = 0);

		void Bind2DTextureImage(IDObject<AutoCleanup2DTexture> imageID, MemoryAllocationFullID allocationID, size_t addOnReserving = 0);
		void Bind2DArrayTextureImage(IDObject<AutoCleanup2DArrayTexture> imageID, MemoryAllocationFullID allocationID, size_t addOnReserving = 0);

		IDObject<AutoCleanupImageView> AddColorRenderTargetImageView(IDObject<AutoCleanupColorRenderTargetImage> imageID,
			size_t addOnReserving = 0);
		IDObject<AutoCleanupImageView> AddDepthStencilRenderTargetImageView(
			IDObject<AutoCleanupDepthStencilRenderTargetImage> imageID, size_t addOnReserving = 0);
		IDObject<AutoCleanupImageView> AddResolveRenderTargetImageView(IDObject<AutoCleanupResolveRenderTargetImage> imageID,
			size_t addOnReserving = 0);

		IDObject<AutoCleanupImageView> Add2DTextureImageFullView(IDObject<AutoCleanup2DTexture> imageID, size_t addOnReserving = 0);
		IDObject<AutoCleanupImageView> Add2DTextureImageSelectedMipmapsView(IDObject<AutoCleanup2DTexture> imageID,
			uint32_t startingMipmap, std::optional<uint32_t> mipmapCount, size_t addOnReserving = 0);

		IDObject<AutoCleanupImageView> Add2DArrayTextureImageFullView(IDObject<AutoCleanup2DArrayTexture> imageID, size_t addOnReserving = 0);
		IDObject<AutoCleanupImageView> Add2DArrayTextureImageSelectedLayersAndMipmapsImageView(IDObject<AutoCleanup2DArrayTexture> imageID,
			uint32_t startingMipmap, std::optional<uint32_t> usedMipmapCount, uint32_t startingLayer, std::optional<uint32_t> usedLayerCount, size_t addOnReserving = 0);

		IDObject<AutoCleanupFramebuffer> AddFramebuffer(IDObject<AutoCleanupRenderPass> renderPass,
			const std::vector<std::pair<RenderTargetImagesID, IDObject<AutoCleanupImageView>>>& attachmentsList, uint32_t width, uint32_t height,
			uint32_t layers, size_t addOnReserving = 0);
		IDObject<AutoCleanupSampler> AddSampler(bool magFilterLinear, bool minFilterLinear, bool mipmapLinear, bool addressXMirrored, bool addressYMirrored,
			bool addressZMirrored, float mipmapBias, float maxAnisotropy, float minLod, std::optional<float> maxLod, size_t addOnReserving = 0);

		bool RemoveFramebuffer(IDObject<AutoCleanupFramebuffer> framebufferID, bool throwOnIDNotFound = true);
		bool RemoveSampler(IDObject<AutoCleanupSampler> samplerID, bool throwOnIDNotFound = false);

	private:
		ImageDataListsInternal& _internal;
	};
}