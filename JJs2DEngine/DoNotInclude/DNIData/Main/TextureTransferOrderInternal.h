#pragma once

namespace JJs2DEngine
{
	struct TextureTransferOrderInternal
	{
		size_t insertionIndex;
		uint32_t texturesWidth;
		uint32_t texturesHeight;

		size_t stagingBufferDataOffset;
		size_t stagingBufferDataSize;

		TextureTransferOrderInternal();
		~TextureTransferOrderInternal();
	};
}
