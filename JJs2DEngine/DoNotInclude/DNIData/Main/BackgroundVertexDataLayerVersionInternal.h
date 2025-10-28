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

		BackgroundObjectBufferData GetBufferData(size_t frameIndex) const;

	private:
		TextureDataMainInternal& _textureDataList;

		BackgroundObjectDataInternal _data;
	};
}
