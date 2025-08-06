#pragma once

#include "../../../Include/Common/TextureArraySize.h"

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>

#include <stdint.h>
#include <array>

namespace JJs2DEngine
{
	class TextureDataMainInternal
	{
	public:
		TextureDataMainInternal(uint64_t transferFramesInFlight, const std::array<size_t, imagesInTextureArray>& preLoadedTexturesMaxAmounts,
			const std::array<size_t, imagesInTextureArray>& streamedTexturesMaxAmounts, VS::DataFormatSetIndependentID textureFormat);
		~TextureDataMainInternal();

	private:
		uint64_t _transferFramesInFlight;
		std::array<size_t, imagesInTextureArray> _preLoadedTexturesMaxAmounts;
		std::array<size_t, imagesInTextureArray> _streamedTexturesMaxAmounts;
		VS::DataFormatSetIndependentID _textureFormat;
	};
}
