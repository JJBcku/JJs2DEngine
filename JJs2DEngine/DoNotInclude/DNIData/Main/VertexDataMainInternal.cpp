#include "MainDNIpch.h"
#include "VertexDataMainInternal.h"

#include "UiVertexDataLayerVersionListInternal.h"

#include "../Common/MaxDepthValue.h"

#include "../../../Include/Main/VertexLayerOrderID.h"

namespace JJs2DEngine
{
	constexpr size_t maxActiveLayersCount = maxLayerDepth + 1;

	VertexDataMainInternal::VertexDataMainInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists dataBufferList, VS::MemoryObjectsList memoryObjectsList,
		VS::SynchronizationDataLists synchroList, VS::CommandPoolQFGroupList transferQFGroup, uint32_t transferFrameAmount, size_t transferQueueID) :
		_textureDataList(textureDataList), _dataBufferList(dataBufferList), _memoryObjectsList(memoryObjectsList), _synchroList(synchroList), _transferQFGroup(transferQFGroup),
		_uiLayersList(maxActiveLayersCount)
	{
		_layerOrderList.reserve(maxActiveLayersCount);
		_transferFrameAmount = transferFrameAmount;

		if (_transferFrameAmount <= 0)
			throw std::runtime_error("VertexDataMainInternal::VertexDataMainInternal Error: Program tried to create zero transfer frames!");
		_currentTranferFrame = 0;

		_vertexTransferFinishedFences.reserve(transferFrameAmount);
		_vertexTransferFinshedSemaphores.reserve(transferFrameAmount);

		for (size_t i = 0; i < transferFrameAmount; ++i)
		{
			_vertexTransferFinishedFences.push_back(_synchroList.AddFence(true));
			_vertexTransferFinshedSemaphores.push_back(_synchroList.AddSemaphore());
		}

		_transferPoolID = _transferQFGroup.AddCommandPoolWithIndividualReset(true, transferQueueID, transferFrameAmount, 0);
		_transferPool.emplace(_transferQFGroup.GetCommandPoolWithIndividualReset(_transferPoolID));
		_transferCommandBuffersIDs = _transferPool->AllocatePrimaryCommandBuffers(static_cast<uint32_t>(transferFrameAmount));
	}

	VertexDataMainInternal::~VertexDataMainInternal()
	{
	}

	IDObject<UiVertexDataLayerVersionListPointer> VertexDataMainInternal::AddUiLayerVersionList(const std::vector<size_t>& versionsMaxVerticesList, size_t addOnReserving)
	{
		if (_layerOrderList.size() == _layerOrderList.capacity())
			throw std::runtime_error("VertexDataMainInternal::AddUiLayerVersionList Error: Program tried to add more layers than the program supports!");

		if (versionsMaxVerticesList.empty())
			throw std::runtime_error("VertexDataMainInternal::AddUiLayerVersionList Error: Program tried to create an empty version list!");

		auto ret = _uiLayersList.AddObject(std::make_unique<UiVertexDataLayerVersionListInternal>(_textureDataList, _dataBufferList, _memoryObjectsList,
			versionsMaxVerticesList, _layerOrderList.size(), _transferFrameAmount), addOnReserving);

		_layerOrderList.emplace_back(ret);

		return ret;
	}

	void VertexDataMainInternal::TransferVertexData()
	{
		for (size_t i = 0; i < _layerOrderList.size(); ++i)
		{
			if (_layerOrderList[i].type != VertexLayerOrderIDType::UI_LAYER)
				continue;

			auto& layer = _uiLayersList.GetObject(_layerOrderList[i].UiLayerID.ID);

			layer->WriteDataToBuffer(_currentTranferFrame);
		}

		_currentTranferFrame++;
		if (_currentTranferFrame >= _transferFrameAmount)
			_currentTranferFrame = 0;
	}

	UiVertexDataLayerVersionListInternal& VertexDataMainInternal::GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID)
	{
		return *_uiLayersList.GetObject(ID);
	}

	const UiVertexDataLayerVersionListInternal& VertexDataMainInternal::GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const
	{
		return *_uiLayersList.GetConstObject(ID);
	}

}