#pragma once

#include "../../../Include/Common/TextureArraySize.h"

#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSImageDataLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>

#include <stdint.h>

namespace JJs2DEngine
{
	struct TextureFrameImageData
	{
		size_t tileSize;
		size_t widthInTiles;
		size_t heightInTiles;
		size_t widthInPixels;
		size_t heightInPixels;
		size_t layers;

		TextureFrameImageData();
		~TextureFrameImageData();
	};

	class TextureDataFrameInternal
	{
	public:
		TextureDataFrameInternal(uint64_t startingIndex, uint64_t max2DImageSize, uint64_t maxImageArrayLayers, const std::array<size_t, imagesInTextureArray>& texturesMaxAmounts,
			VS::DataBufferLists dataBufferList, VS::ImageDataLists imageList, VS::MemoryObjectsList memoryList);
		~TextureDataFrameInternal();

	private:
		VS::DataBufferLists _dataBufferList;
		VS::ImageDataLists _imageList;
		VS::MemoryObjectsList _memoryList;

		uint64_t _startingIndex;
		uint64_t _max2DImageSize;
		uint64_t _maxImageArrayLayers;

		std::array<TextureFrameImageData, imagesInTextureArray> _textureDataArray;

		TextureFrameImageData CompileTextureFrameSizeData(size_t tileSize, size_t texturesMaxAmount, uint64_t max2DImageSize, uint64_t maxImageArrayLayers) const;
	};
}