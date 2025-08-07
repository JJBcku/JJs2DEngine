#pragma once

#include "TextureFormatDef.h"

namespace JJs2DEngine
{
	enum class TextureFormat : uint64_t
	{
		TEXTURE_FORMAT_UNSET = 0,
		TEXTURE_FORMAT_RGBA16 = 1,

		TEXTURE_FORMAT_RGBA8 = 2,
		TEXTURE_FORMAT_BGRA8 = 4,
	};
}
