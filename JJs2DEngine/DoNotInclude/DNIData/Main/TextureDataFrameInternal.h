#pragma once

#include "../../../Include/Common/TextureArraySize.h"
#include "../../../Include/Common/TextureReferenceData.h"

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>
#include <VulkanSimplified/VSCommon/VSMemoryAllocationFullID.h>

#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSImageDataLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>
#include <VulkanSimplified/VSDevice/VSSecondaryNIRCommandBuffer.h>

#include <CustomLists/IDObject.h>

#include <stdint.h>
#include <vector>
#include <array>
#include <memory>
#include <compare>

namespace JJs2DEngine
{
	struct TextureFrameInitData
	{
		uint64_t frameAmount;
		uint64_t startingIndex;
		uint64_t max2DImageSize;
		uint64_t maxImageArrayLayers;
		std::array<size_t, imagesInTextureArray> texturesMaxAmounts;

		size_t stagingBufferSize;
		VS::DataFormatSetIndependentID textureFormat;

		TextureFrameInitData();
	};

	struct TextureFrameImageData
	{
		size_t tileSize;
		size_t widthInTiles;
		size_t heightInTiles;
		size_t widthInPixels;
		size_t heightInPixels;
		size_t layers;

		std::vector<IDObject<VS::AutoCleanup2DArrayTexture>> imageIDs;
		std::vector<IDObject<VS::AutoCleanupImageView>> imageViewIDs;

		std::vector<std::shared_ptr<TextureReferenceData>> textureReferencesList;

		TextureFrameImageData();
		~TextureFrameImageData();

		TextureFrameImageData(const TextureFrameImageData& rhs) noexcept = default;
		TextureFrameImageData(TextureFrameImageData&& rhs) noexcept = default;

		TextureFrameImageData& operator=(const TextureFrameImageData& rhs) noexcept = default;
		TextureFrameImageData& operator=(TextureFrameImageData&& rhs) noexcept = default;

		std::strong_ordering operator<=>(const TextureFrameImageData& rhs) const noexcept = default;
		bool operator==(const TextureFrameImageData& rhs) const noexcept = default;
	};

	class TextureDataFrameInternal
	{
	public:
		TextureDataFrameInternal(const TextureFrameInitData& initData, VS::DataBufferLists dataBufferList, VS::ImageDataLists imageList, VS::MemoryObjectsList memoryList,
			VS::SecondaryNIRCommandBuffer commandBuffer);
		~TextureDataFrameInternal();

		void LoadDefaultTextures(const std::array<std::vector<unsigned char>, imagesInTextureArray>& defaultTexturesData, uint64_t transferQueue, uint64_t graphicsQueue);

		std::array<IDObject<VS::AutoCleanup2DArrayTexture>, imagesInTextureArray> GetImageIDs(size_t frameInFlightIndice) const;
		std::array<IDObject<VS::AutoCleanupImageView>, imagesInTextureArray> GetImageViewIDs(size_t frameInFlightIndice) const;

		std::shared_ptr<TextureReferenceData> GetTextureReference(size_t tileImageIndex, size_t referenceIndex);

		void GetTransferToGraphicsMemoryBarriers(std::vector<VS::ImagesMemoryBarrierData>& outputVector, size_t frameInFlightIndice, uint64_t transferQueue, uint64_t graphicsQueue);

	private:
		VS::DataBufferLists _dataBufferList;
		VS::ImageDataLists _imageList;
		VS::MemoryObjectsList _memoryList;
		VS::SecondaryNIRCommandBuffer _commandBuffer;

		uint64_t _startingIndex;
		uint64_t _max2DImageSize;
		uint64_t _maxImageArrayLayers;

		std::array<TextureFrameImageData, imagesInTextureArray> _textureDataArray;
		VS::MemoryAllocationFullID _textureMemoryID;

		std::vector<IDObject<VS::AutoCleanupStagingBuffer>> _texturesStagingBufferIDs;
		VS::MemoryAllocationFullID _stagingBufferMemoryID;

		TextureFrameImageData CompileTextureFrameSizeData(size_t tileSize, size_t texturesMaxAmount, uint64_t max2DImageSize, uint64_t maxImageArrayLayers) const;
	};
}