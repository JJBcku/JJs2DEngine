#pragma once

#include <CustomLists/IDObject.h>

#include "../VSDevice/VSImageDataListsDef.h"

#include <optional>
#include <stdint.h>

namespace VulkanSimplified
{
	enum class ImagesIDType : uint64_t
	{
		UNKNOWN = 0,
		COLOR_RENDER_TARGET = 1,
		TEXTURE_2D = 2,
		TEXTURE_2D_ARRAY = 4,
	};

	union ImagesGenericID
	{
		ImagesIDType type;

		struct
		{
			ImagesIDType type;
			IDObject<AutoCleanupColorRenderTargetImage> ID;
		} colorRenderTarget;

		struct
		{
			ImagesIDType type;
			IDObject<AutoCleanup2DTexture> ID;

			uint32_t baseMipLevel;
			std::optional<uint32_t> mipLevelCount;
		} texture2D;

		struct
		{
			ImagesIDType type;
			IDObject<AutoCleanup2DArrayTexture> ID;

			uint32_t baseMipLevel;
			std::optional<uint32_t> mipLevelCount;

			uint32_t baseArrayLayer;
			std::optional<uint32_t> arrayLayersCount;
		} textureArray2D;

		ImagesGenericID();
		ImagesGenericID(IDObject<AutoCleanupColorRenderTargetImage> ID);
		ImagesGenericID(IDObject<AutoCleanup2DTexture> ID);
		ImagesGenericID(IDObject<AutoCleanup2DArrayTexture> ID);
	};
}