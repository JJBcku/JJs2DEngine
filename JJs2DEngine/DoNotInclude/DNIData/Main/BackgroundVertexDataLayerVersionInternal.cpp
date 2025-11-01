#include "MainDNIpch.h"
#include "BackgroundVertexDataLayerVersionInternal.h"

#include "BackgroundObjectBufferData.h"
#include "TextureDataMainInternal.h"

#include "../../../Include/Main/BackgroundObjectData.h"
#include "../../../Include/Main/NormalizedValuesList.h"

#include "../../../Include/Common/TextureReferenceData.h"

namespace JJs2DEngine
{
	BackgroundVertexDataLayerVersionInternal::BackgroundVertexDataLayerVersionInternal(TextureDataMainInternal& textureDataList, BackgroundObjectData textureData) :
_textureDataList(textureDataList)
	{
		size_t tileSize = 1ULL << (skippedSizeLevels + textureData.textureIndex.first);
		float fTileSize = static_cast<float>(tileSize);

		float textureWidth = static_cast<float>(textureData.textureWidthInPixels);
		textureWidth /= fTileSize;
		float textureHeight = static_cast<float>(textureData.textureHeightInPixels);
		textureHeight /= fTileSize;

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

	bool BackgroundVertexDataLayerVersionInternal::ChangeObjectsTexture(bool inPreloadedTexturesList, std::pair<size_t, size_t> newTextureID)
	{
		auto newTextures = _textureDataList.GetTextureReference(inPreloadedTexturesList, newTextureID.first, newTextureID.second);

		if (newTextures == _data.textureReference)
			return false;
		else
		{
			_data.textureReference = newTextures;
			return true;
		}
	}

}