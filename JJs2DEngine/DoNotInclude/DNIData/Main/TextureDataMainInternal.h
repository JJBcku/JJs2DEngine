#pragma once

#include "../../../Include/Common/TextureArraySize.h"
#include "../../../Include/Common/TextureFormatDef.h"

#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSImageDataLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>

#include <stdint.h>
#include <array>
#include <memory>
#include <vector>

namespace JJs2DEngine
{
	class TextureDataFrameInternal;

	class TextureDataMainInternal
	{
	public:
		TextureDataMainInternal(uint64_t transferFramesInFlight, uint64_t max2DImageSize, uint64_t maxImageArrayLayers, std::string dataFolder,
			const std::array<size_t, imagesInTextureArray>& preLoadedTexturesMaxAmounts, const std::array<size_t, imagesInTextureArray>& streamedTexturesMaxAmounts,
			size_t preLoadedTexturesStagingBufferPageCount, size_t streamedTexturesStagingBufferPageCount, TextureFormat textureFormat,
			VS::DataBufferLists dataBufferList, VS::ImageDataLists imageList, VS::MemoryObjectsList memoryList);
		~TextureDataMainInternal();

	private:
		VS::DataBufferLists _dataBufferList;
		VS::ImageDataLists _imageList;
		VS::MemoryObjectsList _memoryList;

		std::unique_ptr<TextureDataFrameInternal> _preLoadedTexturesData;

		bool Is16Bit(TextureFormat textureFormat) const;
		bool IsRBReversed(TextureFormat textureFormat) const;

		std::vector<unsigned char> LoadDefautTexture8Bit(std::string dataFolder, uint32_t tileSize, bool RBreversed) const;
		std::vector<unsigned char> LoadDefautTexture16Bit(std::string dataFolder, uint32_t tileSize, bool RBreversed) const;
	};
}
