#include "MainDNIpch.h"
#include "WorldLayerVertexDataLayerVersionListInternal.h"

#include <Miscellaneous/Bool64.h>

#include <VulkanSimplified/VSCommon/VSMemoryTypeProperties.h>
#include <VulkanSimplified/VSCommon/VSAccessFlags.h>

#include <VulkanSimplified/VSDevice/VSPrimaryIRCommandBuffer.h>
#include <VulkanSimplified/VSDevice/VSDataBuffersCopyRegionData.h>
#include <VulkanSimplified/VSDevice/VSDataBuffersMemoryBarrierData.h>

#include <limits>

#include "WorldLayerVertexDataLayerVersionInternal.h"

namespace JJs2DEngine
{
	WorldLayerVertexDataLayerVersionListInternal::WorldLayerVertexDataLayerVersionListInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists& dataBufferList,
		VS::MemoryObjectsList& memoryObjectsList, const std::vector<size_t>& versionsMaxObjectAmountsList, size_t layersDepth, size_t transferFrameAmount) : _dataBufferList(dataBufferList),
		_memoryObjectsList(memoryObjectsList)
	{
		uint32_t allBuffersMask = std::numeric_limits<uint32_t>::max();
		uint64_t allBuffersSize = 0;

		uint64_t biggestBufferSize = 0;

		_versionList.reserve(versionsMaxObjectAmountsList.size());
		for (uint64_t i = 0; i < versionsMaxObjectAmountsList.size(); ++i)
		{
			_versionList.push_back(std::make_unique<WorldLayerVertexDataLayerVersionInternal>(textureDataList, dataBufferList, versionsMaxObjectAmountsList[i],
				layersDepth, transferFrameAmount));

			uint32_t buffersMask = _versionList.back()->GetBuffersMask();
			uint64_t buffersTotalSize = _versionList.back()->GetTotalBuffersMemorySize();
			uint64_t buffersAligment = _versionList.back()->GetMemoryAligment();

			allBuffersMask = allBuffersMask & buffersMask;

			uint64_t aligmentsMod = allBuffersSize % buffersAligment;
			if (aligmentsMod != 0)
			{
				allBuffersSize += buffersAligment - aligmentsMod;
			}
			allBuffersSize += buffersTotalSize;

			uint64_t singleBuffersSize = _versionList.back()->GetSingleBuffersMemorySize();
			if (singleBuffersSize > biggestBufferSize)
				biggestBufferSize = singleBuffersSize;
		}

		std::vector<VS::MemoryTypeProperties> acceptableTypes;
		acceptableTypes.reserve(3);
		acceptableTypes.push_back(VS::DEVICE_LOCAL);
		acceptableTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_VISIBLE | VS::HOST_COHERENT);
		acceptableTypes.push_back(VS::HOST_VISIBLE | VS::HOST_COHERENT);

		_vertexMemoryID = _memoryObjectsList.AllocateMemory(allBuffersSize, _versionList.size() * transferFrameAmount, acceptableTypes, allBuffersMask);

		for (uint64_t i = 0; i < _versionList.size(); ++i)
		{
			for (size_t j = 0; j < transferFrameAmount; ++j)
			{
				auto bufferID = _versionList[i]->GetVertexBufferID(j);

				_dataBufferList.BindVertexBuffer(bufferID, _vertexMemoryID);
			}
		}

		if (!_memoryObjectsList.IsMemoryMapped(_vertexMemoryID))
		{
			_stagingBufferIDs.reserve(transferFrameAmount);

			for (size_t i = 0; i < transferFrameAmount; ++i)
			{
				_stagingBufferIDs.push_back(_dataBufferList.AddStagingBuffer(biggestBufferSize, {}));
			}

			uint32_t stagingBuffersMemoryMask = _dataBufferList.GetStagingBuffersMemoryTypeMask(_stagingBufferIDs[0]);
			uint64_t stagingBuffersMemorySize = _dataBufferList.GetStagingBuffersSize(_stagingBufferIDs[0]);
			uint64_t stagingBuffersMemoryAligment = _dataBufferList.GetStagingBuffersRequiredAligment(_stagingBufferIDs[0]);
			uint64_t stagingBuffersMemorySizeMod = stagingBuffersMemorySize % stagingBuffersMemoryAligment;
			if (stagingBuffersMemorySizeMod != 0)
				stagingBuffersMemorySize += stagingBuffersMemoryAligment - stagingBuffersMemorySizeMod;
			stagingBuffersMemorySize *= _stagingBufferIDs.size();

			std::vector<VS::MemoryTypeProperties> acceptableStagingTypes;
			acceptableStagingTypes.reserve(2);
			acceptableStagingTypes.push_back(VS::HOST_VISIBLE | VS::HOST_COHERENT);
			acceptableStagingTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_VISIBLE | VS::HOST_COHERENT);

			_stagingMemoryID = _memoryObjectsList.AllocateMemory(stagingBuffersMemorySize, transferFrameAmount, acceptableStagingTypes, stagingBuffersMemoryMask);

			for (size_t i = 0; i < transferFrameAmount; ++i)
			{
				_dataBufferList.BindStagingBuffer(_stagingBufferIDs[i], _stagingMemoryID.value());
			}
		}

		_activeVersion = 0;
	}

	WorldLayerVertexDataLayerVersionListInternal::~WorldLayerVertexDataLayerVersionListInternal()
	{
		_versionList.clear();
		_memoryObjectsList.FreeMemory(_vertexMemoryID, false, false);

		if (_stagingMemoryID.has_value())
		{
			for (size_t i = 0; i < _stagingBufferIDs.size(); ++i)
			{
				_dataBufferList.RemoveStagingBuffer(_stagingBufferIDs[i], false);
			}
		}

		if (_stagingMemoryID.has_value())
		{
			_memoryObjectsList.FreeMemory(_stagingMemoryID.value(), false, false);
		}
	}

	bool WorldLayerVertexDataLayerVersionListInternal::WriteDataToBuffer(size_t transferFrameIndice, VS::PrimaryIRCommandBuffer transferCommandBuffer)
	{
		bool commandRecorded = false;

		if (_activeVersion >= _versionList.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionListInternal::WriteDataToBuffer Error: Program tried to access a non-existent layer version!");

		auto& layer = _versionList[_activeVersion];
		if (_stagingMemoryID.has_value())
		{
			if (transferFrameIndice >= _stagingBufferIDs.size())
				throw std::runtime_error("WorldLayerVertexDataLayerVersionListInternal::WriteDataToBuffer Error: Program tried to access an non-existent frame's data!");

			uint64_t writtenData = layer->WriteDataToBuffer(_stagingBufferIDs[transferFrameIndice], transferFrameIndice);

			if (writtenData > 0)
			{
				VS::DataBuffersCopyRegionData copyData;
				copyData.srcOffset = 0;
				copyData.dstOffset = 0;
				copyData.writeSize = writtenData;

				transferCommandBuffer.TransferDataListToVertexBuffer(_stagingBufferIDs[transferFrameIndice],
					_versionList[_activeVersion]->GetVertexBufferID(transferFrameIndice), { copyData });
				commandRecorded = true;
				_versionList[_activeVersion]->SetOwnedByTransferQueue(transferFrameIndice, Misc::BOOL64_TRUE);
			}
		}
		else
		{
			layer->WriteDataToBuffer({}, transferFrameIndice);
			_versionList[_activeVersion]->SetOwnedByTransferQueue(transferFrameIndice, Misc::BOOL64_TRUE);
		}

		return commandRecorded;
	}

	VS::DataBuffersMemoryBarrierData WorldLayerVertexDataLayerVersionListInternal::GetOwnershipTransferData(size_t transferFrameIndice,
		size_t transferQueueID, size_t graphicQueueID)
	{
		VS::DataBuffersMemoryBarrierData ret;

		if (_activeVersion >= _versionList.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionListInternal::GetOwnershipTransferData Error: Program tried to access a non-existent layer version!");

		ret.srcAccess = VS::AccessFlagBits::ACCESS_MEMORY_WRITE;
		ret.dstAccess = VS::AccessFlagBits::ACCESS_MEMORY_READ;
		ret.queueData = { transferQueueID, graphicQueueID };
		ret.bufferID = _versionList[_activeVersion]->GetVertexBufferID(transferFrameIndice);

		return ret;
	}

	void WorldLayerVertexDataLayerVersionListInternal::RecordDrawCommand(size_t transferFrameIndice, VS::PrimaryIRCommandBuffer graphicsCommandBuffer)
	{
		if (_activeVersion >= _versionList.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionListInternal::RecordDrawCommand Error: Program tried to access a non-existent layer version!");

		size_t objectCount = _versionList[_activeVersion]->GetAmountOfObjectsInVertexBuffer(transferFrameIndice);
		auto vertexBufferID = _versionList[_activeVersion]->GetVertexBufferID(transferFrameIndice);

		graphicsCommandBuffer.BindVertexBuffers(0, { { vertexBufferID, 0 } });
		graphicsCommandBuffer.Draw(6, static_cast<uint32_t>(objectCount), 0, 0);
	}

	WorldLayerVertexDataLayerVersionInternal& WorldLayerVertexDataLayerVersionListInternal::GetLayersVersion(size_t versionIndex)
	{
		if (versionIndex >= _versionList.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionListInternal::GetLayersVersion Error: Program tried to get a non-existent version of the layer!");

		return *_versionList[versionIndex];
	}

	const WorldLayerVertexDataLayerVersionInternal& WorldLayerVertexDataLayerVersionListInternal::GetLayersVersion(size_t versionIndex) const
	{
		if (versionIndex >= _versionList.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionListInternal::GetLayersVersion Const Error: Program tried to get a non-existent version of the layer!");

		return *_versionList[versionIndex];
	}

	Misc::Bool64 WorldLayerVertexDataLayerVersionListInternal::IsOwnedByTransferQueue(size_t transferFrameIndice) const
	{
		if (_activeVersion >= _versionList.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionListInternal::GetLayersVersion Const Error: Program tried to get a non-existent version of the layer!");

		return _versionList[_activeVersion]->IsOwnedByTransferQueue(transferFrameIndice);
	}

	void WorldLayerVertexDataLayerVersionListInternal::SetOwnedByTransferQueue(size_t transferFrameIndice, Misc::Bool64Values newValue)
	{
		if (_activeVersion >= _versionList.size())
			throw std::runtime_error("WorldLayerVertexDataLayerVersionListInternal::GetLayersVersion Const Error: Program tried to get a non-existent version of the layer!");

		_versionList[_activeVersion]->SetOwnedByTransferQueue(transferFrameIndice, newValue);
	}

}