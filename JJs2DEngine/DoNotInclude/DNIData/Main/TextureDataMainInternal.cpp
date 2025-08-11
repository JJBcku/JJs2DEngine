#include "MainDNIpch.h"
#include "TextureDataMainInternal.h"

#include "../../../Include/Common/TextureFormat.h"

#include "TextureDataFrameInternal.h"
#include "DeviceSettingsInternal.h"

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace JJs2DEngine
{
	TextureDataMainInitData::TextureDataMainInitData() : preLoadedTexturesMaxAmounts(), streamedTexturesMaxAmounts()
	{
		transferFramesInFlight = 0;
		max2DImageSize = 0;
		maxImageArrayLayers = 0;

		preLoadedTexturesStagingBufferPageCount = 0;
		streamedTexturesStagingBufferPageCount = 0;
		textureFormat = TextureFormat::TEXTURE_FORMAT_UNSET;
	}

	TextureDataMainInternal::TextureDataMainInternal(const TextureDataMainInitData& initData, VS::DataBufferLists dataBufferList, VS::ImageDataLists imageList,
		VS::MemoryObjectsList memoryList) : _dataBufferList(dataBufferList), _imageList(imageList), _memoryList(memoryList)
	{
		std::array<size_t, imagesInTextureArray> _preLoadedTexturesMaxAmounts = initData.preLoadedTexturesMaxAmounts;
		std::array<size_t, imagesInTextureArray> _streamedTexturesMaxAmounts = initData.streamedTexturesMaxAmounts;

		for (size_t i = 0; i < _preLoadedTexturesMaxAmounts.size(); ++i)
		{
			_preLoadedTexturesMaxAmounts[i] += 1;
		}

		for (size_t i = 0; i < _streamedTexturesMaxAmounts.size(); ++i)
		{
			_streamedTexturesMaxAmounts[i] += 1;
		}

		VS::DataFormatSetIndependentID format = TranslateToFormat(initData.textureFormat);

		bool is16Bit = Is16Bit(initData.textureFormat);
		bool isRBReversed = IsRBReversed(initData.textureFormat);

		size_t preLoadedTexturesStagingBuferSize = biggestLevelTilePixelCount * initData.preLoadedTexturesStagingBufferPageCount;

		if (is16Bit)
			preLoadedTexturesStagingBuferSize *= 8;
		else
			preLoadedTexturesStagingBuferSize *= 4;

		TextureFrameInitData frameInitData;
		frameInitData.startingIndex = 0ULL;
		frameInitData.max2DImageSize = initData.max2DImageSize;
		frameInitData.maxImageArrayLayers = initData.maxImageArrayLayers;
		frameInitData.texturesMaxAmounts = _preLoadedTexturesMaxAmounts;
		frameInitData.stagingBufferSize = preLoadedTexturesStagingBuferSize;
		frameInitData.textureFormat = format;

		_preLoadedTexturesData = std::make_unique<TextureDataFrameInternal>(frameInitData, _dataBufferList, _imageList, _memoryList);

		std::array<std::vector<unsigned char>, imagesInTextureArray> _defaultTextureDataList;

		if (is16Bit)
			for (size_t i = 0; i < _defaultTextureDataList.size(); ++i)
			{
				_defaultTextureDataList[i] = LoadDefautTexture16Bit(initData.dataFolder, 1U << (skippedSizeLevels + i), isRBReversed);
			}
		else
			for (size_t i = 0; i < _defaultTextureDataList.size(); ++i)
			{
				_defaultTextureDataList[i] = LoadDefautTexture8Bit(initData.dataFolder, 1U << (skippedSizeLevels + i), isRBReversed);
			}
	}

	TextureDataMainInternal::~TextureDataMainInternal()
	{
	}

	bool TextureDataMainInternal::Is16Bit(TextureFormat textureFormat) const
	{
		if (textureFormat == TextureFormat::TEXTURE_FORMAT_RGBA16)
			return true;

		return false;
	}

	bool TextureDataMainInternal::IsRBReversed(TextureFormat textureFormat) const
	{
		if (textureFormat == TextureFormat::TEXTURE_FORMAT_BGRA8)
			return true;

		return false;
	}

	static const std::string defaultTexturesFolderName("DefaultTextures\\");
	static const std::string defaultTexturesName("DefaultTexture");
	static const std::string defaultTexturesExtension(".png");

	static const std::string RGBTexturesName("RGB");
	static const std::string BGRTexturesName("BGR");

	static const std::string eigthBitTexturesName("8");
	static const std::string sixteenBitTexturesName("16");

	std::vector<unsigned char> TextureDataMainInternal::LoadDefautTexture8Bit(std::string dataFolder, uint32_t tileSize, bool RBreversed) const
	{
		std::vector<unsigned char> ret;
		
		std::string filepath = dataFolder + defaultTexturesFolderName + defaultTexturesName;
		filepath += std::to_string(tileSize);

		if (RBreversed)
		{
			filepath += BGRTexturesName;
		}
		else
		{
			filepath += RGBTexturesName;
		}

		filepath += eigthBitTexturesName;
		filepath += defaultTexturesExtension;

		int width = 0;
		int height = 0;
		int channels = 0;

		stbi_uc* pixels = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (pixels == nullptr)
			throw std::runtime_error("TextureDataMainInternal::LoadDefautTexture8Bit Error: Program failed to read a default texture!");

		if (static_cast<uint32_t>(width) != tileSize || static_cast<uint32_t>(height) != tileSize)
			throw std::runtime_error("TextureDataMainInternal::LoadDefautTexture8Bit Error: Program read an incorrect default texture!");

		size_t imageSize = static_cast<size_t>(tileSize) * tileSize * 4;

		ret.resize(imageSize);
		std::memcpy(ret.data(), pixels, imageSize);

		return ret;
	}

	std::vector<unsigned char> TextureDataMainInternal::LoadDefautTexture16Bit(std::string dataFolder, uint32_t tileSize, bool RBreversed) const
	{
		std::vector<unsigned char> ret;

		std::string filepath = dataFolder + defaultTexturesFolderName + defaultTexturesName;
		filepath += std::to_string(tileSize);

		if (RBreversed)
		{
			filepath += BGRTexturesName;
		}
		else
		{
			filepath += RGBTexturesName;
		}

		filepath += sixteenBitTexturesName;
		filepath += defaultTexturesExtension;

		int width = 0;
		int height = 0;
		int channels = 0;

		stbi_us* pixels = stbi_load_16(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (pixels == nullptr)
			throw std::runtime_error("TextureDataMainInternal::LoadDefautTexture8Bit Error: Program failed to read a default texture!");

		if (static_cast<uint32_t>(width) != tileSize || static_cast<uint32_t>(height) != tileSize)
			throw std::runtime_error("TextureDataMainInternal::LoadDefautTexture8Bit Error: Program read an incorrect default texture!");

		size_t imageSize = static_cast<size_t>(tileSize) * tileSize * 8;

		ret.resize(imageSize);
		std::memcpy(ret.data(), pixels, imageSize);

		return ret;
	}

}