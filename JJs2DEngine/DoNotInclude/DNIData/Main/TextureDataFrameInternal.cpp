#include "MainDNIpch.h"
#include "TextureDataFrameInternal.h"

namespace JJs2DEngine
{
	TextureDataFrameInternal::TextureDataFrameInternal(uint32_t startingIndex, const std::array<size_t, imagesInTextureArray>& texturesMaxAmounts) :
		_startingIndex(startingIndex), _padding(0)
	{
	}

	TextureDataFrameInternal::~TextureDataFrameInternal()
	{
	}

}