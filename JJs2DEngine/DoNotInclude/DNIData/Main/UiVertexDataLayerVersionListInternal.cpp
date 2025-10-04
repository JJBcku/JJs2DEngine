#include "MainDNIpch.h"
#include "UiVertexDataLayerVersionListInternal.h"

#include <VulkanSimplified/VSCommon/VSMemoryTypeProperties.h>

#include <limits>

#include "UiVertexDataLayerVersionInternal.h"

namespace JJs2DEngine
{
	UiVertexDataLayerVersionListInternal::UiVertexDataLayerVersionListInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists& dataBufferList,
		VS::MemoryObjectsList& memoryObjectsList, const std::vector<size_t>& versionsMaxVerticesList, size_t layersDepth, size_t transferFrameAmount) : _dataBufferList(dataBufferList),
		_memoryObjectsList(memoryObjectsList)
	{
		uint32_t allBuffersMask = std::numeric_limits<uint32_t>::max();
		uint64_t allBuffersSize = 0;

		uint64_t biggestBufferSize = 0;

		_versionList.reserve(versionsMaxVerticesList.size());
		for (uint64_t i = 0; i < versionsMaxVerticesList.size(); ++i)
		{
			_versionList.push_back(std::make_unique<UiVertexDataLayerVersionInternal>(textureDataList, dataBufferList, versionsMaxVerticesList[i], layersDepth, transferFrameAmount));

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
			_stagingBufferIDs.emplace();
			_stagingBufferIDs->reserve(transferFrameAmount);

			for (size_t i = 0; i < transferFrameAmount; ++i)
			{
				_stagingBufferIDs->push_back(_dataBufferList.AddStagingBuffer(biggestBufferSize, {}));
			}
			
			uint32_t stagingBuffersMemoryMask = _dataBufferList.GetStagingBuffersMemoryTypeMask(_stagingBufferIDs.value()[0]);
			uint64_t stagingBuffersMemorySize = _dataBufferList.GetStagingBuffersSize(_stagingBufferIDs.value()[0]);
			uint64_t stagingBuffersMemoryAligment = _dataBufferList.GetStagingBuffersRequiredAligment(_stagingBufferIDs.value()[0]);
			uint64_t stagingBuffersMemorySizeMod = stagingBuffersMemorySize % stagingBuffersMemoryAligment;
			if (stagingBuffersMemorySizeMod != 0)
				stagingBuffersMemorySize += stagingBuffersMemoryAligment - stagingBuffersMemorySizeMod;
			stagingBuffersMemorySize *= _stagingBufferIDs->size();
			
			std::vector<VS::MemoryTypeProperties> acceptableStagingTypes;
			acceptableStagingTypes.reserve(2);
			acceptableStagingTypes.push_back(VS::HOST_VISIBLE | VS::HOST_COHERENT);
			acceptableStagingTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_VISIBLE | VS::HOST_COHERENT);

			_stagingMemoryID = _memoryObjectsList.AllocateMemory(stagingBuffersMemorySize, transferFrameAmount, acceptableStagingTypes, stagingBuffersMemoryMask);

			for (size_t i = 0; i < transferFrameAmount; ++i)
			{
				_dataBufferList.BindStagingBuffer(_stagingBufferIDs.value()[i], _stagingMemoryID.value());
			}
		}

		_activeLayer = 0;
	}

	UiVertexDataLayerVersionListInternal::~UiVertexDataLayerVersionListInternal()
	{
		_versionList.clear();
		_memoryObjectsList.FreeMemory(_vertexMemoryID, false, false);

		if (_stagingBufferIDs.has_value())
		{
			for (size_t i = 0; i < _stagingBufferIDs->size(); ++i)
			{
				_dataBufferList.RemoveStagingBuffer(_stagingBufferIDs.value()[i], false);
			}
		}

		if (_stagingMemoryID.has_value())
		{
			_memoryObjectsList.FreeMemory(_stagingMemoryID.value(), false, false);
		}
	}

	void UiVertexDataLayerVersionListInternal::WriteDataToBuffer(size_t transferFrameIndice)
	{
		if (_activeLayer >= _versionList.size())
			throw std::runtime_error("UiVertexDataLayerVersionListInternal::WriteDataToBuffer Error: Program tried to access a non-existent layer version!");

		auto& layer = _versionList[_activeLayer];
		if (_stagingBufferIDs.has_value())
		{
			if (transferFrameIndice >= _stagingBufferIDs->size())
				throw std::runtime_error("UiVertexDataLayerVersionListInternal::WriteDataToBuffer Error: Program tried to access an non-existent frame's data!");

			layer->WriteDataToBuffer(_stagingBufferIDs.value()[transferFrameIndice], transferFrameIndice);
		}
		else
		{
			layer->WriteDataToBuffer({}, transferFrameIndice);
		}
	}

	UiVertexDataLayerVersionInternal& UiVertexDataLayerVersionListInternal::GetLayersVersion(size_t versionIndex)
	{
		if (versionIndex >= _versionList.size())
			throw std::runtime_error("UiVertexDataLayerVersionListInternal::GetLayersVersion Error: Program tried to get a non-existent version of the layer!");

		return *_versionList[versionIndex];
	}

	const UiVertexDataLayerVersionInternal& UiVertexDataLayerVersionListInternal::GetLayersVersion(size_t versionIndex) const
	{
		if (versionIndex >= _versionList.size())
			throw std::runtime_error("UiVertexDataLayerVersionListInternal::GetLayersVersion Const Error: Program tried to get a non-existent version of the layer!");

		return *_versionList[versionIndex];
	}

}