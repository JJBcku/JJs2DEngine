#include "MainDNIpch.h"
#include "TextureDataMainInternal.h"

namespace JJs2DEngine
{
	TextureDataMainInternal::TextureDataMainInternal(uint64_t transferFramesInFlight, const std::array<size_t, 14>& preLoadedTexturesMaxAmounts,
		const std::array<size_t, 14>& streamedTexturesMaxAmounts, VS::DataFormatSetIndependentID textureFormat) : _transferFramesInFlight(transferFramesInFlight),
		_preLoadedTexturesMaxAmounts(preLoadedTexturesMaxAmounts), _streamedTexturesMaxAmounts(streamedTexturesMaxAmounts), _textureFormat(textureFormat)
	{
	}

	TextureDataMainInternal::~TextureDataMainInternal()
	{
	}

}