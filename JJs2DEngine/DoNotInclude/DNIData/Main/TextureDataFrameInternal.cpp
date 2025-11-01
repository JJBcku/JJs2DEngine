#include "MainDNIpch.h"
#include "TextureDataFrameInternal.h"

#include "TextureTransferOrderInternal.h"

#include <glm/vec2.hpp>

#include <VulkanSimplified/VSCommon/VSMemoryTypeProperties.h>
#include <VulkanSimplified/VSCommon/VSImageLayoutFlags.h>
#include <VulkanSimplified/VSCommon/VSAccessFlags.h>
#include <VulkanSimplified/VSCommon/VSPipelineStageFlags.h>

#include <VulkanSimplified/VSDevice/VSDataBuffersMemoryBarrierData.h>
#include <VulkanSimplified/VSDevice/VSGlobalMemoryBarrierData.h>
#include <VulkanSimplified/VSDevice/VSImagesMemoryBarrierData.h>

#include <limits>

namespace JJs2DEngine
{
	JJs2DEngine::TextureFrameInitData::TextureFrameInitData() : texturesMaxAmounts(), textureFormat()
	{
		frameAmount = 0;
		startingIndex = 0;
		max2DImageSize = 0;
		maxImageArrayLayers = 0;

		stagingBufferSize = 0;
	}

	TextureFrameInitData::~TextureFrameInitData()
	{
	}

	JJs2DEngine::TextureFrameStagingBufferData::TextureFrameStagingBufferData()
	{
		totalBufferSize = 0;
		currentlyUsedSize = std::numeric_limits<size_t>::max();
	}

	TextureFrameStagingBufferData::~TextureFrameStagingBufferData()
	{
	}

	JJs2DEngine::TextureFrameImageData::TextureFrameImageData()
	{
		tileSize = 0;
		widthInTiles = 0;
		heightInTiles = 0;
		widthInPixels = 0;
		heightInPixels = 0;
		layers = 0;
	}

	TextureFrameImageData::~TextureFrameImageData()
	{
	}

