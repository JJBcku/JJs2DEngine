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

		struct ImagesColorRenderTargetID
		{
			ImagesIDType type;
			IDObject<AutoCleanupColorRenderTargetImage> ID;

			ImagesColorRenderTargetID(const IDObject<AutoCleanupColorRenderTargetImage>& ID) noexcept;
			~ImagesColorRenderTargetID() = default;
		} colorRenderTarget;

		struct ImagesTexture2DID
		{
			ImagesIDType type;
			IDObject<AutoCleanup2DTexture> ID;

			uint32_t baseMipLevel;
			uint32_t mipLevelPadding;
			std::optional<uint32_t> mipLevelCount;

			ImagesTexture2DID(const IDObject<AutoCleanup2DTexture>& ID) noexcept;
			~ImagesTexture2DID() = default;
		} texture2D;

		struct ImagesTexture2DArrayID
		{
			ImagesIDType type;
			IDObject<AutoCleanup2DArrayTexture> ID;

			uint32_t baseMipLevel;
			std::optional<uint32_t> mipLevelCount;

			uint32_t baseArrayLayer;
			std::optional<uint32_t> arrayLayersCount;

			ImagesTexture2DArrayID(const IDObject<AutoCleanup2DArrayTexture>& ID) noexcept;
			~ImagesTexture2DArrayID() = default;
		} textureArray2D;

		ImagesGenericID() noexcept;
		ImagesGenericID(const IDObject<AutoCleanupColorRenderTargetImage>& ID) noexcept;
		ImagesGenericID(const IDObject<AutoCleanup2DTexture>& ID) noexcept;
		ImagesGenericID(const IDObject<AutoCleanup2DArrayTexture>& ID) noexcept;

		ImagesGenericID(const ImagesGenericID& rhs) noexcept;
		ImagesGenericID(ImagesGenericID&& rhs) noexcept;
		~ImagesGenericID();

		ImagesGenericID& operator=(const ImagesGenericID& rhs) noexcept;
		ImagesGenericID& operator=(ImagesGenericID&& rhs) noexcept;
	};
}