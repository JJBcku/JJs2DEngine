#include "MainDNIpch.h"
#include "BackgroundVertexDataLayerVersionListInternal.h"

#include <Miscellaneous/Bool64.h>

#include <VulkanSimplified/VSCommon/VSMemoryTypeProperties.h>
#include <VulkanSimplified/VSCommon/VSAccessFlags.h>

#include <VulkanSimplified/VSDevice/VSPrimaryIRCommandBuffer.h>
#include <VulkanSimplified/VSDevice/VSDataBuffersCopyRegionData.h>
#include <VulkanSimplified/VSDevice/VSDataBuffersMemoryBarrierData.h>

#include "BackgroundVertexDataLayerVersionInternal.h"
#include "BackgroundObjectBufferData.h"

#include "../../../Include/Main/BackgroundObjectData.h"

namespace JJs2DEngine
{
	BackgroundVertexDataLayerVersionListInternal::BackgroundVertexDataLayerVersionListInternal(VS::DataBufferLists dataBufferList, VS::MemoryObjectsList memoryObjectsList,
		TextureDataMainInternal& textureDataList, const std::vector<BackgroundObjectData>& versionDataList, size_t transferFramesAmount) :
		_dataBufferList(dataBufferList), _memoryObjectsList(memoryObjectsList)
	{
		if (versionDataList.empty())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::BackgroundVertexDataLayerVersionListInternal Error: Program tried to create an empty version list!");

		_versionList.resize(versionDataList.size());
		for (size_t i = 0; i < versionDataList.size(); ++i)
		{
			_versionList[i] = std::make_unique<BackgroundVertexDataLayerVersionInternal>(textureDataList, versionDataList[i]);
		}

		_ownedByTransferQueue.resize(versionDataList.size());
		_changed.resize(versionDataList.size());

		for (size_t i = 0; i < versionDataList.size(); ++i)
		{
			_ownedByTransferQueue[i].resize(transferFramesAmount, Misc::BOOL64_FALSE);
			_changed[i].resize(transferFramesAmount, Misc::BOOL64_TRUE);
		}

		_vertexBufferIDs.resize(versionDataList.size());
		for (size_t i = 0; i < versionDataList.size(); ++i)
		{
			_vertexBufferIDs[i].reserve(transferFramesAmount);
			for (size_t j = 0; j < transferFramesAmount; ++j)
			{
				_vertexBufferIDs[i].push_back(dataBufferList.AddVertexBuffer(sizeof(BackgroundObjectBufferData), {}));
			}
		}

		size_t requiredVertexSize = dataBufferList.GetVertexBuffersSize(_vertexBufferIDs.back().back());
		size_t requiredVertexAligment = dataBufferList.GetVertexBuffersRequiredAligment(_vertexBufferIDs.back().back());
		uint32_t vertexMemoryMask = dataBufferList.GetVertexBuffersMemoryTypeMask(_vertexBufferIDs.back().back());

		size_t vertexSuballocations = versionDataList.size() * transferFramesAmount;

		size_t requiredVertexMod = requiredVertexSize % requiredVertexAligment;
		if (requiredVertexMod != 0)
			requiredVertexSize += requiredVertexAligment - requiredVertexMod;
		requiredVertexSize *= vertexSuballocations;

		std::vector<VS::MemoryTypeProperties> acceptableVertexTypes;
		acceptableVertexTypes.reserve(3);
		acceptableVertexTypes.push_back(VS::DEVICE_LOCAL);
		acceptableVertexTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_VISIBLE | VS::HOST_COHERENT);
		acceptableVertexTypes.push_back(VS::HOST_VISIBLE | VS::HOST_COHERENT);

		_vertexMemoryID = memoryObjectsList.AllocateMemory(requiredVertexSize, vertexSuballocations, acceptableVertexTypes, vertexMemoryMask);

		for (size_t i = 0; i < _vertexBufferIDs.size(); ++i)
		{
			for (size_t j = 0; j < _vertexBufferIDs[i].size(); ++j)
			{
				dataBufferList.BindVertexBuffer(_vertexBufferIDs[i][j], _vertexMemoryID);
			}
		}

		bool vertexISAccesible = memoryObjectsList.IsMemoryMapped(_vertexMemoryID);

