#pragma once

#include "../../../Include/Common/TextureArraySize.h"
#include "../../../Include/Common/TextureFormatDef.h"

#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSImageDataLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>
#include <VulkanSimplified/VSDevice/VSSynchronizationDataLists.h>
#include <VulkanSimplified/VSDevice/VSCommandPoolQFGroupList.h>
#include <VulkanSimplified/VSDevice/VSDescriptorDataLists.h>
#include <VulkanSimplified/VSDevice/VSNIRCommandPoolDef.h>
#include <VulkanSimplified/VSDevice/VSPrimaryNIRCommandBufferDef.h>
#include <VulkanSimplified/VSDevice/VSSecondaryNIRCommandBufferDef.h>
#include <VulkanSimplified/VSDevice/VSImagesMemoryBarrierData.h>

#include <CustomLists/IDObject.h>

#include <Miscellaneous/Bool64Def.h>

#include <stdint.h>
#include <array>
#include <memory>
#include <vector>
#include <utility>
#include <optional>

namespace JJs2DEngine
{
	class TextureDataFrameInternal;

	struct TextureReferenceData;

	struct TextureDataMainInitData
	{
		uint64_t transferQueueID;
		uint64_t graphicsQueueID;

		uint64_t transferFramesInFlight;
		uint64_t max2DImageSize;
		uint64_t maxImageArrayLayers;

		std::string dataFolder;
		std::array<size_t, imagesInTextureArray> preLoadedTexturesMaxAmounts;
		std::array<size_t, imagesInTextureArray> streamedTexturesMaxAmounts;

		size_t preLoadedTexturesStagingBufferPageCount;
		size_t streamedTexturesStagingBufferPageCount;
		TextureFormat textureFormat;

		IDObject<VS::AutoCleanupDescriptorSetLayout> textureDescriptorSetLayout;

		TextureDataMainInitData();
	};

	class TextureDataMainInternal
	{
	public:
		TextureDataMainInternal(const TextureDataMainInitData& initData, VS::DataBufferLists dataBufferList, VS::ImageDataLists imageList, VS::MemoryObjectsList memoryList,
			VS::SynchronizationDataLists synchroList, VS::CommandPoolQFGroupList transferQFGroup, VS::DescriptorDataLists descriptorDataList);
		~TextureDataMainInternal();

		std::vector<std::shared_ptr<TextureReferenceData>> GetTextureReference(bool inPreloadedTexturesList, size_t tileImageIndex, size_t referenceIndex);

		IDObject<VS::AutoCleanupNIFDescriptorPool> GetTexturesDescriptorSetPool();
		IDObject<VS::AutoCleanupDescriptorSet> GetTexturesDescriptorSets(size_t currentTransferFrame);

		bool AreStreamedTextureCreated() const;

		std::vector<VS::ImagesMemoryBarrierData> GetPreLoadedTransferToGraphicsMemoryBarriers(uint64_t transferQueue, uint64_t graphicsQueue);
		std::vector<VS::ImagesMemoryBarrierData> GetStreamedTransferToGraphicsMemoryBarriers(size_t frameInFlightIndice, uint64_t transferQueue, uint64_t graphicsQueue);

		std::optional<std::pair<size_t, size_t>> TryToAddTextureToPreloadedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height);
		std::optional<std::pair<size_t, size_t>> TryToAddTextureToStreamedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height);

	private:
		VS::DataBufferLists _dataBufferList;
		VS::ImageDataLists _imageList;
		VS::MemoryObjectsList _memoryList;
		VS::SynchronizationDataLists _synchroList;
		VS::CommandPoolQFGroupList _transferQFGroup;
		VS::DescriptorDataLists _descriptorDataList;

		IDObject<VS::NIRPoolPointer> _textureCommandPoolID;
		IDObject<VS::PrimaryNIRPointer> _primaryCommandBufferID;

		IDObject<VS::SecondaryNIRPointer> _preLoadedCommandBufferID;
		IDObject<VS::SecondaryNIRPointer> _streamedCommandBufferID;

		std::vector<IDObject<VS::AutoCleanupFence>> _fenceList;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _transferSemaphoresList;

		std::unique_ptr<TextureDataFrameInternal> _preLoadedTexturesData;
		std::unique_ptr<TextureDataFrameInternal> _streamedTexturesData;

		IDObject<VS::AutoCleanupNIFDescriptorPool> _transferDescriptorPool;
		std::vector<IDObject<VS::AutoCleanupDescriptorSet>> _textureDescriptorSets;

		Misc::Bool64Values _textureFormatIs16Bit;
		Misc::Bool64Values _textureFormatIsRBReversed;

		bool Is16Bit(TextureFormat textureFormat) const;
		bool IsRBReversed(TextureFormat textureFormat) const;

		std::vector<unsigned char> LoadDefautTexture8Bit(std::string dataFolder, uint32_t tileSize, bool RBreversed) const;
		std::vector<unsigned char> LoadDefautTexture16Bit(std::string dataFolder, uint32_t tileSize, bool RBreversed) const;
	};
}
