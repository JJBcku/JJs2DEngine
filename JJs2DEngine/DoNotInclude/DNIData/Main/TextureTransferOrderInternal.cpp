#include "MainDNIpch.h"
#include "TextureTransferOrderInternal.h"

namespace JJs2DEngine
{
	TextureTransferOrderInternal::TextureTransferOrderInternal()
	{
		insertionIndex = std::numeric_limits<size_t>::max();
		texturesWidth = 0;
		texturesHeight = 0;

		stagingBufferDataOffset = std::numeric_limits<size_t>::max();
		stagingBufferDataSize = 0;
	}

	TextureTransferOrderInternal::~TextureTransferOrderInternal()
	{
	}

}