		if (!vertexISAccesible)
		{
			_stagingBufferIDs.reserve(transferFramesAmount);
			for (size_t i = 0; i < transferFramesAmount; ++i)
			{
				_stagingBufferIDs.push_back(dataBufferList.AddStagingBuffer(sizeof(BackgroundObjectBufferData), {}));
			}

			size_t requiredStagingSize = dataBufferList.GetStagingBuffersSize(_stagingBufferIDs.back());
			size_t requiredStagingAligment = dataBufferList.GetStagingBuffersRequiredAligment(_stagingBufferIDs.back());
			uint32_t stagingMemoryMask = dataBufferList.GetStagingBuffersMemoryTypeMask(_stagingBufferIDs.back());

			size_t requiredStagingMod = requiredStagingSize % requiredStagingAligment;
			if (requiredVertexMod != 0)
				requiredStagingSize += requiredStagingAligment - requiredStagingMod;
			requiredStagingSize *= transferFramesAmount;

			std::vector<VS::MemoryTypeProperties> acceptableStagingTypes;
			acceptableStagingTypes.reserve(2);
			acceptableStagingTypes.push_back(VS::HOST_VISIBLE | VS::HOST_COHERENT);
			acceptableStagingTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_VISIBLE | VS::HOST_COHERENT);

