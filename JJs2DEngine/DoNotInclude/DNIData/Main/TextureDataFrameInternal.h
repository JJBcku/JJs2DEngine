#pragma once

#include "../../../Include/Common/TextureArraySize.h"
#include "../../../Include/Common/TextureReferenceData.h"

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>
#include <VulkanSimplified/VSCommon/VSMemoryAllocationFullID.h>

#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSImageDataLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>
#include <VulkanSimplified/VSDevice/VSSecondaryIRCommandBuffer.h>

#include <CustomLists/IDObject.h>

#include <stdint.h>
#include <vector>
#include <array>
#include <memory>
#include <compare>

namespace JJs2DEngine
{
	struct TextureTransferOrderInternal;

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
		~TextureFrameInitData();
	};

	struct TextureFrameStagingBufferData
	{
		IDObject<VS::AutoCleanupStagingBuffer> stagingBufferID;
		uint64_t totalBufferSize;
		uint64_t currentlyUsedSize;

		TextureFrameStagingBufferData();
		~TextureFrameStagingBufferData();
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

		std::vector<std::vector<std::shared_ptr<TextureReferenceData>>> textureReferencesList;
		std::vector<std::vector<TextureTransferOrderInternal>> textureTransferOrderLists;

		TextureReferenceData defaultReference;

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
			const std::vector<VS::SecondaryIRCommandBuffer>& commandBuffersList);
		~TextureDataFrameInternal();

		void LoadDefaultTextures(const std::array<std::vector<unsigned char>, imagesInTextureArray>& defaultTexturesData, uint64_t transferQueue, uint64_t graphicsQueue);

		std::array<IDObject<VS::AutoCleanup2DArrayTexture>, imagesInTextureArray> GetImageIDs(size_t frameInFlightIndice) const;
		std::array<IDObject<VS::AutoCleanupImageView>, imagesInTextureArray> GetImageViewIDs(size_t frameInFlightIndice) const;

		std::vector<std::shared_ptr<TextureReferenceData>> GetTextureReference(size_t tileImageIndex, size_t referenceIndex);

		void GetGraphicsToTransferMemoryBarriers(std::vector<VS::ImagesMemoryBarrierData>& outputVector, size_t frameInFlightIndice, uint64_t transferQueue, uint64_t graphicsQueue);
		void GetTransferToGraphicsMemoryBarriers(std::vector<VS::ImagesMemoryBarrierData>& outputVector, size_t frameInFlightIndice, uint64_t transferQueue, uint64_t graphicsQueue);

		std::optional<std::pair<size_t, size_t>> TryToAddTextureToTransferList(const unsigned char& data, size_t dataSize, uint32_t width, uint32_t height);
		void RecordTransferBuffer(size_t frameInFlightIndice, uint64_t transferQueue, uint64_t graphicsQueue);
		void FinishTextureTransfer(size_t frameInFlightIndice);

	private:
		VS::DataBufferLists _dataBufferList;
		VS::ImageDataLists _imageList;
		VS::MemoryObjectsList _memoryList;
		std::vector<VS::SecondaryIRCommandBuffer> _commandBuffersList;

		uint64_t _startingIndex;
		uint64_t _max2DImageSize;
		uint64_t _maxImageArrayLayers;

		std::array<TextureFrameImageData, imagesInTextureArray> _textureDataArray;
		VS::MemoryAllocationFullID _textureMemoryID;

		std::vector<TextureFrameStagingBufferData> _texturesStagingBufferFrames;
		VS::MemoryAllocationFullID _stagingBufferMemoryID;

		TextureFrameImageData CompileTextureFrameSizeData(size_t tileSize, size_t texturesMaxAmount, uint64_t max2DImageSize, uint64_t maxImageArrayLayers) const;

		uint64_t GetLeastStagingMemoryUnused() const;
	};
}