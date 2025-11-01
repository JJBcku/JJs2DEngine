#include "MainDNIpch.h"
#include "VertexDataMainInternal.h"

#include "BackgroundVertexDataLayerVersionListInternal.h"
#include "UiVertexDataLayerVersionListInternal.h"
#include "WorldLayerVertexDataLayerVersionListInternal.h"

#include "../Common/MaxDepthValue.h"

#include "../../../Include/Main/VertexLayerOrderID.h"

#include "TextureDataMainInternal.h"
#include "RenderDataInternal.h"
#include "WindowDataInternal.h"

#include <VulkanSimplified/VSCommon/VSPipelineStageFlags.h>

#include <VulkanSimplified/VSDevice/VSCommandBufferSubmissionData.h>
#include <VulkanSimplified/VSDevice/VSDataBuffersMemoryBarrierData.h>
#include <VulkanSimplified/VSDevice/VSGlobalMemoryBarrierData.h>
#include <VulkanSimplified/VSDevice/VSImagesMemoryBarrierData.h>
#include <VulkanSimplified/VSDevice/VSDescriptorPoolGenericID.h>

#include <Miscellaneous/Bool64.h>

#include <glm/gtc/matrix_transform.hpp>

namespace JJs2DEngine
{
	constexpr size_t maxActiveLayersCount = maxLayerDepth + 1;

	VertexDataMainInternal::VertexDataMainInternal(TextureDataMainInternal& textureDataList, RenderDataInternal& renderDataList, WindowDataInternal& windowDataList,
		VS::DataBufferLists dataBufferList, VS::MemoryObjectsList memoryObjectsList, VS::SynchronizationDataLists synchroList,
		VS::CommandPoolQFGroupList transferQFGroup, uint32_t transferFrameAmount, size_t transferQueueID,
		VS::CommandPoolQFGroupList graphicsQFGroup, uint32_t graphicsFrameAmount, size_t graphicsQueueID) : _textureDataList(textureDataList), _renderDataList(renderDataList),
		_windowDataList(windowDataList), _dataBufferList(dataBufferList), _memoryObjectsList(memoryObjectsList), _synchroList(synchroList),
		_transferQFGroup(transferQFGroup), _graphicsQFGroup(graphicsQFGroup), _uiLayersList(maxActiveLayersCount), _worldLayersList(maxActiveLayersCount)
	{
		_layerOrderList.reserve(maxActiveLayersCount);

		if (transferFrameAmount <= 0)
			throw std::runtime_error("VertexDataMainInternal::VertexDataMainInternal Error: Program tried to create zero transfer frames!");
		_transferFrameAmount = transferFrameAmount;
		_currentTransferFrame = 0;

		if (graphicsFrameAmount <= 0)
			throw std::runtime_error("VertexDataMainInternal::VertexDataMainInternal Error: Program tried to create zero graphics frames!");
		_graphicsFrameAmount = graphicsFrameAmount;
		_currentGraphicsFrame = 0;

		_transferQueueID = transferQueueID;
		_graphicsQueueID = graphicsQueueID;

		_gammaValue = 2.0f;

		_vertexTransferFinishedFences.reserve(transferFrameAmount);
		_vertexTransferFinishedSemaphores.reserve(transferFrameAmount);
		_lastGraphicsFrameUsingThisTransferFrame.resize(transferFrameAmount);

		for (size_t i = 0; i < transferFrameAmount; ++i)
		{
			_vertexTransferFinishedFences.push_back(_synchroList.AddFence(true));
			_vertexTransferFinishedSemaphores.push_back(_synchroList.AddSemaphore());
		}

		_renderingFinishedFences.reserve(graphicsFrameAmount);
		_texturesQueueTrasferFinishedFences.reserve(graphicsFrameAmount);
		_imageAcquiredSemaphores.reserve(graphicsFrameAmount);
		_renderingFinishedSemaphores.reserve(graphicsFrameAmount);
		_transferDataFreeToChangeSemaphores.reserve(graphicsFrameAmount);
		_texturesQueueTrasferFinishedSemaphores.reserve(graphicsFrameAmount);

		for (size_t i = 0; i < graphicsFrameAmount; ++i)
		{
			_renderingFinishedFences.push_back(_synchroList.AddFence(true));
			_texturesQueueTrasferFinishedFences.push_back(_synchroList.AddFence(false));
			_imageAcquiredSemaphores.push_back(_synchroList.AddSemaphore());
			_renderingFinishedSemaphores.push_back(_synchroList.AddSemaphore());
			_transferDataFreeToChangeSemaphores.push_back(_synchroList.AddSemaphore());
			_texturesQueueTrasferFinishedSemaphores.push_back(_synchroList.AddSemaphore());
		}

		_transferPoolID = _transferQFGroup.AddCommandPoolWithIndividualReset(true, transferQueueID, transferFrameAmount, 0);
		_transferPool.emplace(_transferQFGroup.GetCommandPoolWithIndividualReset(_transferPoolID));
		_transferCommandBuffersIDs = _transferPool->AllocatePrimaryCommandBuffers(transferFrameAmount);

		_graphicsPoolID = _graphicsQFGroup.AddCommandPoolWithIndividualReset(false, graphicsQueueID, graphicsFrameAmount, 0);
		_graphicsPool.emplace(_graphicsQFGroup.GetCommandPoolWithIndividualReset(_graphicsPoolID));
		_graphicsCommandBuffersIDs = _graphicsPool->AllocatePrimaryCommandBuffers(graphicsFrameAmount);
	}

