#include "MainDNIpch.h"
#include "UiVertexDataLayerVersionListInternal.h"

#include <VulkanSimplified/VSCommon/VSMemoryTypeProperties.h>

#include <limits>

#include "UiVertexDataLayerVersionInternal.h"

namespace JJs2DEngine
{
	UiVertexDataLayerVersionListInternal::UiVertexDataLayerVersionListInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists& dataBufferList,
		VS::MemoryObjectsList& memoryObjectsList, const std::vector<size_t>& versionsMaxVerticesList, size_t layersDepth) : _dataBufferList(dataBufferList),
		_memoryObjectsList(memoryObjectsList)
	{
		uint32_t allBuffersMask = std::numeric_limits<uint32_t>::max();
		uint64_t allBuffersSize = 0;

		uint64_t biggestBufferSize = 0;

		_versionList.reserve(versionsMaxVerticesList.size());
		for (uint64_t i = 0; i < versionsMaxVerticesList.size(); ++i)
		{
			_versionList.push_back(std::make_unique<UiVertexDataLayerVersionInternal>(textureDataList, dataBufferList, versionsMaxVerticesList[i], layersDepth));

			uint32_t buffersMask = _versionList.back()->GetBuffersMask();
			uint64_t buffersSize = _versionList.back()->GetMemorySize();
			uint64_t buffersAligment = _versionList.back()->GetMemoryAligment();

			allBuffersMask = allBuffersMask & buffersMask;

			uint64_t aligmentsMod = allBuffersSize % buffersAligment;
			if (aligmentsMod != 0)
			{
				allBuffersSize += buffersAligment - aligmentsMod;
			}
			allBuffersSize += buffersSize;

			if (buffersSize > biggestBufferSize)
				biggestBufferSize = buffersSize;
		}

		std::vector<VS::MemoryTypeProperties> acceptableTypes;
		acceptableTypes.reserve(3);
		acceptableTypes.push_back(VS::DEVICE_LOCAL);
		acceptableTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_VISIBLE | VS::HOST_COHERENT);
		acceptableTypes.push_back(VS::HOST_VISIBLE | VS::HOST_COHERENT);

		_vertexMemoryID = _memoryObjectsList.AllocateMemory(allBuffersSize, _versionList.size(), acceptableTypes, allBuffersMask, 0x10);

		for (uint64_t i = 0; i < _versionList.size(); ++i)
		{
			auto bufferID = _versionList[i]->GetVertexBufferID();

			_dataBufferList.BindVertexBuffer(bufferID, _vertexMemoryID, 0x10);
		}

		if (!_memoryObjectsList.IsMemoryMapped(_vertexMemoryID))
		{
			_stagingBufferID = _dataBufferList.AddStagingBuffer(biggestBufferSize, {});
			uint32_t stagingBuffersMemoryMask = _dataBufferList.GetStagingBuffersMemoryTypeMask(_stagingBufferID.value());
			uint64_t stagingBuffersMemorySize = _dataBufferList.GetStagingBuffersSize(_stagingBufferID.value());
			
			std::vector<VS::MemoryTypeProperties> acceptableStagingTypes;
			acceptableStagingTypes.reserve(2);
			acceptableStagingTypes.push_back(VS::HOST_VISIBLE | VS::HOST_COHERENT);
			acceptableStagingTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_VISIBLE | VS::HOST_COHERENT);

			_stagingMemoryID = _memoryObjectsList.AllocateMemory(stagingBuffersMemorySize, 1, acceptableStagingTypes, stagingBuffersMemoryMask);

			_dataBufferList.BindStagingBuffer(_stagingBufferID.value(), _stagingMemoryID.value());
		}

		_activeLayer = 0;
	}

	UiVertexDataLayerVersionListInternal::~UiVertexDataLayerVersionListInternal()
	{
		_versionList.clear();
		_memoryObjectsList.FreeMemory(_vertexMemoryID, false, false);
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