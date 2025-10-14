#pragma once

#include <optional>
#include <utility>

namespace JJs2DEngine
{
	class TextureDataMainInternal;

	class TextureDataMain
	{
	public:
		TextureDataMain(TextureDataMainInternal& ref);
		~TextureDataMain();

		std::optional<std::pair<size_t, size_t>> TryToAddTextureToPreloadedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height);
		std::optional<std::pair<size_t, size_t>> TryToAddTextureToStreamedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height);

	private:
		TextureDataMainInternal& _internal;
	};
}
