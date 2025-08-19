#include "MainDNIpch.h"
#include "TextureDataFrameInternal.h"

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
		startingIndex = 0;
		max2DImageSize = 0;
		maxImageArrayLayers = 0;

		stagingBufferSize = 0;
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
		VS::MemoryObjectsList memoryList, VS::SecondaryNIRCommandBuffer commandBuffer) :
		_dataBufferList(dataBufferList), _imageList(imageList), _memoryList(memoryList), _commandBuffer(commandBuffer),
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

				textureData.imageID = _imageList.Add2DArrayTextureImage(static_cast<uint32_t>(textureData.widthInPixels), static_cast<uint32_t>(textureData.heightInPixels),
					static_cast<uint32_t>(textureData.layers), 1, initData.textureFormat, {}, false, 1);
				memoryTypeMask = memoryTypeMask & _imageList.Get2DArrayTextureImagesMemoryTypeMask(textureData.imageID);
				totalSize += _imageList.Get2DArrayTextureImagesSize(textureData.imageID);
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
				_imageList.Bind2DArrayTextureImage(textureData.imageID, _textureMemoryID, _textureDataArray.size());
				textureData.imageViewID = _imageList.Add2DArrayTextureImageFullView(textureData.imageID, _textureDataArray.size());
			}
		}

		{
			_texturesStagingBufferID = _dataBufferList.AddStagingBuffer(initData.stagingBufferSize, {}, 0x10);

			acceptableMemoryTypes.clear();
			acceptableMemoryTypes.push_back(VS::HOST_VISIBLE | VS::HOST_CACHED);
			acceptableMemoryTypes.push_back(VS::HOST_COHERENT | VS::HOST_VISIBLE | VS::HOST_CACHED);
			acceptableMemoryTypes.push_back(VS::HOST_COHERENT | VS::HOST_VISIBLE);
			acceptableMemoryTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_VISIBLE | VS::HOST_CACHED);
			acceptableMemoryTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_COHERENT | VS::HOST_VISIBLE | VS::HOST_CACHED);
			acceptableMemoryTypes.push_back(VS::DEVICE_LOCAL | VS::HOST_COHERENT | VS::HOST_VISIBLE);

			size_t requiredSize = _dataBufferList.GetStagingBuffersSize(_texturesStagingBufferID);
			uint32_t memoryMask = _dataBufferList.GetStagingBuffersMemoryTypeMask(_texturesStagingBufferID);

			_stagingBufferMemoryID = _memoryList.AllocateMemory(requiredSize, 1, acceptableMemoryTypes, memoryMask, 0x10);
			_dataBufferList.BindStagingBuffer(_texturesStagingBufferID, _stagingBufferMemoryID, 0x10);
		}

		for (size_t i = 0; i < _textureDataArray.size(); ++i)
		{
			auto& textureData = _textureDataArray[i];
			textureData.textureReferencesList.resize(initData.texturesMaxAmounts[i]);

			TextureReferenceData defaultReference;
			defaultReference.textureCoords = glm::vec2(0.0f);
			defaultReference.textureSize = glm::vec2(1.0f);
			defaultReference.textureLayer = 0;
			defaultReference.textureIndex = static_cast<uint32_t>(_startingIndex + i);

			for (size_t j = 0; j < textureData.textureReferencesList.size(); ++j)
			{
				textureData.textureReferencesList[j] = std::make_shared<TextureReferenceData>(defaultReference);
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
			_dataBufferList.WriteToStagingBuffer(_texturesStagingBufferID, offset, *defaultTexturesData[i].data(), defaultTexturesData[i].size());

			offset += defaultTexturesData[i].size();
		}

		VS::ImagesMemoryBarrierData toTransfer, fromTransfer;

		toTransfer.srcAccess = VS::AccessFlagBits::ACCESS_NONE;
		toTransfer.dstAccess = VS::AccessFlagBits::ACCESS_TRANSFER_WRITE;
		toTransfer.oldLayout = VS::ImageLayoutFlags::UNDEFINED;
		toTransfer.newLayout = VS::ImageLayoutFlags::TRANSFER_DESTINATION;

		fromTransfer.srcAccess = VS::AccessFlagBits::ACCESS_TRANSFER_WRITE;
		fromTransfer.dstAccess = VS::AccessFlagBits::ACCESS_SHADER_READ;
		fromTransfer.oldLayout = VS::ImageLayoutFlags::TRANSFER_DESTINATION;
		fromTransfer.newLayout = VS::ImageLayoutFlags::SHADER_READ_ONLY;

		if (transferQueue != graphicsQueue)
		{
			fromTransfer.queueData.emplace();
			fromTransfer.queueData->srcQueueIndex = transferQueue;
			fromTransfer.queueData->dstQueueIndex = graphicsQueue;
		}

		offset = 0;

		_commandBuffer.BeginRecording(VS::CommandBufferUsage::ONE_USE, {}, {}, false);

		for (size_t i = 0; i < defaultTexturesData.size(); ++i)
		{
			auto& textureData = _textureDataArray[i];

			toTransfer.imageID = VS::ImagesGenericID(textureData.imageID);
			_commandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_TOP_OF_PIPE, VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER, {}, {}, { toTransfer });

			_commandBuffer.TransferDataTo2dArrayTextureSingleLayer(_texturesStagingBufferID, offset, defaultTexturesData[i].size(), textureData.imageID,
				0, 0, static_cast<uint32_t>(textureData.tileSize), static_cast<uint32_t>(textureData.tileSize), 0, 0);

			offset += defaultTexturesData[i].size();

			fromTransfer.imageID = VS::ImagesGenericID(textureData.imageID);
			_commandBuffer.CreatePipelineBarrier(VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER, VS::PipelineStageFlagBits::PIPELINE_STAGE_BOTTOM_OF_PIPE, {}, {}, { fromTransfer });
		}

		_commandBuffer.EndRecording();
	}

	std::array<IDObject<VS::AutoCleanup2DArrayTexture>, imagesInTextureArray> TextureDataFrameInternal::GetImageIDs() const
	{
		std::array<IDObject<VS::AutoCleanup2DArrayTexture>, imagesInTextureArray> ret;

		for (size_t i = 0; i < ret.size(); ++i)
		{
			ret[i] = _textureDataArray[i].imageID;
		}

		return ret;
	}

	std::array<IDObject<VS::AutoCleanupImageView>, imagesInTextureArray> TextureDataFrameInternal::GetImageViewIDs() const
	{
		std::array<IDObject<VS::AutoCleanupImageView>, imagesInTextureArray> ret;

		for (size_t i = 0; i < ret.size(); ++i)
		{
			ret[i] = _textureDataArray[i].imageViewID;
		}

		return ret;
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

}