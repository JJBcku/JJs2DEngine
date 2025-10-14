#pragma once

namespace JJs2DEngine
{
	struct TextureTransferOrderInternal
	{
		uint64_t insertionIndex;
		uint32_t texturesWidth;
		uint32_t texturesHeight;

		uint64_t stagingBufferDataOffset;
		uint64_t stagingBufferDataSize;

		TextureTransferOrderInternal();
		~TextureTransferOrderInternal();
	};
}
