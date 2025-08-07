#pragma once

#include "../../../Include/Common/TextureArraySize.h"
#include "../../../Include/Common/TextureFormatDef.h"

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>

#include <stdint.h>
#include <array>
#include <memory>

namespace JJs2DEngine
{
	class TextureDataFrameInternal;

	class TextureDataMainInternal
	{
	public:
		TextureDataMainInternal(uint64_t transferFramesInFlight, uint64_t max2DImageSize, uint64_t maxImageArrayLayers,
			const std::array<size_t, imagesInTextureArray>& preLoadedTexturesMaxAmounts,
			const std::array<size_t, imagesInTextureArray>& streamedTexturesMaxAmounts, TextureFormat textureFormat);
		~TextureDataMainInternal();

	private:
		std::unique_ptr<TextureDataFrameInternal> _preLoadedTexturesData;

		bool Is16Bit(TextureFormat textureFormat) const;
	};
}
