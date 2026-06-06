#pragma once

#include "BackgroundObjectDataInternal.h"

namespace JJs2DEngine
{
	class TextureDataMainInternal;

	struct BackgroundObjectData;
	struct BackgroundObjectBufferData;

	class BackgroundVertexDataLayerVersionInternal
	{
	public:
		BackgroundVertexDataLayerVersionInternal(TextureDataMainInternal& textureDataList, BackgroundObjectData textureData);
		~BackgroundVertexDataLayerVersionInternal();

		BackgroundVertexDataLayerVersionInternal& operator=(const BackgroundVertexDataLayerVersionInternal&) noexcept = delete;

		BackgroundObjectBufferData GetBufferData(size_t frameIndex) const;

		bool ChangeObjectsTexture(bool inPreloadedTexturesList, std::pair<size_t, size_t> newTextureID);

	private:
		TextureDataMainInternal& _textureDataList;

		BackgroundObjectDataInternal _data;
	};
}
