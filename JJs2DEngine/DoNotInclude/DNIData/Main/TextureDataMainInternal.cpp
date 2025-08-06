#include "MainDNIpch.h"
#include "TextureDataMainInternal.h"

namespace JJs2DEngine
{
	TextureDataMainInternal::TextureDataMainInternal(uint64_t transferFramesInFlight, const std::array<size_t, 10>& preLoadedTexturesMaxAmounts,
		const std::array<size_t, 10>& streamedTexturesMaxAmounts) : _transferFramesInFlight(transferFramesInFlight), _preLoadedTexturesMaxAmounts(preLoadedTexturesMaxAmounts),
		_streamedTexturesMaxAmounts(streamedTexturesMaxAmounts)
	{
	}

	TextureDataMainInternal::~TextureDataMainInternal()
	{
	}

}