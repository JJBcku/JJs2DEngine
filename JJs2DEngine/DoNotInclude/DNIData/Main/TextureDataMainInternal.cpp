#include "MainDNIpch.h"
#include "TextureDataMainInternal.h"

#include "../../../Include/Common/TextureFormat.h"

namespace JJs2DEngine
{
	TextureDataMainInternal::TextureDataMainInternal(uint64_t transferFramesInFlight, const std::array<size_t, 14>& preLoadedTexturesMaxAmounts,
		const std::array<size_t, 14>& streamedTexturesMaxAmounts, TextureFormat textureFormat) : _transferFramesInFlight(transferFramesInFlight),
		_preLoadedTexturesMaxAmounts(preLoadedTexturesMaxAmounts), _streamedTexturesMaxAmounts(streamedTexturesMaxAmounts)
	{
	}

	TextureDataMainInternal::~TextureDataMainInternal()
	{
	}

	bool TextureDataMainInternal::Is16Bit(TextureFormat textureFormat) const
	{
		if (textureFormat == TextureFormat::TEXTURE_FORMAT_RGBA16)
			return true;

		return false;
	}

}