#pragma once

#include "../../../Include/Common/TextureArraySize.h"
#include "../../../Include/Common/TextureFormatDef.h"

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>

#include <stdint.h>
#include <array>

namespace JJs2DEngine
{
	class TextureDataMainInternal
	{
	public:
		TextureDataMainInternal(uint64_t transferFramesInFlight, const std::array<size_t, imagesInTextureArray>& preLoadedTexturesMaxAmounts,
			const std::array<size_t, imagesInTextureArray>& streamedTexturesMaxAmounts, TextureFormat textureFormat);
		~TextureDataMainInternal();

	private:
		uint64_t _transferFramesInFlight;
		std::array<size_t, imagesInTextureArray> _preLoadedTexturesMaxAmounts;
		std::array<size_t, imagesInTextureArray> _streamedTexturesMaxAmounts;

		bool Is16Bit(TextureFormat textureFormat) const;
	};
}
