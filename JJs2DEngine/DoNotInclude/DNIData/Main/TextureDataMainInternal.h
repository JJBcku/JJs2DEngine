#pragma once

#include "../../../Include/Common/TextureArraySize.h"
#include "../../../Include/Common/TextureFormatDef.h"

#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSImageDataLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>
#include <VulkanSimplified/VSDevice/VSCommandPoolQFGroupList.h>
#include <VulkanSimplified/VSDevice/VSNIRCommandPoolDef.h>
#include <VulkanSimplified/VSDevice/VSPrimaryNIRCommandBufferDef.h>
#include <VulkanSimplified/VSDevice/VSSecondaryNIRCommandBufferDef.h>

#include <CustomLists/IDObject.h>

#include <stdint.h>
#include <array>
#include <memory>
#include <vector>

namespace JJs2DEngine
{
	class TextureDataFrameInternal;

	struct TextureDataMainInitData
	{
		uint64_t transferQueueID;

		uint64_t transferFramesInFlight;
		uint64_t max2DImageSize;
		uint64_t maxImageArrayLayers;

		std::string dataFolder;
		std::array<size_t, imagesInTextureArray> preLoadedTexturesMaxAmounts;
		std::array<size_t, imagesInTextureArray> streamedTexturesMaxAmounts;

		size_t preLoadedTexturesStagingBufferPageCount;
		size_t streamedTexturesStagingBufferPageCount;
		TextureFormat textureFormat;

		TextureDataMainInitData();
	};

	class TextureDataMainInternal
	{
	public:
		TextureDataMainInternal(const TextureDataMainInitData& initData, VS::DataBufferLists dataBufferList, VS::ImageDataLists imageList, VS::MemoryObjectsList memoryList,
			VS::CommandPoolQFGroupList transferQFGroup);
		~TextureDataMainInternal();

	private:
		VS::DataBufferLists _dataBufferList;
		VS::ImageDataLists _imageList;
		VS::MemoryObjectsList _memoryList;
		VS::CommandPoolQFGroupList _transferQFGroup;

		IDObject<VS::NIRPoolPointer> _textureCommandPoolID;
		IDObject<VS::PrimaryNIRPointer> _primaryCommandBufferID;

		IDObject<VS::SecondaryNIRPointer> _preLoadedCommandBufferID;

		std::unique_ptr<TextureDataFrameInternal> _preLoadedTexturesData;

		bool Is16Bit(TextureFormat textureFormat) const;
		bool IsRBReversed(TextureFormat textureFormat) const;

		std::vector<unsigned char> LoadDefautTexture8Bit(std::string dataFolder, uint32_t tileSize, bool RBreversed) const;
		std::vector<unsigned char> LoadDefautTexture16Bit(std::string dataFolder, uint32_t tileSize, bool RBreversed) const;
	};
}
