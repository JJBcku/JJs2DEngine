#pragma once

#include "../../../Include/Common/TextureArraySize.h"
#include "../../../Include/Common/TextureFormatDef.h"

#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSImageDataLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>
#include <VulkanSimplified/VSDevice/VSSynchronizationDataLists.h>
#include <VulkanSimplified/VSDevice/VSCommandPoolQFGroupList.h>
#include <VulkanSimplified/VSDevice/VSDescriptorDataLists.h>
#include <VulkanSimplified/VSDevice/VSIRCommandPoolDef.h>
#include <VulkanSimplified/VSDevice/VSPrimaryIRCommandBufferDef.h>
#include <VulkanSimplified/VSDevice/VSSecondaryIRCommandBufferDef.h>
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

		IDObject<VS::AutoCleanupNIFDescriptorPool> GetTexturesDescriptorSetPool() const;
		IDObject<VS::AutoCleanupDescriptorSet> GetTexturesDescriptorSets(size_t currentTransferFrame);

		bool AreStreamedTextureCreated() const;

		std::vector<VS::ImagesMemoryBarrierData> GetPreLoadedTransferToGraphicsMemoryBarriers(uint64_t transferQueue, uint64_t graphicsQueue);
		std::vector<VS::ImagesMemoryBarrierData> GetStreamedTransferToGraphicsMemoryBarriers(size_t frameInFlightIndice, uint64_t transferQueue, uint64_t graphicsQueue);

		std::vector<VS::ImagesMemoryBarrierData> GetPreLoadedGraphicsToTransferMemoryBarriers(uint64_t transferQueue, uint64_t graphicsQueue);
		std::vector<VS::ImagesMemoryBarrierData> GetStreamedGraphicsToTransferMemoryBarriers(size_t frameInFlightIndice, uint64_t transferQueue, uint64_t graphicsQueue);

		std::optional<std::pair<size_t, size_t>> TryToAddTextureToPreloadedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height);
		std::optional<std::pair<size_t, size_t>> TryToAddTextureToStreamedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height);

		void TransferPreLoadedTexturesData(uint64_t transferQueue, uint64_t graphicsQueue);
		void TransferStreamedTexturesData(size_t frameInFlightIndice, uint64_t transferQueue, uint64_t graphicsQueue);

		IDObject<VS::AutoCleanupSemaphore> GetTransferFinishedSemaphore(size_t frameInFlightIndice) const;
		void SetTextureUseFinishedSemaphore(size_t frameInFlightIndice, IDObject<VS::AutoCleanupSemaphore> semaphore);

		bool PopTextureChangedValues(size_t frameInFlightIndice);

	private:
		VS::DataBufferLists _dataBufferList;
		VS::ImageDataLists _imageList;
		VS::MemoryObjectsList _memoryList;
		VS::SynchronizationDataLists _synchroList;
		VS::CommandPoolQFGroupList _transferQFGroup;
		VS::DescriptorDataLists _descriptorDataList;

		IDObject<VS::IRPoolPointer> _textureCommandPoolID;
		std::vector<IDObject<VS::PrimaryIRPointer>> _primaryCommandBufferIDs;

		IDObject<VS::SecondaryIRPointer> _preLoadedCommandBufferID;
		std::vector<IDObject<VS::SecondaryIRPointer>> _streamedCommandBufferIDs;

		std::vector<IDObject<VS::AutoCleanupFence>> _fenceList;
		std::vector<IDObject<VS::AutoCleanupSemaphore>> _transferSemaphoresList;
		std::vector<std::optional<IDObject<VS::AutoCleanupSemaphore>>> _textureBeingUsedSemaphores;

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
