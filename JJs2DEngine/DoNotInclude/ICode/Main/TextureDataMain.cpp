#include "MainIpch.h"
#include "../../../Include/Main/TextureDataMain.h"

#include "../../../DoNotInclude/DNIData/Main/TextureDataMainInternal.h"

namespace JJs2DEngine
{
	TextureDataMain::TextureDataMain(TextureDataMainInternal& ref) : _internal(ref)
	{
	}

	TextureDataMain::~TextureDataMain()
	{
	}

	std::optional<std::pair<size_t, size_t>> TextureDataMain::TryToAddTextureToPreloadedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height)
	{
		return _internal.TryToAddTextureToPreloadedTexturesTransferList(data, width, height);
	}

	std::optional<std::pair<size_t, size_t>> TextureDataMain::TryToAddTextureToStreamedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height)
	{
		return _internal.TryToAddTextureToStreamedTexturesTransferList(data, width, height);
	}

}