	VertexDataMainInternal::~VertexDataMainInternal()
	{
	}

	void VertexDataMainInternal::CreateBackgroundLayerVersionList(const std::vector<BackgroundObjectData>& versionDataList)
	{
		if (_backgroundLayerVersionList != nullptr)
			throw std::runtime_error("VertexDataMainInternal::CreateBackgroundLayerVersionList Error: Program tried to create already existing background layer version list!");

		_backgroundLayerVersionList = std::make_unique<BackgroundVertexDataLayerVersionListInternal>(_dataBufferList, _memoryObjectsList, _textureDataList, versionDataList,
			_transferFrameAmount);
	}

	IDObject<UiVertexDataLayerVersionListPointer> VertexDataMainInternal::AddUiLayerVersionList(const std::vector<size_t>& versionsMaxObjectAmountsList, size_t addOnReserving)
	{
		if (_layerOrderList.size() == _layerOrderList.capacity())
			throw std::runtime_error("VertexDataMainInternal::AddUiLayerVersionList Error: Program tried to add more layers than the program supports!");

		if (versionsMaxObjectAmountsList.empty())
			throw std::runtime_error("VertexDataMainInternal::AddUiLayerVersionList Error: Program tried to create an empty version list!");

		auto ret = _uiLayersList.AddObject(std::make_unique<UiVertexDataLayerVersionListInternal>(_textureDataList, _dataBufferList, _memoryObjectsList,
			versionsMaxObjectAmountsList, _layerOrderList.size(), _transferFrameAmount), addOnReserving);

		_layerOrderList.emplace_back(ret);

		return ret;
	}

	IDObject<WorldLayerVertexDataLayerVersionListPointer> VertexDataMainInternal::AddWorldLayerVersionList(const std::vector<size_t>& versionsMaxObjectAmountsList, size_t addOnReserving)
	{
		if (_layerOrderList.size() == _layerOrderList.capacity())
			throw std::runtime_error("VertexDataMainInternal::AddWorldLayerVersionList Error: Program tried to add more layers than the program supports!");

		if (versionsMaxObjectAmountsList.empty())
			throw std::runtime_error("VertexDataMainInternal::AddWorldLayerVersionList Error: Program tried to create an empty version list!");

		auto ret = _worldLayersList.AddObject(std::make_unique<WorldLayerVertexDataLayerVersionListInternal>(_textureDataList, _dataBufferList, _memoryObjectsList,
			versionsMaxObjectAmountsList, _layerOrderList.size(), _transferFrameAmount), addOnReserving);

		_layerOrderList.emplace_back(ret);

		return ret;
	}

	void VertexDataMainInternal::PreRenderingTexturesOwnershipTransfer()
	{
		if (_graphicsQueueID == _transferQueueID)
			return;

		auto graphicsCommandBuffer = _graphicsPool->GetPrimaryCommandBuffer(_graphicsCommandBuffersIDs[0]);

		_synchroList.ResetFences({ _renderingFinishedFences[0] });

		graphicsCommandBuffer.ResetCommandBuffer(false);
		graphicsCommandBuffer.BeginRecording(VS::CommandBufferUsage::ONE_USE);

		auto preLoadedImageBarriers = _textureDataList.GetPreLoadedTransferToGraphicsMemoryBarriers(_transferQueueID, _graphicsQueueID);
		graphicsCommandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_BOTTOM_OF_PIPE, VS::PipelineStageFlagBits::PIPELINE_STAGE_FRAGMENT_SHADER,
			{}, {}, preLoadedImageBarriers);

