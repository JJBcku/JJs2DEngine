#include "MainDNIpch.h"
#include "WorldLayerVertexDataLayerVersionInternal.h"

#include "TextureDataMainInternal.h"
#include "WorldLayerObjectBufferData.h"
#include "WorldLayerObjectDataInternal.h"

#include "../Common/MaxDepthValue.h"

#include "../../../Include/Main/WorldLayerObjectData.h"

#include "../../../Include/Common/TextureReferenceData.h"

#include <Miscellaneous/Bool64.h>

#include <assert.h>
#include <limits>

namespace JJs2DEngine
{
	WorldLayerVertexDataLayerVersionInternal::WorldLayerVertexDataLayerVersionInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists& dataBufferList,
		size_t maxObjectAmount, size_t layersDepth, size_t transferFrameAmount) : _textureDataList(textureDataList), _dataBufferList(dataBufferList), _objectList(maxObjectAmount)
	{
		_usedObjectAmount = 0;
		_nextDepthValueUNORM = 0;
		_layersDepth = layersDepth;

		if (maxObjectAmount > maxVertexMaximumValue)
			throw std::runtime_error("WorldLayerVertexDataLayerVersionInternal::WorldLayerVertexDataLayerVersionInternal Error: Layer was given too big max vertex value!");

		if (layersDepth > maxLayerDepth)
			throw std::runtime_error("WorldLayerVertexDataLayerVersionInternal::WorldLayerVertexDataLayerVersionInternal Error: Layer was given too big layers depth value!");

		_unusedIndexes.reserve(maxObjectAmount);
		for (size_t i = 0; i < maxObjectAmount; ++i)
		{
			_unusedIndexes.push_back(maxObjectAmount - (i + 1));
		}

		_ownedByTransferQueue.resize(transferFrameAmount);
		_frameData.resize(transferFrameAmount);
		for (size_t i = 0; i < _frameData.size(); ++i)
		{
			_ownedByTransferQueue[i] = Misc::BOOL64_FALSE;
			_frameData[i].vertexBuffer = _dataBufferList.AddVertexBuffer(sizeof(WorldLayerObjectBufferData) * _objectList.size(), {}, 0x10);
		}

		_buffersMemoryMask = _dataBufferList.GetVertexBuffersMemoryTypeMask(_frameData[0].vertexBuffer);
		_singleBuffersMemorySize = _dataBufferList.GetVertexBuffersSize(_frameData[0].vertexBuffer);
		_buffersMemoryAligment = _dataBufferList.GetVertexBuffersRequiredAligment(_frameData[0].vertexBuffer);

		_totalBuffersMemorySize = _singleBuffersMemorySize;
		uint64_t totalBuffersMemorySizeMod = _totalBuffersMemorySize % _buffersMemoryAligment;
		if (totalBuffersMemorySizeMod != 0)
		{
			_totalBuffersMemorySize += _buffersMemoryAligment - totalBuffersMemorySizeMod;
		}
		_totalBuffersMemorySize *= _frameData.size();
	}

	WorldLayerVertexDataLayerVersionInternal::~WorldLayerVertexDataLayerVersionInternal()
	{
		for (size_t i = 0; i < _frameData.size(); ++i)
		{
			_dataBufferList.RemoveVertexBuffer(_frameData[i].vertexBuffer, false);
		}
	}

	std::optional<size_t> WorldLayerVertexDataLayerVersionInternal::AddObject(const WorldLayerObjectData& newObjectData)
	{
		std::optional<size_t> ret;

		if (_usedObjectAmount >= _objectList.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionInternal::AddObject Error: Program tried to add more than the maximum number of objects to the layer!");

		assert(!_unusedIndexes.empty());

		if (_nextDepthValueUNORM > maxDepthValue)
			return ret;

		WorldLayerObjectDataInternal added;

		size_t tileSize = 1ULL << (skippedSizeLevels + newObjectData.textureIndex.first);
		float fTileSize = static_cast<float>(tileSize);

		float textureWidth = static_cast<float>(newObjectData.textureWidthInPixels) * 2.0f - 1.0f;
		textureWidth /= fTileSize * 2.0f;
		float textureHeight = static_cast<float>(newObjectData.textureHeightInPixels) * 2.0f - 1.0f;
		textureHeight /= fTileSize * 2.0f;
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
		added.textureDataPointer = _textureDataList.GetTextureReference(newObjectData.inPreloadedTexturesList,
			newObjectData.textureIndex.first, newObjectData.textureIndex.second);

		size_t addedIndex = _unusedIndexes.back();
		assert(!_objectList[addedIndex].has_value());
		_objectList[addedIndex] = added;

		ret = addedIndex;
		_unusedIndexes.pop_back();
		_nextDepthValueUNORM++;
		_usedObjectAmount++;

		for (size_t i = 0; i < _frameData.size(); ++i)
		{
			_frameData[i].changed = Misc::Bool64Values::BOOL64_TRUE;
		}

		return ret;
	}

	uint32_t WorldLayerVertexDataLayerVersionInternal::GetBuffersMask() const
	{
		return static_cast<uint32_t>(_buffersMemoryMask);
	}

	uint64_t WorldLayerVertexDataLayerVersionInternal::GetSingleBuffersMemorySize() const
	{
		return _singleBuffersMemorySize;
	}

	uint64_t WorldLayerVertexDataLayerVersionInternal::GetTotalBuffersMemorySize() const
	{
		return _totalBuffersMemorySize;
	}

	uint64_t WorldLayerVertexDataLayerVersionInternal::GetMemoryAligment() const
	{
		return _buffersMemoryAligment;
	}

	IDObject<VS::AutoCleanupVertexBuffer> WorldLayerVertexDataLayerVersionInternal::GetVertexBufferID(size_t transferFrameIndice)
	{
		if (transferFrameIndice >= _frameData.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionInternal::GetVertexBufferID Error: Program tried to access an non-existent frame's data!");

		return _frameData[transferFrameIndice].vertexBuffer;
	}

	size_t WorldLayerVertexDataLayerVersionInternal::WriteDataToBuffer(std::optional<IDObject<VS::AutoCleanupStagingBuffer>> stagingBufferID, size_t transferFrameIndice)
	{
		if (transferFrameIndice >= _frameData.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionInternal::WriteDataToBuffer Error: Program tried to access an non-existent frame's data!");

		assert(_frameData[transferFrameIndice].changed == Misc::BOOL64_TRUE || _frameData[transferFrameIndice].changed == Misc::BOOL64_FALSE);

		if (_frameData[transferFrameIndice].changed != Misc::BOOL64_TRUE)
			return 0;

		size_t writtenSize = 0;

		std::vector<WorldLayerObjectBufferData> dataToWrite;
		dataToWrite.resize(_usedObjectAmount);

		size_t foundVertexes = 0;
		size_t currentListIndex = 0;
		while (foundVertexes < _usedObjectAmount)
		{
			if (currentListIndex >= _objectList.size())
				throw std::runtime_error("WorldLayerVertexDataLayerVersionInternal::WriteDataToBuffer Error: Program failed to find all objects in a layer!");

			if (_objectList[currentListIndex].has_value())
			{
				auto& objectToWrite = dataToWrite[foundVertexes];
				auto& inputObject = _objectList[currentListIndex].value();

				if (inputObject.textureDataPointer.size() > 1)
				{
					objectToWrite.texCoords = inputObject.textureDataPointer[transferFrameIndice]->textureCoords;
					objectToWrite.texSize = inputObject.textureDataPointer[transferFrameIndice]->textureSize * inputObject.texturesSizeInTile;
				}
				else
				{
					objectToWrite.texCoords = inputObject.textureDataPointer[0]->textureCoords;
					objectToWrite.texSize = inputObject.textureDataPointer[0]->textureSize * inputObject.texturesSizeInTile;
				}

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

				if (inputObject.textureDataPointer.size() > 1)
				{
					objectToWrite.texLayer = inputObject.textureDataPointer[transferFrameIndice]->textureLayer;
					objectToWrite.texIndex = inputObject.textureDataPointer[transferFrameIndice]->textureIndex;
				}
				else
				{
					objectToWrite.texLayer = inputObject.textureDataPointer[0]->textureLayer;
					objectToWrite.texIndex = inputObject.textureDataPointer[0]->textureIndex;
				}

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
			_dataBufferList.WriteToVertexBuffer(_frameData[transferFrameIndice].vertexBuffer, 0, *reinterpret_cast<const unsigned char*>(dataToWrite.data()), writtenSize);
		}

		_frameData[transferFrameIndice].changed = Misc::BOOL64_FALSE;
		_frameData[transferFrameIndice].lastAmountWrittenToVertexBuffer = dataToWrite.size();

		return writtenSize;
	}

	size_t WorldLayerVertexDataLayerVersionInternal::GetCurrentUsedObjectAmount() const
	{
		return _usedObjectAmount;
	}

	size_t WorldLayerVertexDataLayerVersionInternal::GetAmountOfObjectsInVertexBuffer(size_t transferFrameIndice) const
	{
		if (transferFrameIndice >= _ownedByTransferQueue.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionInternal::GetAmountOfObjectsInVertexBuffer Error: Program tried to access an non-existent frame's data!");

		return _frameData[transferFrameIndice].lastAmountWrittenToVertexBuffer;
	}

	Misc::Bool64 WorldLayerVertexDataLayerVersionInternal::IsOwnedByTransferQueue(size_t transferFrameIndice) const
	{
		if (transferFrameIndice >= _ownedByTransferQueue.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionInternal::IsOwnedByTransferQueue Error: Program tried to access an non-existent frame's data!");

		assert(_ownedByTransferQueue[transferFrameIndice] == Misc::BOOL64_FALSE || _ownedByTransferQueue[transferFrameIndice] == Misc::BOOL64_TRUE);

		return _ownedByTransferQueue[transferFrameIndice];
	}

	void WorldLayerVertexDataLayerVersionInternal::SetOwnedByTransferQueue(size_t transferFrameIndice, Misc::Bool64Values newValue)
	{
		if (transferFrameIndice >= _ownedByTransferQueue.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionInternal::SetOwnedByTransferQueue Error: Program tried to access an non-existent frame's data!");

		assert(newValue == Misc::BOOL64_FALSE || newValue == Misc::BOOL64_TRUE);

		_ownedByTransferQueue[transferFrameIndice] = newValue;
	}
}