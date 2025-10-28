#include "MainDNIpch.h"
#include "BackgroundVertexDataLayerVersionInternal.h"

#include "BackgroundObjectBufferData.h"
#include "TextureDataMainInternal.h"

#include "../../../Include/Main/BackgroundObjectData.h"
#include "../../../Include/Main/NormalizedValuesList.h"

#include "../../../Include/Common/TextureReferenceData.h"

namespace JJs2DEngine
{
	BackgroundVertexDataLayerVersionInternal::BackgroundVertexDataLayerVersionInternal(TextureDataMainInternal& textureDataList, BackgroundObjectData textureData) : _textureDataList(textureDataList)
	{
		float textureWidth = static_cast<float>(textureData.textureWidth_UNORM);
		textureWidth /= static_cast<float>(onePointZeroUNORMValue);
		float textureHeight = static_cast<float>(textureData.textureHeight_UNORM);
		textureHeight /= static_cast<float>(onePointZeroUNORMValue);

		_data.inTextureSize = glm::vec2(textureWidth, textureHeight);
		_data.textureReference = _textureDataList.GetTextureReference(textureData.inPreloadedTexturesList, textureData.textureIndex.first, textureData.textureIndex.second);
	}

	BackgroundVertexDataLayerVersionInternal::~BackgroundVertexDataLayerVersionInternal()
	{
	}

	BackgroundObjectBufferData BackgroundVertexDataLayerVersionInternal::GetBufferData(size_t frameIndex) const
	{
		size_t frame = 0;
		if (_data.textureReference.size() > 1)
			frame = frameIndex;

		if (frame >= _data.textureReference.size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionInternal::GetBufferData Error: Program tried to access non-existent texture reference!");

		BackgroundObjectBufferData ret;

		ret.textureCoordinates = _data.textureReference[frame]->textureCoords;
		ret.textureSize = _data.textureReference[frame]->textureSize * _data.inTextureSize;
		ret.textureLayer = _data.textureReference[frame]->textureLayer;
		ret.textureIndex = _data.textureReference[frame]->textureIndex;

		return ret;
	}

}