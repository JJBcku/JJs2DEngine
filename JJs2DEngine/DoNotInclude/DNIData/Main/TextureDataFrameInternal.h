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

		IDObject<VS::AutoCleanup2DArrayTexture> imageID;
		IDObject<VS::AutoCleanupImageView> imageViewID;

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

		IDObject<VS::AutoCleanupStagingBuffer> _texturesStagingBufferID;
		VS::MemoryAllocationFullID _stagingBufferMemoryID;

		TextureFrameImageData CompileTextureFrameSizeData(size_t tileSize, size_t texturesMaxAmount, uint64_t max2DImageSize, uint64_t maxImageArrayLayers) const;
	};
}