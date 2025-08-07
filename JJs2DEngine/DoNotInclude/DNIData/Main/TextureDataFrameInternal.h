#pragma once

#include "../../../Include/Common/TextureArraySize.h"

#include <stdint.h>

namespace JJs2DEngine
{
	class TextureDataFrameInternal
	{
	public:
		TextureDataFrameInternal(uint32_t startingIndex, const std::array<size_t, imagesInTextureArray>& texturesMaxAmounts);
		~TextureDataFrameInternal();

	private:
		uint32_t _startingIndex;
		uint32_t _padding;
	};
}