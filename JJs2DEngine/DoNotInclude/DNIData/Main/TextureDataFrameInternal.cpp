#include "MainDNIpch.h"
#include "TextureDataFrameInternal.h"

#include <limits>

namespace JJs2DEngine
{
	JJs2DEngine::TextureFrameImageData::TextureFrameImageData()
	{
		tileSize = 0;
		widthInTiles = 0;
		heightInTiles = 0;
		widthInPixels = 0;
		heightInPixels = 0;
		layers = 0;
	}

	TextureFrameImageData::~TextureFrameImageData()
	{
	}

	TextureDataFrameInternal::TextureDataFrameInternal(uint64_t startingIndex, uint64_t max2DImageSize, uint64_t maxImageArrayLayers,
		const std::array<size_t, imagesInTextureArray>& texturesMaxAmounts, VS::DataFormatSetIndependentID textureFormat,
		VS::DataBufferLists dataBufferList, VS::ImageDataLists imageList, VS::MemoryObjectsList memoryList) :
		_dataBufferList(dataBufferList), _imageList(imageList), _memoryList(memoryList), _startingIndex(startingIndex), _max2DImageSize(max2DImageSize),
		_maxImageArrayLayers(maxImageArrayLayers)
	{
		uint32_t memoryTypeMask = std::numeric_limits<uint32_t>::max();
		size_t totalSize = 0;

		for (size_t i = 0; i < _textureDataArray.size(); ++i)
		{
			auto& textureData = _textureDataArray[i];

			size_t tileSize = 1ULL << (skippedSizeLevels + i);
			textureData = CompileTextureFrameSizeData(tileSize, texturesMaxAmounts[i], max2DImageSize, maxImageArrayLayers);

			textureData.imageID = _imageList.Add2DArrayTextureImage(static_cast<uint32_t>(textureData.widthInPixels), static_cast<uint32_t>(textureData.heightInPixels),
				static_cast<uint32_t>(textureData.layers), 1, textureFormat, {}, false, 1);
			memoryTypeMask = memoryTypeMask & _imageList.Get2DArrayTextureImagesMemoryTypeMask(textureData.imageID);
			totalSize += _imageList.Get2DArrayTextureImagesSize(textureData.imageID);
		}
	}

	TextureDataFrameInternal::~TextureDataFrameInternal()
	{
	}

	TextureFrameImageData TextureDataFrameInternal::CompileTextureFrameSizeData(size_t tileSize, size_t texturesMaxAmount, uint64_t max2DImageSize, uint64_t maxImageArrayLayers) const
	{
		TextureFrameImageData ret;

		ret.tileSize = tileSize;

		size_t stripeMaxCount = max2DImageSize / ret.tileSize;
		size_t layerMaxTilesCount = stripeMaxCount * stripeMaxCount;

		bool found = false;

		if (!found && texturesMaxAmount <= stripeMaxCount)
		{
			ret.widthInTiles = texturesMaxAmount;
			ret.heightInTiles = 1;
			ret.widthInPixels = ret.widthInTiles * ret.tileSize;
			ret.heightInPixels = ret.tileSize;
			ret.layers = 1;
			found = true;
		}

		if (!found && texturesMaxAmount <= maxImageArrayLayers)
		{
			ret.widthInTiles = 1;
			ret.heightInTiles = 1;
			ret.widthInPixels = ret.tileSize;
			ret.heightInPixels = ret.tileSize;
			ret.layers = texturesMaxAmount;
			found = true;
		}

		size_t stripeCount = texturesMaxAmount / stripeMaxCount;
		size_t stripeCountMod = texturesMaxAmount % stripeMaxCount;

		if (!found && stripeCountMod == 0 && (stripeCount <= stripeMaxCount || stripeCount <= maxImageArrayLayers))
		{
			ret.widthInTiles = stripeMaxCount;
			ret.widthInPixels = stripeMaxCount * ret.tileSize;
			if (stripeCount <= stripeMaxCount)
			{
				ret.heightInTiles = stripeCount;
				ret.heightInPixels = ret.heightInTiles * ret.tileSize;
				ret.layers = 1;
			}
			else
			{
				ret.heightInTiles = 1;
				ret.heightInPixels = ret.tileSize;
				ret.layers = stripeCount;
			}
			found = true;
		}

		if (!found)
		{
			for (size_t layersInTexture = 2; layersInTexture < maxImageArrayLayers; ++layersInTexture)
			{
				size_t imagesInLayer = texturesMaxAmount / layersInTexture;
				size_t imagesInLayerMod = texturesMaxAmount % layersInTexture;

				if (imagesInLayerMod != 0)
					continue;

				for (size_t heightTiles = 2; heightTiles <= imagesInLayer / 2; ++heightTiles)
				{
					if (heightTiles > stripeMaxCount)
						break;

					size_t widthTiles = imagesInLayer / heightTiles;
					size_t widthTilesMod = imagesInLayer % heightTiles;

					if (widthTilesMod == 0 && widthTiles <= stripeMaxCount)
					{
						ret.widthInTiles = widthTiles;
						ret.heightInTiles = heightTiles;
						ret.widthInPixels = widthTiles * ret.tileSize;
						ret.heightInPixels = heightTiles * ret.tileSize;
						ret.layers = layersInTexture;

						found = true;
						break;
					}
				}

				if (found)
					break;
			}
		}

		if (!found)
		{
			size_t oversizedTileStripesCount = (texturesMaxAmount / stripeMaxCount) + 1;

			if (!found && oversizedTileStripesCount <= stripeMaxCount)
			{
				ret.widthInTiles = stripeMaxCount;
				ret.heightInTiles = oversizedTileStripesCount;
				ret.widthInPixels = stripeMaxCount * ret.tileSize;
				ret.heightInPixels = oversizedTileStripesCount * ret.tileSize;
				ret.layers = 1;
				found = true;
			}

			if (!found && oversizedTileStripesCount <= maxImageArrayLayers)
			{
				ret.widthInTiles = stripeMaxCount;
				ret.heightInTiles = 1;
				ret.widthInPixels = stripeMaxCount * ret.tileSize;
				ret.heightInPixels = ret.tileSize;
				ret.layers = oversizedTileStripesCount;
				found = true;
			}

			size_t oversizedLayerCount = (texturesMaxAmount / layerMaxTilesCount) + 1;
			if (!found && oversizedLayerCount <= maxImageArrayLayers)
			{
				ret.widthInTiles = stripeMaxCount;
				ret.heightInTiles = stripeMaxCount;
				ret.widthInPixels = stripeMaxCount * ret.tileSize;
				ret.heightInPixels = ret.widthInPixels;
				ret.layers = oversizedLayerCount;
				found = true;
			}
		}

		if (!found)
			throw std::runtime_error("TextureDataFrameInternal::CompileTextureFrameSizeData Error: Program failed to find the proper size for an image!");

		return ret;
	}

}