		if (_textureDataList.AreStreamedTextureCreated())
		{
			for (size_t i = 0; i < _transferFrameAmount; ++i)
			{
				auto streamedImageBarriers = _textureDataList.GetStreamedTransferToGraphicsMemoryBarriers(i, _transferQueueID, _graphicsQueueID);

				graphicsCommandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_BOTTOM_OF_PIPE, VS::PipelineStageFlagBits::PIPELINE_STAGE_FRAGMENT_SHADER,
					{}, {}, streamedImageBarriers);
			}
		}

		graphicsCommandBuffer.EndRecording();

		VS::CommandBufferSubmissionData submitData;
		submitData.commandBufferIDs.resize(1);
		submitData.commandBufferIDs[0].IRPrimaryID.type = VS::CommandBufferIDType::IR_PRIMARY;
		submitData.commandBufferIDs[0].IRPrimaryID.commandPoolID = _graphicsPoolID;
		submitData.commandBufferIDs[0].IRPrimaryID.commandBufferID = _graphicsCommandBuffersIDs[0];

		_graphicsQFGroup.SubmitBuffers(_graphicsQueueID, { submitData }, _renderingFinishedFences[0]);

		if (_synchroList.WaitOnFences({ _renderingFinishedFences[0] }, false, 1'000'000'000ULL) != true)
			throw std::runtime_error("VertexDataMainInternal::PreRenderingTexturesOwnershipTransfer Error: Waiting on fence has timed out!");
	}

	void VertexDataMainInternal::TransferVertexData()
	{
		if (_currentTransferFrame >= _transferCommandBuffersIDs.size())
			throw std::runtime_error("VertexDataMainInternal::TransferVertexData Error: Program tried to use a non-existent transfer frame!");

		if (_synchroList.WaitOnFences({ _vertexTransferFinishedFences[_currentTransferFrame] }, false, 1'000'000'000ULL) != true)
			throw std::runtime_error("VertexDataMainInternal::TransferVertexData Error: Waiting on fence has timed out!");

		_synchroList.ResetFences({ _vertexTransferFinishedFences[_currentTransferFrame] });

		auto tranferCommandBuffer = _transferPool->GetPrimaryCommandBuffer(_transferCommandBuffersIDs[_currentTransferFrame]);
		tranferCommandBuffer.ResetCommandBuffer(false);
		tranferCommandBuffer.BeginRecording(VS::CommandBufferUsage::ONE_USE);

		std::vector<VS::DataBuffersMemoryBarrierData> vertexBuffersOwnershipTransferDataList;
		vertexBuffersOwnershipTransferDataList.reserve(_layerOrderList.size() + 1);

		if (_backgroundLayerVersionList != nullptr)
		{
			auto& layer = *_backgroundLayerVersionList;

			bool commandRecorded = layer.WriteDataToBuffer(_currentTransferFrame, tranferCommandBuffer);
			if (commandRecorded && _transferQueueID != _graphicsQueueID)
			{
				vertexBuffersOwnershipTransferDataList.push_back(layer.GetOwnershipTransferData(_currentTransferFrame, _transferQueueID, _graphicsQueueID));
			}
		}

		for (size_t i = 0; i < _layerOrderList.size(); ++i)
		{
			if (_layerOrderList[i].type != VertexLayerOrderIDType::UI_LAYER)
				continue;

			auto& layer = _uiLayersList.GetObject(_layerOrderList[i].uiLayerID.ID);

			bool commandRecorded = layer->WriteDataToBuffer(_currentTransferFrame, tranferCommandBuffer);
			if (commandRecorded && _transferQueueID != _graphicsQueueID)
			{
				vertexBuffersOwnershipTransferDataList.push_back(layer->GetOwnershipTransferData(_currentTransferFrame, _transferQueueID, _graphicsQueueID));
			}
		}

		for (size_t i = 0; i < _layerOrderList.size(); ++i)
		{
			if (_layerOrderList[i].type != VertexLayerOrderIDType::WORLD_LAYER)
				continue;

			auto& layer = _worldLayersList.GetObject(_layerOrderList[i].worldLayerID.ID);

			bool commandRecorded = layer->WriteDataToBuffer(_currentTransferFrame, tranferCommandBuffer);
			if (commandRecorded && _transferQueueID != _graphicsQueueID)
			{
				vertexBuffersOwnershipTransferDataList.push_back(layer->GetOwnershipTransferData(_currentTransferFrame, _transferQueueID, _graphicsQueueID));
			}
		}

		if (!vertexBuffersOwnershipTransferDataList.empty())
		{
			tranferCommandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER, VS::PipelineStageFlagBits::PIPELINE_STAGE_TOP_OF_PIPE,
				{}, vertexBuffersOwnershipTransferDataList, {});
		}

		tranferCommandBuffer.EndRecording();

		VS::CommandBufferSubmissionData submitData;
		submitData.commandBufferIDs.resize(1);
		submitData.commandBufferIDs[0].IRPrimaryID.type = VS::CommandBufferIDType::IR_PRIMARY;
		submitData.commandBufferIDs[0].IRPrimaryID.commandPoolID = _transferPoolID;
		submitData.commandBufferIDs[0].IRPrimaryID.commandBufferID = _transferCommandBuffersIDs[_currentTransferFrame];

		submitData.signalSemaphores.push_back(_vertexTransferFinishedSemaphores[_currentTransferFrame]);
		if (_lastGraphicsFrameUsingThisTransferFrame[_currentTransferFrame].has_value())
		{
			submitData.waitSemaphores.emplace_back(_transferDataFreeToChangeSemaphores[_lastGraphicsFrameUsingThisTransferFrame[_currentTransferFrame].value()], VS::PIPELINE_STAGE_TRANSFER);
		}

		_transferQFGroup.SubmitBuffers(_transferQueueID, { submitData }, _vertexTransferFinishedFences[_currentTransferFrame]);
	}

	void VertexDataMainInternal::DrawFrame()
	{
		if (_currentGraphicsFrame >= _graphicsCommandBuffersIDs.size())
			throw std::runtime_error("VertexDataMainInternal::DrawFrame Error: Program tried to use a non-existent graphics frame!");

		bool streamedTexturesTransfered = _textureDataList.AreStreamedTextureCreated() && TransferStreamedTextures();

		if (streamedTexturesTransfered)
		{
			if (_synchroList.WaitOnFences({ _texturesQueueTrasferFinishedFences[_currentGraphicsFrame] }, false, 1'000'000'000ULL) != true)
				throw std::runtime_error("VertexDataMainInternal::DrawFrame Error: Waiting on queue transfer fence has timed out!");
			_synchroList.ResetFences({ _texturesQueueTrasferFinishedFences[_currentGraphicsFrame] });
		}
		else
		{
			if (_synchroList.WaitOnFences({ _renderingFinishedFences[_currentGraphicsFrame] }, false, 1'000'000'000ULL) != true)
				throw std::runtime_error("VertexDataMainInternal::DrawFrame Error: Waiting on rendering finished fence has timed out!");
			_synchroList.ResetFences({ _renderingFinishedFences[_currentGraphicsFrame] });
		}

		uint32_t nextImagesIndice = std::numeric_limits<uint32_t>::max();
		
		if (_windowDataList.AcquireNextImage(1'000'000'000ULL, _imageAcquiredSemaphores[_currentGraphicsFrame], {}, nextImagesIndice) != true)
			throw std::runtime_error("VertexDataMainInternal::DrawFrame Error: Waiting on image acquiring has timed out!");

		auto graphicsCommandBuffer = _graphicsPool->GetPrimaryCommandBuffer(_graphicsCommandBuffersIDs[_currentGraphicsFrame]);

		graphicsCommandBuffer.ResetCommandBuffer(false);
		graphicsCommandBuffer.BeginRecording(VS::CommandBufferUsage::ONE_USE);

		if (streamedTexturesTransfered && _graphicsQueueID != _transferQueueID)
		{
			auto toGraphics = _textureDataList.GetStreamedTransferToGraphicsMemoryBarriers(_currentTransferFrame, _transferQueueID, _graphicsQueueID);
			graphicsCommandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_BOTTOM_OF_PIPE, VS::PipelineStageFlagBits::PIPELINE_STAGE_FRAGMENT_SHADER,
				{}, {}, toGraphics);
		}

		std::vector<VS::DataBuffersMemoryBarrierData> vertexBuffersOwnershipTransferDataList;

		if (_graphicsQueueID != _transferQueueID)
		{
			vertexBuffersOwnershipTransferDataList.reserve(_layerOrderList.size() + 1);

			if (_backgroundLayerVersionList != nullptr)
			{
				auto& layer = *_backgroundLayerVersionList;

				if (layer.IsOwnedByTransferQueue(_currentTransferFrame) == Misc::BOOL64_TRUE)
				{
					vertexBuffersOwnershipTransferDataList.push_back(layer.GetOwnershipTransferData(_currentTransferFrame, _transferQueueID, _graphicsQueueID));
					layer.SetOwnedByTransferQueue(_currentTransferFrame, Misc::BOOL64_FALSE);
				}
			}

			for (size_t i = 0; i < _layerOrderList.size(); ++i)
			{
				if (_layerOrderList[i].type != VertexLayerOrderIDType::UI_LAYER)
					continue;

				auto& layer = _uiLayersList.GetObject(_layerOrderList[i].uiLayerID.ID);

				if (layer->IsOwnedByTransferQueue(_currentTransferFrame) == Misc::BOOL64_TRUE)
				{
					vertexBuffersOwnershipTransferDataList.push_back(layer->GetOwnershipTransferData(_currentTransferFrame, _transferQueueID, _graphicsQueueID));
					layer->SetOwnedByTransferQueue(_currentTransferFrame, Misc::BOOL64_FALSE);
				}
			}

			for (size_t i = 0; i < _layerOrderList.size(); ++i)
			{
				if (_layerOrderList[i].type != VertexLayerOrderIDType::WORLD_LAYER)
					continue;

				auto& layer = _worldLayersList.GetObject(_layerOrderList[i].worldLayerID.ID);

				if (layer->IsOwnedByTransferQueue(_currentTransferFrame) == Misc::BOOL64_TRUE)
				{
					vertexBuffersOwnershipTransferDataList.push_back(layer->GetOwnershipTransferData(_currentTransferFrame, _transferQueueID, _graphicsQueueID));
					layer->SetOwnedByTransferQueue(_currentTransferFrame, Misc::BOOL64_FALSE);
				}
			}
		}

		if (!vertexBuffersOwnershipTransferDataList.empty())
		{
			graphicsCommandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_BOTTOM_OF_PIPE, VS::PipelineStageFlagBits::PIPELINE_STAGE_VERTEX_INPUT,
				{}, vertexBuffersOwnershipTransferDataList, {});
		}

		graphicsCommandBuffer.BeginRenderPass(_renderDataList.GetCurrentRenderPass(), _windowDataList.GetFramebufferID(_currentGraphicsFrame), 0, 0, _windowDataList.GetRenderWidth(),
			_windowDataList.GetRenderHeight(), _renderDataList.GetClearValuesList());

		if (_backgroundLayerVersionList != nullptr)
		{
			graphicsCommandBuffer.BindGraphicsPipeline(_renderDataList.GetBackgroundLayerGraphicsPipeline());

			graphicsCommandBuffer.BindDescriptorSetsToGraphicsPipeline(_renderDataList.GetBackgroundLayerGraphicsPipelineLayout(), 0,
				_textureDataList.GetTexturesDescriptorSetPool(), { _textureDataList.GetTexturesDescriptorSets(_currentTransferFrame) }, {});

			auto& layer = *_backgroundLayerVersionList;

			layer.RecordDrawCommand(_currentTransferFrame, graphicsCommandBuffer);
		}

		if (_uiLayersList.GetUsedSize() != 0)
		{
			graphicsCommandBuffer.BindGraphicsPipeline(_renderDataList.GetUILayerGraphicsPipeline());

			graphicsCommandBuffer.BindDescriptorSetsToGraphicsPipeline(_renderDataList.GetUILayerGraphicsPipelineLayout(), 0, _textureDataList.GetTexturesDescriptorSetPool(),
				{ _textureDataList.GetTexturesDescriptorSets(_currentTransferFrame) }, {});

			for (size_t i = 0; i < _layerOrderList.size(); ++i)
			{
				if (_layerOrderList[i].type != VertexLayerOrderIDType::UI_LAYER)
					continue;

				auto& layer = _uiLayersList.GetObject(_layerOrderList[i].uiLayerID.ID);

				layer->RecordDrawCommand(_currentTransferFrame, graphicsCommandBuffer);
			}
		}

		if (_worldLayersList.GetUsedSize() != 0)
		{
			graphicsCommandBuffer.BindGraphicsPipeline(_renderDataList.GetWorldLayerGraphicsPipeline());

			graphicsCommandBuffer.BindDescriptorSetsToGraphicsPipeline(_renderDataList.GetWorldLayerGraphicsPipelineLayout(), 0, _textureDataList.GetTexturesDescriptorSetPool(),
				{ _textureDataList.GetTexturesDescriptorSets(_currentTransferFrame) }, {});

			std::vector<unsigned char> cameraData;
			cameraData.resize(sizeof(_camera));
			std::memcpy(cameraData.data(), &_camera, sizeof(_camera));

			graphicsCommandBuffer.PushConstants(_renderDataList.GetWorldLayerGraphicsPipelineLayout(), VS::ShaderTypeFlagBit::SHADER_TYPE_VERTEX, 0, cameraData);

			for (size_t i = 0; i < _layerOrderList.size(); ++i)
			{
				if (_layerOrderList[i].type != VertexLayerOrderIDType::WORLD_LAYER)
					continue;

				auto& layer = _worldLayersList.GetObject(_layerOrderList[i].worldLayerID.ID);

				layer->RecordDrawCommand(_currentTransferFrame, graphicsCommandBuffer);
			}
		}

		graphicsCommandBuffer.BeginNextSubpass(false);

		graphicsCommandBuffer.BindGraphicsPipeline(_renderDataList.GetGammaCorrectionGraphicsPipeline());

		graphicsCommandBuffer.BindDescriptorSetsToGraphicsPipeline(_renderDataList.GetGammaCorrectionGraphicsPipelineLayout(), 0, _windowDataList.GetGammaCorrectionDescriptorPool(),
			{ _windowDataList.GetGammaCorrectionDescriptorSet(_currentGraphicsFrame) }, {});

		std::vector<unsigned char> pushData;
		pushData.resize(sizeof(_gammaValue));
		std::memcpy(pushData.data(), &_gammaValue, sizeof(_gammaValue));

		graphicsCommandBuffer.PushConstants(_renderDataList.GetGammaCorrectionGraphicsPipelineLayout(), VS::ShaderTypeFlagBit::SHADER_TYPE_FRAGMENT, 0, pushData);

		graphicsCommandBuffer.Draw(6, 1, 0, 0);

		graphicsCommandBuffer.EndRenderPass();

		graphicsCommandBuffer.TransitionSwapchainImageToTrasferDestination(_windowDataList.GetWindowID(), {}, nextImagesIndice);
		graphicsCommandBuffer.BlitColorRenderTargetToSwapchainImage(_windowDataList.GetWindowID(), _windowDataList.GetColorRenderTargetImage(_currentGraphicsFrame), 0, 0,
			_windowDataList.GetRenderWidth(), _windowDataList.GetRenderHeight(), nextImagesIndice);
		graphicsCommandBuffer.TransitionSwapchainImageToPresent(_windowDataList.GetWindowID(), {}, nextImagesIndice);

		graphicsCommandBuffer.EndRecording();

		VS::CommandBufferSubmissionData submitData;
		submitData.commandBufferIDs.resize(1);
		submitData.commandBufferIDs[0].IRPrimaryID.type = VS::CommandBufferIDType::IR_PRIMARY;
		submitData.commandBufferIDs[0].IRPrimaryID.commandPoolID = _graphicsPoolID;
		submitData.commandBufferIDs[0].IRPrimaryID.commandBufferID = _graphicsCommandBuffersIDs[_currentGraphicsFrame];

		submitData.waitSemaphores.reserve(2);
		submitData.waitSemaphores.emplace_back(_vertexTransferFinishedSemaphores[_currentTransferFrame], VS::PIPELINE_STAGE_VERTEX_INPUT);
		if (streamedTexturesTransfered)
			submitData.waitSemaphores.emplace_back(_textureDataList.GetTransferFinishedSemaphore(_currentTransferFrame), VS::PIPELINE_STAGE_FRAGMENT_SHADER);
		submitData.waitSemaphores.emplace_back(_imageAcquiredSemaphores[_currentGraphicsFrame], VS::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT);

		submitData.signalSemaphores.reserve(2);
		submitData.signalSemaphores.push_back(_renderingFinishedSemaphores[nextImagesIndice]);
		submitData.signalSemaphores.push_back(_transferDataFreeToChangeSemaphores[_currentGraphicsFrame]);

		_lastGraphicsFrameUsingThisTransferFrame[_currentTransferFrame] = _currentGraphicsFrame;

		_graphicsQFGroup.SubmitBuffers(_graphicsQueueID, { submitData }, _renderingFinishedFences[_currentGraphicsFrame]);

		_graphicsPool->PresentSwapchainToQueue(_windowDataList.GetWindowID(), { _renderingFinishedSemaphores[nextImagesIndice] }, nextImagesIndice);
	}

	void VertexDataMainInternal::IncrementCurrentFrames()
	{
		_currentTransferFrame++;
		if (_currentTransferFrame >= _transferFrameAmount)
			_currentTransferFrame = 0;

		_currentGraphicsFrame++;
		if (_currentGraphicsFrame >= _graphicsFrameAmount)
			_currentGraphicsFrame = 0;
	}

	void VertexDataMainInternal::TransferPreLoadedTextures()
	{
		if (_synchroList.WaitOnFences({ _renderingFinishedFences[0] }, false, 1'000'000'000ULL) != true)
			throw std::runtime_error("VertexDataMainInternal::TransferPreLoadedTextures Error: First waiting on a fence has timed out!");
		_synchroList.ResetFences({ _renderingFinishedFences[0] });

		auto graphicsCommandBuffer = _graphicsPool->GetPrimaryCommandBuffer(_graphicsCommandBuffersIDs[0]);
		VS::CommandBufferSubmissionData submitData;
		submitData.commandBufferIDs.resize(1);
		submitData.commandBufferIDs[0].IRPrimaryID.type = VS::CommandBufferIDType::IR_PRIMARY;
		submitData.commandBufferIDs[0].IRPrimaryID.commandPoolID = _graphicsPoolID;
		submitData.commandBufferIDs[0].IRPrimaryID.commandBufferID = _graphicsCommandBuffersIDs[0];

		submitData.signalSemaphores.push_back(_renderingFinishedSemaphores[0]);

		graphicsCommandBuffer.ResetCommandBuffer(false);
		graphicsCommandBuffer.BeginRecording(VS::CommandBufferUsage::ONE_USE);

		if (_transferQueueID != _graphicsQueueID)
		{
			auto fromGraphics = _textureDataList.GetPreLoadedGraphicsToTransferMemoryBarriers(_transferQueueID, _graphicsQueueID);
			graphicsCommandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_FRAGMENT_SHADER, VS::PipelineStageFlagBits::PIPELINE_STAGE_TOP_OF_PIPE,
				{}, {}, fromGraphics);
		}

		graphicsCommandBuffer.EndRecording();

		_graphicsQFGroup.SubmitBuffers(_graphicsQueueID, { submitData }, _renderingFinishedFences[0]);

		_textureDataList.SetTextureUseFinishedSemaphore(0, _renderingFinishedSemaphores[0]);

		_textureDataList.TransferPreLoadedTexturesData(_transferQueueID, _graphicsQueueID);

		if (_synchroList.WaitOnFences({ _renderingFinishedFences[0] }, false, 1'000'000'000ULL) != true)
			throw std::runtime_error("VertexDataMainInternal::TransferPreLoadedTextures Error: Second waiting on a fence has timed out!");
		_synchroList.ResetFences({ _renderingFinishedFences[0] });

		submitData.signalSemaphores.clear();
		submitData.waitSemaphores.emplace_back(_textureDataList.GetTransferFinishedSemaphore(0), VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER);

		graphicsCommandBuffer.ResetCommandBuffer(false);
		graphicsCommandBuffer.BeginRecording(VS::CommandBufferUsage::ONE_USE);

		if (_transferQueueID != _graphicsQueueID)
		{
			auto toGraphics = _textureDataList.GetPreLoadedTransferToGraphicsMemoryBarriers(_transferQueueID, _graphicsQueueID);
			graphicsCommandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_BOTTOM_OF_PIPE, VS::PipelineStageFlagBits::PIPELINE_STAGE_FRAGMENT_SHADER,
				{}, {}, toGraphics);
		}

		graphicsCommandBuffer.EndRecording();

		_graphicsQFGroup.SubmitBuffers(_graphicsQueueID, { submitData }, _renderingFinishedFences[0]);

		if (_synchroList.WaitOnFences({ _renderingFinishedFences[0] }, false, 1'000'000'000ULL) != true)
			throw std::runtime_error("VertexDataMainInternal::TransferPreLoadedTextures Error: Third waiting on a fence has timed out!");
	}

	void VertexDataMainInternal::SetGammaValue(float newGammaValue)
	{
		_gammaValue = newGammaValue;
	}

	float VertexDataMainInternal::GetGammaValue() const
	{
		return _gammaValue;
	}

	void VertexDataMainInternal::SetCameraPosition(float X, float Y)
	{
		_camera.position = glm::vec2(X, Y);
	}

	void VertexDataMainInternal::SetPerspectiveRotation(float rotation)
	{
		if (rotation == 0.0f)
		{
			_camera.perspectiveRotation = glm::identity<glm::mat4x4>();
			return;
		}

		_camera.perspectiveRotation = glm::rotate(glm::identity<glm::mat4x4>(), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	void VertexDataMainInternal::SetCameraZoom(float zoom)
	{
		_camera.zoom = zoom;
	}

	void VertexDataMainInternal::SetCameraAspectRatio(float ratio)
	{
		_camera.aspectRatio = ratio;
	}

	BackgroundVertexDataLayerVersionListInternal& VertexDataMainInternal::GetBackgroundVertexDataLayerVersionList()
	{
		if (_backgroundLayerVersionList == nullptr)
			throw std::runtime_error("VertexDataMainInternal::CreateBackgroundLayerVersionList Error: Program tried to access a non-existent backgrond layers version list!");

		return *_backgroundLayerVersionList;
	}

	UiVertexDataLayerVersionListInternal& VertexDataMainInternal::GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID)
	{
		return *_uiLayersList.GetObject(ID);
	}

	WorldLayerVertexDataLayerVersionListInternal& VertexDataMainInternal::GetWorldLayerVertexDataLayerVersionList(IDObject<WorldLayerVertexDataLayerVersionListPointer> ID)
	{
		return *_worldLayersList.GetObject(ID);
	}

	const BackgroundVertexDataLayerVersionListInternal& VertexDataMainInternal::GetBackgroundVertexDataLayerVersionList() const
	{
		if (_backgroundLayerVersionList == nullptr)
			throw std::runtime_error("VertexDataMainInternal::CreateBackgroundLayerVersionList Const Error: Program tried to access a non-existent backgrond layers version list!");

		return *_backgroundLayerVersionList;
	}

	const UiVertexDataLayerVersionListInternal& VertexDataMainInternal::GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const
	{
		return *_uiLayersList.GetConstObject(ID);
	}

	const WorldLayerVertexDataLayerVersionListInternal& VertexDataMainInternal::GetWorldLayerVertexDataLayerVersionList(IDObject<WorldLayerVertexDataLayerVersionListPointer> ID) const
	{
		return *_worldLayersList.GetConstObject(ID);
	}

	bool VertexDataMainInternal::TransferStreamedTextures()
	{
		if (!_textureDataList.AreStreamedTextureCreated() || _textureDataList.AreAllFramesStreamedTransferOrderListsEmpty(_currentTransferFrame))
			return false;

		if (_synchroList.WaitOnFences({ _renderingFinishedFences[_currentGraphicsFrame] }, false, 1'000'000'000ULL) != true)
			throw std::runtime_error("VertexDataMainInternal::TransferPreLoadedTextures Error: First waiting on a fence has timed out!");
		_synchroList.ResetFences({ _renderingFinishedFences[_currentGraphicsFrame] });

		auto graphicsCommandBuffer = _graphicsPool->GetPrimaryCommandBuffer(_graphicsCommandBuffersIDs[_currentGraphicsFrame]);
		VS::CommandBufferSubmissionData submitData;
		submitData.commandBufferIDs.resize(1);
		submitData.commandBufferIDs[0].IRPrimaryID.type = VS::CommandBufferIDType::IR_PRIMARY;
		submitData.commandBufferIDs[0].IRPrimaryID.commandPoolID = _graphicsPoolID;
		submitData.commandBufferIDs[0].IRPrimaryID.commandBufferID = _graphicsCommandBuffersIDs[_currentGraphicsFrame];

		submitData.signalSemaphores.push_back(_texturesQueueTrasferFinishedSemaphores[_currentGraphicsFrame]);

		graphicsCommandBuffer.ResetCommandBuffer(false);
		graphicsCommandBuffer.BeginRecording(VS::CommandBufferUsage::ONE_USE);

		if (_transferQueueID != _graphicsQueueID)
		{
			auto fromGraphics = _textureDataList.GetStreamedGraphicsToTransferMemoryBarriers(_currentTransferFrame, _transferQueueID, _graphicsQueueID);
			graphicsCommandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_FRAGMENT_SHADER, VS::PipelineStageFlagBits::PIPELINE_STAGE_TOP_OF_PIPE,
				{}, {}, fromGraphics);
		}

		graphicsCommandBuffer.EndRecording();

		_graphicsQFGroup.SubmitBuffers(_graphicsQueueID, { submitData }, _texturesQueueTrasferFinishedFences[_currentGraphicsFrame]);

		_textureDataList.SetTextureUseFinishedSemaphore(_currentTransferFrame, _texturesQueueTrasferFinishedSemaphores[_currentGraphicsFrame]);

		_textureDataList.TransferStreamedTexturesData(_currentTransferFrame, _transferQueueID, _graphicsQueueID);

		return true;
	}

}