#include "MainDNIpch.h"
#include "UiVertexDataLayerVersionInternal.h"

#include "TextureDataMainInternal.h"
#include "UiObjectBufferData.h"
#include "UiObjectDataInternal.h"

#include "../Common/MaxDepthValue.h"

#include "../../../Include/Main/UiObjectData.h"

#include "../../../Include/Common/TextureReferenceData.h"

#include <Miscellaneous/Bool64.h>

#include <assert.h>
#include <limits>

namespace JJs2DEngine
{
	UiVertexDataLayerVersionInternal::UiVertexDataLayerVersionInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists& dataBufferList,
		size_t maxVertexAmount, size_t layersDepth) : _textureDataList(textureDataList), _dataBufferList(dataBufferList), _objectList(maxVertexAmount)
	{
		_usedVertexAmount = 0;
		_nextDepthValueUNORM = 0;
		_layersDepth = layersDepth;

		if (maxVertexAmount > maxVertexMaximumValue)
			throw std::runtime_error("UiVertexDataLayerVersionInternal::UiVertexDataLayerVersionInternal Error: Layer was given too big max vertex value!");

		if (layersDepth > maxLayerDepth)
			throw std::runtime_error("UiVertexDataLayerVersionInternal::UiVertexDataLayerVersionInternal Error: Layer was given too big layers depth value!");

		_unusedIndexes.reserve(maxVertexAmount);
		for (size_t i = 0; i < maxVertexAmount; ++i)
		{
			_unusedIndexes.push_back(maxVertexAmount - (i + 1));
		}

		_vertexBuffer = _dataBufferList.AddVertexBuffer(sizeof(UiObjectBufferData) * _objectList.size(), {}, 0x10);

		_buffersMemoryMask = _dataBufferList.GetVertexBuffersMemoryTypeMask(_vertexBuffer);;
		_buffersMemorySize = _dataBufferList.GetVertexBuffersSize(_vertexBuffer);
		_buffersMemoryAligment = _dataBufferList.GetVertexBuffersRequiredAligment(_vertexBuffer);

		_changed = Misc::Bool64Values::BOOL64_FALSE;
	}

	UiVertexDataLayerVersionInternal::~UiVertexDataLayerVersionInternal()
	{
		_dataBufferList.RemoveVertexBuffer(_vertexBuffer, false);
	}

	std::optional<size_t> UiVertexDataLayerVersionInternal::AddObject(const UiObjectData& newObjectData)
	{
		std::optional<size_t> ret;

		if (_usedVertexAmount >= _objectList.size())
			throw std::runtime_error("UiVertexDataLayerVersionInternal::AddObject Error: Program tried to add more than the maximum number of objects to the layer!");

		assert(!_unusedIndexes.empty());

		if (_nextDepthValueUNORM > maxDepthValue)
			return ret;

		UiObjectDataInternal added;

		float textureWidth = static_cast<float>(newObjectData.textureWidth_UNORM);
		textureWidth /= static_cast<float>(onePointZeroUNORMValue);
		float textureHeight = static_cast<float>(newObjectData.textureHeight_UNORM);
		textureHeight /= static_cast<float>(onePointZeroUNORMValue);
		added.texturesSizeInTile = glm::vec2(textureWidth, textureHeight);

		float screenWidth = static_cast<float>(newObjectData.screenWidth_UNORM);
		screenWidth /= static_cast<float>(onePointZeroUNORMValue);
		float screenHeight = static_cast<float>(newObjectData.screenHeight_UNORM);
		screenHeight /= static_cast<float>(onePointZeroUNORMValue);
		added.objectsSizeOnScreen = glm::vec2(screenWidth, screenHeight);

		float screenX = static_cast<float>(newObjectData.screenPositionX_SNORM);
		screenX /= static_cast<float>(onePointZeroSNORMValue);
		float screenY = static_cast<float>(newObjectData.screenPositionY_SNORM);
		screenY /= static_cast<float>(onePointZeroSNORMValue);
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

		_changed = Misc::Bool64Values::BOOL64_TRUE;

		return ret;
	}

	uint32_t UiVertexDataLayerVersionInternal::GetBuffersMask() const
	{
		return static_cast<uint32_t>(_buffersMemoryMask);
	}

	uint64_t UiVertexDataLayerVersionInternal::GetMemorySize() const
	{
		return _buffersMemorySize;
	}

	uint64_t UiVertexDataLayerVersionInternal::GetMemoryAligment() const
	{
		return _buffersMemoryAligment;
	}

	IDObject<VS::AutoCleanupVertexBuffer> UiVertexDataLayerVersionInternal::GetVertexBufferID()
	{
		return _vertexBuffer;
	}

	size_t UiVertexDataLayerVersionInternal::WriteDataToBuffer(std::optional<IDObject<VS::AutoCleanupStagingBuffer>> stagingBufferID)
	{
		assert(_changed == Misc::BOOL64_TRUE || _changed == Misc::BOOL64_FALSE);

		if (_changed != Misc::BOOL64_TRUE)
			return 0;

		size_t writtenSize = 0;

		std::vector<UiObjectBufferData> dataToWrite;
		dataToWrite.resize(_usedVertexAmount);

		size_t foundVertexes = 0;
		size_t currentListIndex = 0;
		while (foundVertexes < _usedVertexAmount)
		{
			if (currentListIndex >= _objectList.size())
				throw std::runtime_error("UiVertexDataLayerVersionInternal::WriteDataToBuffer Error: Program failed to find all objects in a layer!");

			if (_objectList[currentListIndex].has_value())
			{
				auto& objectToWrite = dataToWrite[foundVertexes];
				auto& inputObject = _objectList[currentListIndex].value();

				objectToWrite.texCoords = inputObject.textureDataPointer->textureCoords;
				objectToWrite.texSize = inputObject.textureDataPointer->textureSize * inputObject.texturesSizeInTile;

				float depthMantissa = static_cast<float>(inputObject.depthUNORM) / static_cast<float>(maxVertexMaximumValue);

				float depthExponent = 0.5f;
				size_t layerDepthRemaining = _layersDepth;
				while (layerDepthRemaining > 0)
				{
					size_t shiftSize = 24;
					if (layerDepthRemaining < shiftSize)
						shiftSize = layerDepthRemaining;

					depthExponent /= static_cast<float>(1 << shiftSize);
					layerDepthRemaining -= shiftSize;
				}

				objectToWrite.pos = glm::vec4(inputObject.objectsPositionOnScreen, glm::vec2(depthMantissa * depthExponent, 1.0f));
				objectToWrite.size = inputObject.objectsSizeOnScreen;

				objectToWrite.texLayer = inputObject.textureDataPointer->textureLayer;
				objectToWrite.texIndex = inputObject.textureDataPointer->textureIndex;

				foundVertexes++;
			}

			currentListIndex++;
		}

		writtenSize = sizeof(dataToWrite[0]) * dataToWrite.size();

		if (stagingBufferID.has_value())
		{
			_dataBufferList.WriteToStagingBuffer(stagingBufferID.value(), 0, *reinterpret_cast<const unsigned char*>(dataToWrite.data()), writtenSize);
		}
		else
		{
			_dataBufferList.WriteToVertexBuffer(_vertexBuffer, 0, *reinterpret_cast<const unsigned char*>(dataToWrite.data()), writtenSize);
		}

		return writtenSize;
	}

}