	TextureDataFrameInternal::TextureDataFrameInternal(const TextureFrameInitData& initData, VS::DataBufferLists dataBufferList, VS::ImageDataLists imageList,
		VS::MemoryObjectsList memoryList, const std::vector<VS::SecondaryIRCommandBuffer>& commandBuffersList) :
		_dataBufferList(dataBufferList), _imageList(imageList), _memoryList(memoryList), _commandBuffersList(commandBuffersList),
		_startingIndex(initData.startingIndex), _max2DImageSize(initData.max2DImageSize), _maxImageArrayLayers(initData.maxImageArrayLayers)
	{
		std::vector<VS::MemoryTypeProperties> acceptableMemoryTypes;

		{
			uint32_t memoryTypeMask = std::numeric_limits<uint32_t>::max();
			size_t totalSize = 0;

			for (size_t i = 0; i < _textureDataArray.size(); ++i)
			{
				auto& textureData = _textureDataArray[i];

				size_t tileSize = 1ULL << (skippedSizeLevels + i);
				textureData = CompileTextureFrameSizeData(tileSize, initData.texturesMaxAmounts[i], initData.max2DImageSize, initData.maxImageArrayLayers);

				textureData.imageIDs.reserve(initData.frameAmount);
				for (uint64_t j = 0; j < initData.frameAmount; ++j)
				{
					textureData.imageIDs.push_back(_imageList.Add2DArrayTextureImage(static_cast<uint32_t>(textureData.widthInPixels), static_cast<uint32_t>(textureData.heightInPixels),
						static_cast<uint32_t>(textureData.layers), 1, initData.textureFormat, {}, false, 1));
				}
				
				memoryTypeMask = memoryTypeMask & _imageList.Get2DArrayTextureImagesMemoryTypeMask(textureData.imageIDs[0]);
				totalSize += _imageList.Get2DArrayTextureImagesSize(textureData.imageIDs[0]) * textureData.imageIDs.size();
			}

			acceptableMemoryTypes.reserve(7);
			acceptableMemoryTypes.push_back(VS::DEVICE_LOCAL);
			acceptableMemoryTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_VISIBLE | VS::HOST_CACHED);
			acceptableMemoryTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_COHERENT | VS::HOST_VISIBLE | VS::HOST_CACHED);
			acceptableMemoryTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_COHERENT | VS::HOST_VISIBLE);
			acceptableMemoryTypes.push_back(VS::HOST_VISIBLE | VS::HOST_CACHED);
			acceptableMemoryTypes.push_back(VS::HOST_COHERENT | VS::HOST_VISIBLE | VS::HOST_CACHED);
			acceptableMemoryTypes.push_back(VS::HOST_COHERENT | VS::HOST_VISIBLE);

			_textureMemoryID = _memoryList.AllocateMemory(totalSize, _textureDataArray.size(), acceptableMemoryTypes, memoryTypeMask, 0x10);
			for (size_t i = 0; i < _textureDataArray.size(); ++i)
			{
				auto& textureData = _textureDataArray[i];
				textureData.imageViewIDs.reserve(textureData.imageIDs.size());
				for (uint64_t j = 0; j < textureData.imageIDs.size(); ++j)
				{
					_imageList.Bind2DArrayTextureImage(textureData.imageIDs[j], _textureMemoryID, _textureDataArray.size());
					textureData.imageViewIDs.push_back(_imageList.Add2DArrayTextureImageFullView(textureData.imageIDs[j], _textureDataArray.size()));
				}
			}
		}

		{
			_texturesStagingBufferFrames.resize(initData.frameAmount);
			for (uint64_t i = 0; i < initData.frameAmount; ++i)
			{
				_texturesStagingBufferFrames[i].stagingBufferID = (_dataBufferList.AddStagingBuffer(initData.stagingBufferSize, {}, 0x10));
				_texturesStagingBufferFrames[i].totalBufferSize = initData.stagingBufferSize;
				_texturesStagingBufferFrames[i].currentlyUsedSize = 0;
			}

			acceptableMemoryTypes.clear();
			acceptableMemoryTypes.push_back(VS::HOST_COHERENT | VS::HOST_VISIBLE | VS::HOST_CACHED);
			acceptableMemoryTypes.push_back(VS::HOST_COHERENT | VS::HOST_VISIBLE);
			acceptableMemoryTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_COHERENT | VS::HOST_VISIBLE | VS::HOST_CACHED);
			acceptableMemoryTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_COHERENT | VS::HOST_VISIBLE);

			size_t requiredSize = _dataBufferList.GetStagingBuffersSize(_texturesStagingBufferFrames[0].stagingBufferID) * _texturesStagingBufferFrames.size();
			uint32_t memoryMask = _dataBufferList.GetStagingBuffersMemoryTypeMask(_texturesStagingBufferFrames[0].stagingBufferID);

			_stagingBufferMemoryID = _memoryList.AllocateMemory(requiredSize, 1, acceptableMemoryTypes, memoryMask, 0x10);
			for (uint64_t i = 0; i < _texturesStagingBufferFrames.size(); ++i)
			{
				_dataBufferList.BindStagingBuffer(_texturesStagingBufferFrames[i].stagingBufferID, _stagingBufferMemoryID, 0x10);
			}
		}

		for (size_t i = 0; i < _textureDataArray.size(); ++i)
		{
			auto& textureData = _textureDataArray[i];
			textureData.textureReferencesList.resize(initData.texturesMaxAmounts[i]);

			TextureReferenceData reference;
			reference.textureIndex = static_cast<uint32_t>(_startingIndex + i);

			reference.textureSize = glm::vec2(static_cast<float>(textureData.tileSize) / static_cast<float>(textureData.widthInPixels),
				static_cast<float>(textureData.tileSize) / static_cast<float>(textureData.heightInPixels));

			for (size_t j = 0; j < textureData.textureReferencesList.size(); ++j)
			{
				textureData.textureReferencesList[j].resize(initData.frameAmount);

				size_t tileStartWidth = j % textureData.widthInTiles;
				tileStartWidth *= textureData.tileSize;
				size_t divTemp = j / textureData.widthInTiles;
				size_t tileStartHeight = divTemp % textureData.heightInTiles;
				tileStartHeight *= textureData.tileSize;
				size_t tileLayer = divTemp / textureData.heightInTiles;

				reference.textureCoords = glm::vec2(((static_cast<float>(tileStartWidth) * 2.0f) + 1.0f) / (static_cast<float>(textureData.widthInPixels) * 2.0f),
					((static_cast<float>(tileStartHeight) * 2.0f) + 1.0f) / (static_cast<float>(textureData.heightInPixels) * 2.0f));
				reference.textureLayer = static_cast<uint32_t>(tileLayer);
				reference.textureIsUsed = j == 0;

				for (size_t k = 0; k < initData.frameAmount; ++k)
				{
					textureData.textureReferencesList[j][k] = std::make_shared<TextureReferenceData>(reference);
				}
			}

			textureData.textureTransferOrderLists.resize(initData.frameAmount);
			for (size_t j = 0; j < textureData.textureTransferOrderLists.size(); ++j)
			{
				textureData.textureTransferOrderLists[j].reserve(initData.texturesMaxAmounts[i]);
			}
		}
	}

	TextureDataFrameInternal::~TextureDataFrameInternal()
	{
	}

	void TextureDataFrameInternal::LoadDefaultTextures(const std::array<std::vector<unsigned char>, imagesInTextureArray>& defaultTexturesData, uint64_t transferQueue, uint64_t graphicsQueue)
	{
		size_t offset = 0;

		for (size_t i = 0; i < defaultTexturesData.size(); ++i)
		{
			for (size_t j = 0; j < _texturesStagingBufferFrames.size(); ++j)
			{
				_dataBufferList.WriteToStagingBuffer(_texturesStagingBufferFrames[j].stagingBufferID, offset, *defaultTexturesData[i].data(), defaultTexturesData[i].size());
			}

			offset += defaultTexturesData[i].size();
		}

		std::vector<VS::ImagesMemoryBarrierData> toTransfer, fromTransfer;

		toTransfer.resize(_texturesStagingBufferFrames.size());
		for (size_t i = 0; i < toTransfer.size(); ++i)
		{
			toTransfer[i].srcAccess = VS::AccessFlagBits::ACCESS_NONE;
			toTransfer[i].dstAccess = VS::AccessFlagBits::ACCESS_TRANSFER_WRITE;
			toTransfer[i].oldLayout = VS::ImageLayoutFlags::UNDEFINED;
			toTransfer[i].newLayout = VS::ImageLayoutFlags::TRANSFER_DESTINATION;
		}

		fromTransfer.resize(_texturesStagingBufferFrames.size());
		for (size_t i = 0; i < toTransfer.size(); ++i)
		{
			fromTransfer[i].srcAccess = VS::AccessFlagBits::ACCESS_TRANSFER_WRITE;
			fromTransfer[i].dstAccess = VS::AccessFlagBits::ACCESS_SHADER_READ;
			fromTransfer[i].oldLayout = VS::ImageLayoutFlags::TRANSFER_DESTINATION;
			fromTransfer[i].newLayout = VS::ImageLayoutFlags::SHADER_READ_ONLY;

			if (transferQueue != graphicsQueue)
			{
				fromTransfer[i].queueData.emplace();
				fromTransfer[i].queueData->srcQueueIndex = transferQueue;
				fromTransfer[i].queueData->dstQueueIndex = graphicsQueue;
			}
		}

		offset = 0;

		_commandBuffersList[0].BeginRecording(VS::CommandBufferUsage::ONE_USE, {}, {}, false);

		for (size_t i = 0; i < defaultTexturesData.size(); ++i)
		{
			auto& textureData = _textureDataArray[i];

			for (size_t j = 0; j < toTransfer.size(); ++j)
			{
				toTransfer[j].imageID = VS::ImagesGenericID(textureData.imageIDs[j]);
			}
			_commandBuffersList[0].CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER, VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER, {}, {}, toTransfer);

			for (size_t j = 0; j < textureData.textureReferencesList.size(); ++j)
			{
				size_t tileStartWidth = j % textureData.widthInTiles;
				tileStartWidth *= textureData.tileSize;
				size_t divTemp = j / textureData.widthInTiles;
				size_t tileStartHeight = divTemp % textureData.heightInTiles;
				tileStartHeight *= textureData.tileSize;
				size_t tileLayer = divTemp / textureData.heightInTiles;

				for (size_t k = 0; k < toTransfer.size(); ++k)
				{
					_commandBuffersList[0].TransferDataTo2dArrayTextureSingleLayer(_texturesStagingBufferFrames[k].stagingBufferID, offset, defaultTexturesData[i].size(),
						textureData.imageIDs[k], static_cast<uint32_t>(tileStartWidth), static_cast<uint32_t>(tileStartHeight),
						static_cast<uint32_t>(textureData.tileSize), static_cast<uint32_t>(textureData.tileSize), 0, static_cast<uint32_t>(tileLayer));
				}
			}

			offset += defaultTexturesData[i].size();

			for (size_t j = 0; j < toTransfer.size(); ++j)
			{
				fromTransfer[j].imageID = VS::ImagesGenericID(textureData.imageIDs[j]);
			}
			if (transferQueue == graphicsQueue)
			{
				_commandBuffersList[0].CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER, VS::PipelineStageFlagBits::PIPELINE_STAGE_FRAGMENT_SHADER, {}, {}, fromTransfer);
			}
			else
			{
				_commandBuffersList[0].CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER, VS::PipelineStageFlagBits::PIPELINE_STAGE_TOP_OF_PIPE, {}, {}, fromTransfer);
			}
		}

		_commandBuffersList[0].EndRecording();
	}

	std::array<IDObject<VS::AutoCleanup2DArrayTexture>, imagesInTextureArray> TextureDataFrameInternal::GetImageIDs(size_t frameInFlightIndice) const
	{
		std::array<IDObject<VS::AutoCleanup2DArrayTexture>, imagesInTextureArray> ret;

		for (size_t i = 0; i < ret.size(); ++i)
		{
			ret[i] = _textureDataArray[i].imageIDs[frameInFlightIndice];
		}

		return ret;
	}

	std::array<IDObject<VS::AutoCleanupImageView>, imagesInTextureArray> TextureDataFrameInternal::GetImageViewIDs(size_t frameInFlightIndice) const
	{
		std::array<IDObject<VS::AutoCleanupImageView>, imagesInTextureArray> ret;

		for (size_t i = 0; i < ret.size(); ++i)
		{
			ret[i] = _textureDataArray[i].imageViewIDs[frameInFlightIndice];
		}

		return ret;
	}

	std::vector<std::shared_ptr<TextureReferenceData>> TextureDataFrameInternal::GetTextureReference(size_t tileImageIndex, size_t referenceIndex)
	{
		if (tileImageIndex >= _textureDataArray.size())
			throw std::runtime_error("TextureDataFrameInternal::GetTextureReference Error: Program tried to get reference from a non-existent tile image!");

		auto& imageData = _textureDataArray[tileImageIndex];
		if (referenceIndex >= imageData.textureReferencesList.size())
			throw std::runtime_error("TextureDataFrameInternal::GetTextureReference Error: Program tried to get a non-existent reference!");

		return imageData.textureReferencesList[referenceIndex];
	}

	void TextureDataFrameInternal::GetGraphicsToTransferMemoryBarriers(std::vector<VS::ImagesMemoryBarrierData>& outputVector, size_t frameInFlightIndice,
		uint64_t transferQueue, uint64_t graphicsQueue)
	{
		if (frameInFlightIndice >= _textureDataArray[0].imageIDs.size())
			throw std::runtime_error("TextureDataFrameInternal::GetGraphicsToTransferMemoryBarriers Error: Program tried to access a non-existent frame data!");

		VS::ImagesMemoryBarrierData added;

		added.srcAccess = VS::AccessFlagBits::ACCESS_MEMORY_READ;
		added.dstAccess = VS::AccessFlagBits::ACCESS_MEMORY_WRITE;
		added.oldLayout = VS::ImageLayoutFlags::SHADER_READ_ONLY;
		added.newLayout = VS::ImageLayoutFlags::TRANSFER_DESTINATION;

		if (transferQueue != graphicsQueue)
		{
			added.queueData.emplace();
			added.queueData->srcQueueIndex = graphicsQueue;
			added.queueData->dstQueueIndex = transferQueue;
		}

		for (size_t i = 0; i < _textureDataArray.size(); ++i)
		{
			added.imageID = _textureDataArray[i].imageIDs[frameInFlightIndice];

			outputVector.push_back(added);
		}
	}

	void TextureDataFrameInternal::GetTransferToGraphicsMemoryBarriers(std::vector<VS::ImagesMemoryBarrierData>& outputVector, size_t frameInFlightIndice,
		uint64_t transferQueue, uint64_t graphicsQueue)
	{
		if (frameInFlightIndice >= _textureDataArray[0].imageIDs.size())
			throw std::runtime_error("TextureDataFrameInternal::GetTransferToGraphicsMemoryBarrier Error: Program tried to access a non-existent frame data!");

		VS::ImagesMemoryBarrierData added;

		added.srcAccess = VS::AccessFlagBits::ACCESS_TRANSFER_WRITE;
		added.dstAccess = VS::AccessFlagBits::ACCESS_SHADER_READ;
		added.oldLayout = VS::ImageLayoutFlags::TRANSFER_DESTINATION;
		added.newLayout = VS::ImageLayoutFlags::SHADER_READ_ONLY;

		if (transferQueue != graphicsQueue)
		{
			added.queueData.emplace();
			added.queueData->srcQueueIndex = transferQueue;
			added.queueData->dstQueueIndex = graphicsQueue;
		}

		for (size_t i = 0; i < _textureDataArray.size(); ++i)
		{
			added.imageID = _textureDataArray[i].imageIDs[frameInFlightIndice];

			outputVector.push_back(added);
		}
	}

	bool TextureDataFrameInternal::AreAllFramesTransferOrderListsEmpty(size_t frameInFlightIndice) const
	{
		if (frameInFlightIndice >= _commandBuffersList.size())
			throw std::runtime_error("TextureDataFrameInternal::AreAllFramesTransferOrderListsEmpty Error: Program tried to access a non-existent frame data!");

		for (size_t i = 0; i < _textureDataArray.size(); ++i)
		{
			const auto& textureData = _textureDataArray[i];

			if (!textureData.textureTransferOrderLists[frameInFlightIndice].empty())
				return false;
		}

		return true;
	}

	std::optional<std::pair<size_t, size_t>> TextureDataFrameInternal::TryToAddTextureToTransferList(const unsigned char& data, size_t dataSize, uint32_t width, uint32_t height)
	{
		std::optional<std::pair<size_t, size_t>> ret;

		if (GetLeastStagingMemoryUnused() < dataSize)
			return ret;

		std::optional<size_t> arraysIndex;
		{
			uint32_t biggerSide = std::max(width, height);

			if (biggerSide > _textureDataArray.back().tileSize)
				throw std::runtime_error("TextureDataFrameInternal::TryToAddTextureToTransferList Error: Program was given texture bigger than biggest tile!");

			for (size_t i = 0; i < _textureDataArray.size(); ++i)
			{
				if (biggerSide <= _textureDataArray[i].tileSize)
				{
					arraysIndex = i;
					break;
				}
			}

			if (!arraysIndex.has_value())
				throw std::runtime_error("TextureDataFrameInternal::TryToAddTextureToTransferList Error: Program failed to find an appropriate tiled texture image!");
		}

		auto& textureImageData = _textureDataArray[arraysIndex.value()];

		std::optional<size_t> insertionIndex;
		for (size_t i = 1; i < textureImageData.textureReferencesList.size(); ++i)
		{
			if (!textureImageData.textureReferencesList[i][0]->textureIsUsed)
			{
				insertionIndex = i;
				break;
			}
		}

		if (!insertionIndex.has_value())
			throw std::runtime_error("TextureDataFrameInternal::TryToAddTextureToTransferList Error: Program failed to find an appropriate tile!");

		TextureTransferOrderInternal addedOrder;
		addedOrder.insertionIndex = insertionIndex.value();
		addedOrder.texturesWidth = width;
		addedOrder.texturesHeight = height;

		for (size_t i = 0; i < _texturesStagingBufferFrames.size(); ++i)
		{
			addedOrder.stagingBufferDataOffset = _texturesStagingBufferFrames[i].currentlyUsedSize;
			addedOrder.stagingBufferDataSize = dataSize;

			_texturesStagingBufferFrames[i].currentlyUsedSize += addedOrder.stagingBufferDataSize;

			_dataBufferList.WriteToStagingBuffer(_texturesStagingBufferFrames[i].stagingBufferID, addedOrder.stagingBufferDataOffset, data, addedOrder.stagingBufferDataSize);

			if (textureImageData.textureTransferOrderLists[i].size() == textureImageData.textureTransferOrderLists[i].capacity())
				textureImageData.textureTransferOrderLists[i].reserve(textureImageData.textureTransferOrderLists[i].capacity() << 1);
			textureImageData.textureTransferOrderLists[i].push_back(addedOrder);

			textureImageData.textureReferencesList[insertionIndex.value()][i]->textureIsUsed = true;
		}

		ret.emplace(arraysIndex.value(), insertionIndex.value());

		return ret;
	}

	void TextureDataFrameInternal::RecordTransferBuffer(size_t frameInFlightIndice, uint64_t transferQueue, uint64_t graphicsQueue)
	{
		if (frameInFlightIndice >= _commandBuffersList.size())
			throw std::runtime_error("TextureDataFrameInternal::RecordTransferBuffer Error: Program tried to access a non-existent frame data!");

		auto& transferCommandBuffer = _commandBuffersList[frameInFlightIndice];

		transferCommandBuffer.BeginRecording(VS::CommandBufferUsage::ONE_USE, {}, {}, false);

		std::vector<VS::ImagesMemoryBarrierData> fromGraphics, toGraphics;

		fromGraphics.reserve(_textureDataArray.size());
		GetGraphicsToTransferMemoryBarriers(fromGraphics, frameInFlightIndice, transferQueue, graphicsQueue);
		if (transferQueue == graphicsQueue)
		{
			transferCommandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_FRAGMENT_SHADER, VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER, {}, {}, fromGraphics);
		}
		else
		{
			transferCommandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_BOTTOM_OF_PIPE, VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER, {}, {}, fromGraphics);
		}

		for (size_t i = 0; i < _textureDataArray.size(); ++i)
		{
			auto& textureData = _textureDataArray[i];

			for (size_t j = 0; j < textureData.textureTransferOrderLists[frameInFlightIndice].size(); ++j)
			{
				const auto& transferOrder = textureData.textureTransferOrderLists[frameInFlightIndice][j];

				size_t tileStartWidth = transferOrder.insertionIndex % textureData.widthInTiles;
				tileStartWidth *= textureData.tileSize;
				size_t divTemp = transferOrder.insertionIndex / textureData.widthInTiles;
				size_t tileStartHeight = divTemp % textureData.heightInTiles;
				tileStartHeight *= textureData.tileSize;
				size_t tileLayer = divTemp / textureData.heightInTiles;

				transferCommandBuffer.TransferDataTo2dArrayTextureSingleLayer(_texturesStagingBufferFrames[frameInFlightIndice].stagingBufferID, transferOrder.stagingBufferDataOffset,
					transferOrder.stagingBufferDataSize, textureData.imageIDs[frameInFlightIndice], static_cast<uint32_t>(tileStartWidth), static_cast<uint32_t>(tileStartHeight),
					transferOrder.texturesWidth, transferOrder.texturesHeight, 0, static_cast<uint32_t>(tileLayer));
			}

			textureData.textureTransferOrderLists[frameInFlightIndice].clear();
		}

		toGraphics.reserve(_textureDataArray.size());
		GetTransferToGraphicsMemoryBarriers(toGraphics, frameInFlightIndice, transferQueue, graphicsQueue);
		if (transferQueue == graphicsQueue)
		{
			transferCommandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER, VS::PipelineStageFlagBits::PIPELINE_STAGE_FRAGMENT_SHADER, {}, {}, toGraphics);
		}
		else
		{
			transferCommandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER, VS::PipelineStageFlagBits::PIPELINE_STAGE_TOP_OF_PIPE, {}, {}, toGraphics);
		}

		transferCommandBuffer.EndRecording();
	}

	TextureFrameImageData TextureDataFrameInternal::CompileTextureFrameSizeData(size_t tileSize, size_t texturesMaxAmount, uint64_t max2DImageSize, uint64_t maxImageArrayLayers) const
	{
		TextureFrameImageData ret;

		ret.tileSize = tileSize;

		size_t stripeMaxCount = max2DImageSize / ret.tileSize;
		size_t layerMaxTilesCount = stripeMaxCount * stripeMaxCount;

		bool found = false;

		if (!found && texturesMaxAmount <= stripeMaxCount)
		{
			ret.widthInTiles = texturesMaxAmount;
			ret.heightInTiles = 1;
			ret.widthInPixels = ret.widthInTiles * ret.tileSize;
			ret.heightInPixels = ret.tileSize;
			ret.layers = 1;
			found = true;
		}

		if (!found && texturesMaxAmount <= maxImageArrayLayers)
		{
			ret.widthInTiles = 1;
			ret.heightInTiles = 1;
			ret.widthInPixels = ret.tileSize;
			ret.heightInPixels = ret.tileSize;
			ret.layers = texturesMaxAmount;
			found = true;
		}

		size_t stripeCount = texturesMaxAmount / stripeMaxCount;
		size_t stripeCountMod = texturesMaxAmount % stripeMaxCount;

		if (!found && stripeCountMod == 0 && (stripeCount <= stripeMaxCount || stripeCount <= maxImageArrayLayers))
		{
			ret.widthInTiles = stripeMaxCount;
			ret.widthInPixels = stripeMaxCount * ret.tileSize;
			if (stripeCount <= stripeMaxCount)
			{
				ret.heightInTiles = stripeCount;
				ret.heightInPixels = ret.heightInTiles * ret.tileSize;
				ret.layers = 1;
			}
			else
			{
				ret.heightInTiles = 1;
				ret.heightInPixels = ret.tileSize;
				ret.layers = stripeCount;
			}
			found = true;
		}

		if (!found)
		{
			for (size_t layersInTexture = 2; layersInTexture < maxImageArrayLayers; ++layersInTexture)
			{
				size_t imagesInLayer = texturesMaxAmount / layersInTexture;
				size_t imagesInLayerMod = texturesMaxAmount % layersInTexture;

				if (imagesInLayerMod != 0)
					continue;

				for (size_t heightTiles = 2; heightTiles <= imagesInLayer / 2; ++heightTiles)
				{
					if (heightTiles > stripeMaxCount)
						break;

					size_t widthTiles = imagesInLayer / heightTiles;
					size_t widthTilesMod = imagesInLayer % heightTiles;

					if (widthTilesMod == 0 && widthTiles <= stripeMaxCount)
					{
						ret.widthInTiles = widthTiles;
						ret.heightInTiles = heightTiles;
						ret.widthInPixels = widthTiles * ret.tileSize;
						ret.heightInPixels = heightTiles * ret.tileSize;
						ret.layers = layersInTexture;

						found = true;
						break;
					}
				}

				if (found)
					break;
			}
		}

		if (!found)
		{
			size_t oversizedTileStripesCount = (texturesMaxAmount / stripeMaxCount) + 1;

			if (!found && oversizedTileStripesCount <= stripeMaxCount)
			{
				ret.widthInTiles = stripeMaxCount;
				ret.heightInTiles = oversizedTileStripesCount;
				ret.widthInPixels = stripeMaxCount * ret.tileSize;
				ret.heightInPixels = oversizedTileStripesCount * ret.tileSize;
				ret.layers = 1;
				found = true;
			}

			if (!found && oversizedTileStripesCount <= maxImageArrayLayers)
			{
				ret.widthInTiles = stripeMaxCount;
				ret.heightInTiles = 1;
				ret.widthInPixels = stripeMaxCount * ret.tileSize;
				ret.heightInPixels = ret.tileSize;
				ret.layers = oversizedTileStripesCount;
				found = true;
			}

			size_t oversizedLayerCount = (texturesMaxAmount / layerMaxTilesCount) + 1;
			if (!found && oversizedLayerCount <= maxImageArrayLayers)
			{
				ret.widthInTiles = stripeMaxCount;
				ret.heightInTiles = stripeMaxCount;
				ret.widthInPixels = stripeMaxCount * ret.tileSize;
				ret.heightInPixels = ret.widthInPixels;
				ret.layers = oversizedLayerCount;
				found = true;
			}
		}

		if (!found)
			throw std::runtime_error("TextureDataFrameInternal::CompileTextureFrameSizeData Error: Program failed to find the proper size for an image!");

		return ret;
	}

	uint64_t TextureDataFrameInternal::GetLeastStagingMemoryUnused() const
	{
		uint64_t ret = std::numeric_limits<size_t>::max();

		assert(!_texturesStagingBufferFrames.empty());
		for (size_t i = 0; i < _texturesStagingBufferFrames.size(); ++i)
		{
			assert(_texturesStagingBufferFrames[i].totalBufferSize >= _texturesStagingBufferFrames[i].currentlyUsedSize);
			uint64_t unusedSize = _texturesStagingBufferFrames[i].totalBufferSize - _texturesStagingBufferFrames[i].currentlyUsedSize;

			if (unusedSize <= ret)
				ret = unusedSize;
		}

		return ret;
	}

}