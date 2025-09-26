#include "MainDNIpch.h"
#include "UiVertexDataLayerVersionInternal.h"

#include "ObjectDataInternal.h"

#include "../Common/MaxDepthValue.h"

#include "../../../Include/Main/ObjectData.h"

#include "TextureDataMainInternal.h"

#include <assert.h>
#include <limits>

namespace JJs2DEngine
{
	UiVertexDataLayerVersionInternal::UiVertexDataLayerVersionInternal(TextureDataMainInternal& textureDataList, size_t maxVertexAmount, size_t layersDepth) :
		_textureDataList(textureDataList), _objectList(maxVertexAmount)
	{
		_usedVertexAmount = 0;
		_nextDepthValueUNORM = 0;
		_layersDepth = layersDepth;

		_unusedIndexes.reserve(maxVertexAmount);
		for (size_t i = 0; i < maxVertexAmount; ++i)
		{
			_unusedIndexes.push_back(maxVertexAmount - (i + 1));
		}
	}

	UiVertexDataLayerVersionInternal::~UiVertexDataLayerVersionInternal()
	{
	}

	std::optional<size_t> UiVertexDataLayerVersionInternal::AddObject(const ObjectData& newObjectData)
	{
		std::optional<size_t> ret;

		if (_usedVertexAmount >= _objectList.size())
			throw std::runtime_error("UiVertexDataLayerVersionInternal::AddObject Error: Program tried to add more than the maximum number of objects to the layer!");

		assert(!_unusedIndexes.empty());

		if (_nextDepthValueUNORM > maxDepthValue)
			return ret;

		ObjectDataInternal added;

		float textureWidth = static_cast<float>(newObjectData.textureWidthUNORM);
		textureWidth /= static_cast<float>(onePointZeroUNORMValue);
		float textureHeight = static_cast<float>(newObjectData.textureHeightUNORM);
		textureHeight /= static_cast<float>(onePointZeroUNORMValue);
		added.texturesSizeInTile = glm::vec2(textureWidth, textureHeight);

		float screenWidth = static_cast<float>(newObjectData.screenWidthUNORM);
		screenWidth /= static_cast<float>(onePointZeroUNORMValue);
		float screenHeight = static_cast<float>(newObjectData.screenHeightUNORM);
		screenHeight /= static_cast<float>(onePointZeroUNORMValue);
		added.objectsSizeOnScreen = glm::vec2(screenWidth, screenHeight);

		float screenX = static_cast<float>(newObjectData.screenPositionXUNORM) - static_cast<float>(onePointZeroUNORMValue / 2);
		screenX /= static_cast<float>(onePointZeroUNORMValue / 2);
		float screenY = static_cast<float>(newObjectData.screenPositionYUNORM) - static_cast<float>(onePointZeroUNORMValue / 2);
		screenY /= static_cast<float>(onePointZeroUNORMValue / 2);
		added.objectsPositionOnScreen = glm::vec2(screenX, screenY);

		added.depthUNORM = _nextDepthValueUNORM;
		added.textureDataPointer = _textureDataList.GetTextureReference(newObjectData.inPreloadedTexturesList, newObjectData.textureIndex.first, newObjectData.textureIndex.second);

		size_t addedIndex = _unusedIndexes.back();
		assert(!_objectList[addedIndex].has_value());
		_objectList[addedIndex] = added;

		ret = addedIndex;
		_unusedIndexes.pop_back();
		_nextDepthValueUNORM++;
		_usedVertexAmount++;

		return ret;
	}

}