			_stagingMemoryID = memoryObjectsList.AllocateMemory(requiredStagingSize, transferFramesAmount, acceptableStagingTypes, stagingMemoryMask);
			for (size_t i = 0; i < transferFramesAmount; ++i)
			{
				dataBufferList.BindStagingBuffer(_stagingBufferIDs[i], _stagingMemoryID.value());
			}
		}
	}

	BackgroundVertexDataLayerVersionListInternal::~BackgroundVertexDataLayerVersionListInternal()
	{
	}

	bool BackgroundVertexDataLayerVersionListInternal::WriteDataToBuffer(size_t transferFrameIndice, VS::PrimaryIRCommandBuffer transferCommandBuffer)
	{
		bool commandRecorded = false;

		if (!_activeVersion.has_value())
			return commandRecorded;

		if (_activeVersion.value() >= _versionList.size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::WriteDataToBuffer Error: Program tried to access a non-existent layer version!");

		if (transferFrameIndice >= _vertexBufferIDs[_activeVersion.value()].size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::WriteDataToBuffer Error: Program tried to access an non-existent frame's data!");

		assert(_changed[_activeVersion.value()][transferFrameIndice] == Misc::BOOL64_TRUE || _changed[_activeVersion.value()][transferFrameIndice] == Misc::BOOL64_FALSE);

		if (_changed[_activeVersion.value()][transferFrameIndice] != Misc::BOOL64_TRUE)
			return commandRecorded;

		auto dataToWrite = _versionList[_activeVersion.value()]->GetBufferData(transferFrameIndice);

		if (_stagingMemoryID.has_value())
		{
			_dataBufferList.WriteToStagingBuffer(_stagingBufferIDs[transferFrameIndice], 0,
				*reinterpret_cast<unsigned char*>(&dataToWrite), sizeof(dataToWrite));

			VS::DataBuffersCopyRegionData copyData;
			copyData.srcOffset = 0;
			copyData.dstOffset = 0;
			copyData.writeSize = sizeof(dataToWrite);

			transferCommandBuffer.TransferDataListToVertexBuffer(_stagingBufferIDs[transferFrameIndice], _vertexBufferIDs[_activeVersion.value()][transferFrameIndice],
				{ copyData });

			commandRecorded = true;
		}
		else
		{
			_dataBufferList.WriteToVertexBuffer(_vertexBufferIDs[_activeVersion.value()][transferFrameIndice], 0,
				*reinterpret_cast<unsigned char*>(&dataToWrite), sizeof(dataToWrite));
		}

		_changed[_activeVersion.value()][transferFrameIndice] = Misc::BOOL64_FALSE;
		_ownedByTransferQueue[_activeVersion.value()][transferFrameIndice] = Misc::BOOL64_TRUE;

		return commandRecorded;
	}

	VS::DataBuffersMemoryBarrierData BackgroundVertexDataLayerVersionListInternal::GetOwnershipTransferData(size_t transferFrameIndice,
		size_t transferQueueID, size_t graphicQueueID)
	{
		VS::DataBuffersMemoryBarrierData ret;

		if (!_activeVersion.has_value())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::GetOwnershipTransferData Error: Program tried to access an unset active layer version!");

		if (_activeVersion.value() >= _versionList.size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::GetOwnershipTransferData Error: Program tried to access a non-existent layer version!");

		if (transferFrameIndice >= _vertexBufferIDs[_activeVersion.value()].size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::GetOwnershipTransferData Error: Program tried to access an non-existent frame's data!");

		ret.srcAccess = VS::AccessFlagBits::ACCESS_MEMORY_WRITE;
		ret.dstAccess = VS::AccessFlagBits::ACCESS_MEMORY_READ;
		if (transferQueueID != graphicQueueID)
			ret.queueData = { transferQueueID, graphicQueueID };
		ret.bufferID = _vertexBufferIDs[_activeVersion.value()][transferFrameIndice];

		return ret;
	}

	void BackgroundVertexDataLayerVersionListInternal::RecordDrawCommand(size_t transferFrameIndice, VS::PrimaryIRCommandBuffer graphicsCommandBuffer)
	{
		if (!_activeVersion.has_value())
			return;

		if (_activeVersion.value() >= _versionList.size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::RecordDrawCommand Error: Program tried to access a non-existent layer version!");

		if (transferFrameIndice >= _vertexBufferIDs[_activeVersion.value()].size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::RecordDrawCommand Error: Program tried to access an non-existent frame's data!");

		graphicsCommandBuffer.BindVertexBuffers(0, { { _vertexBufferIDs[_activeVersion.value()][transferFrameIndice], 0}});
		graphicsCommandBuffer.Draw(6, 1, 0, 0);
	}

	void BackgroundVertexDataLayerVersionListInternal::SetActiveVersion(std::optional<size_t> newActiveVersion)
	{
		if (newActiveVersion.has_value() && newActiveVersion.value() > _vertexBufferIDs.size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::SetActiveVersion Error: Program tried to set to big value of active layer version!");

		_activeVersion = newActiveVersion;
	}

	std::optional<size_t> BackgroundVertexDataLayerVersionListInternal::GetActiveVersion() const
	{
		return _activeVersion;
	}

	Misc::Bool64 BackgroundVertexDataLayerVersionListInternal::IsOwnedByTransferQueue(size_t transferFrameIndice) const
	{
		Misc::Bool64 ret = Misc::BOOL64_FALSE;

		if (!_activeVersion.has_value())
			return ret;

		if (_activeVersion.value() >= _versionList.size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::RecordDrawCommand Error: Program tried to access a non-existent layer version!");

		if (transferFrameIndice >= _ownedByTransferQueue[_activeVersion.value()].size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::RecordDrawCommand Error: Program tried to access an non-existent frame's data!");

		ret = _ownedByTransferQueue[_activeVersion.value()][transferFrameIndice];

		return ret;
	}

	void BackgroundVertexDataLayerVersionListInternal::SetOwnedByTransferQueue(size_t transferFrameIndice, Misc::Bool64Values newValue)
	{
		if (!_activeVersion.has_value())
			return;

		if (_activeVersion.value() >= _versionList.size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::RecordDrawCommand Error: Program tried to access a non-existent layer version!");

		if (transferFrameIndice >= _ownedByTransferQueue[_activeVersion.value()].size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::RecordDrawCommand Error: Program tried to access an non-existent frame's data!");

		assert(newValue == Misc::BOOL64_FALSE || newValue == Misc::BOOL64_TRUE);

		_ownedByTransferQueue[_activeVersion.value()][transferFrameIndice] = newValue;
	}

	void BackgroundVertexDataLayerVersionListInternal::ChangeObjectsTexture(size_t layerIndex, bool inPreloadedTexturesList, std::pair<size_t, size_t> newTextureID)
	{
		if (layerIndex > _vertexBufferIDs.size())
			throw std::runtime_error("BackgroundVertexDataLayerVersionListInternal::ChangeObjectsTexture Error: Program tried access non-existent layers version!");

		bool changed = _versionList[layerIndex]->ChangeObjectsTexture(inPreloadedTexturesList, newTextureID);

		if (changed)
		{
			for (size_t i = 0; i < _changed[layerIndex].size(); ++i)
			{
				_changed[layerIndex][i] = Misc::BOOL64_TRUE;
			}
		}
	}

}