#include "MainDNIpch.h"
#include "TextureDataMainInternal.h"

#include "../../../Include/Common/TextureFormat.h"

#include "TextureDataFrameInternal.h"
#include "DeviceSettingsInternal.h"

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>

namespace JJs2DEngine
{
	TextureDataMainInternal::TextureDataMainInternal(uint64_t transferFramesInFlight, uint64_t max2DImageSize, uint64_t maxImageArrayLayers,
		const std::array<size_t, imagesInTextureArray>& preLoadedTexturesMaxAmounts, const std::array<size_t, imagesInTextureArray>& streamedTexturesMaxAmounts,
		TextureFormat textureFormat, VS::DataBufferLists dataBufferList, VS::ImageDataLists imageList, VS::MemoryObjectsList memoryList) :
		_dataBufferList(dataBufferList), _imageList(imageList), _memoryList(memoryList)
	{
		std::array<size_t, imagesInTextureArray> _preLoadedTexturesMaxAmounts = preLoadedTexturesMaxAmounts;
		std::array<size_t, imagesInTextureArray> _streamedTexturesMaxAmounts = streamedTexturesMaxAmounts;

		for (size_t i = 0; i < _preLoadedTexturesMaxAmounts.size(); ++i)
		{
			_preLoadedTexturesMaxAmounts[i] += 1;
		}

		for (size_t i = 0; i < _streamedTexturesMaxAmounts.size(); ++i)
		{
			_streamedTexturesMaxAmounts[i] += 1;
		}

		VS::DataFormatSetIndependentID format = TranslateToFormat(textureFormat);

		_preLoadedTexturesData = std::make_unique<TextureDataFrameInternal>(0ULL, max2DImageSize, maxImageArrayLayers, _preLoadedTexturesMaxAmounts, format,
			_dataBufferList, _imageList, _memoryList);

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

}