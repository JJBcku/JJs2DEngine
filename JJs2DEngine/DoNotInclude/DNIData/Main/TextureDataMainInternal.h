#pragma once

#include <stdint.h>
#include <array>

namespace JJs2DEngine
{
	class TextureDataMainInternal
	{
	public:
		TextureDataMainInternal(uint64_t transferFramesInFlight, const std::array<size_t, 10>& preLoadedTexturesMaxAmounts, const std::array<size_t, 10>& streamedTexturesMaxAmounts);
		~TextureDataMainInternal();

	private:
		uint64_t _transferFramesInFlight;
		std::array<size_t, 10> _preLoadedTexturesMaxAmounts;
		std::array<size_t, 10> _streamedTexturesMaxAmounts;
	};
}
