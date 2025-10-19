#include "MainDNIpch.h"
#include "TextureDataMainInternal.h"

#include "../../../Include/Common/TextureFormat.h"

#include "TextureDataFrameInternal.h"
#include "DeviceSettingsInternal.h"

#include <VulkanSimplified/VSCommon/VSDescriptorTypeFlags.h>

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>
#include <VulkanSimplified/VSDevice/VSIRCommandPool.h>
#include <VulkanSimplified/VSDevice/VSPrimaryIRCommandBuffer.h>
#include <VulkanSimplified/VSDevice/VSSecondaryIRCommandBuffer.h>
#include <VulkanSimplified/VSDevice/VSCommandBufferSubmissionData.h>
#include <VulkanSimplified/VSDevice/VSCommandBufferGenericID.h>
#include <VulkanSimplified/VSDevice/VSDescriptorPoolGenericID.h>
#include <VulkanSimplified/VSDevice/VSDescriptorSetCombined2DArrayTextureSamplerWriteData.h>

#include <Miscellaneous/Bool64.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <limits>

namespace JJs2DEngine
{
	TextureDataMainInitData::TextureDataMainInitData() : preLoadedTexturesMaxAmounts(), streamedTexturesMaxAmounts()
	{
		transferQueueID = std::numeric_limits<uint64_t>::max();
		graphicsQueueID = std::numeric_limits<uint64_t>::max();

		transferFramesInFlight = 0;
		max2DImageSize = 0;
		maxImageArrayLayers = 0;

		preLoadedTexturesStagingBufferPageCount = 0;
		streamedTexturesStagingBufferPageCount = 0;
		textureFormat = TextureFormat::TEXTURE_FORMAT_UNSET;
	}

	TextureDataMainInternal::TextureDataMainInternal(const TextureDataMainInitData& initData, VS::DataBufferLists dataBufferList, VS::ImageDataLists imageList,
		VS::MemoryObjectsList memoryList, VS::SynchronizationDataLists synchroList, VS::CommandPoolQFGroupList transferQFGroup, VS::DescriptorDataLists descriptorDataList) :
		_dataBufferList(dataBufferList), _imageList(imageList), _memoryList(memoryList), _synchroList(synchroList), _transferQFGroup(transferQFGroup),
		_descriptorDataList(descriptorDataList)
	{
		bool streamedTexturesUsed = false;

		for (size_t i = 0; i < initData.streamedTexturesMaxAmounts.size(); ++i)
		{
			if (initData.streamedTexturesMaxAmounts[i] > 0)
				streamedTexturesUsed = true;
		}

		std::array<size_t, imagesInTextureArray> _preLoadedTexturesMaxAmounts = initData.preLoadedTexturesMaxAmounts;
		std::array<size_t, imagesInTextureArray> _streamedTexturesMaxAmounts = initData.streamedTexturesMaxAmounts;

		for (size_t i = 0; i < _preLoadedTexturesMaxAmounts.size(); ++i)
		{
			_preLoadedTexturesMaxAmounts[i] += 1;
		}

		for (size_t i = 0; i < _streamedTexturesMaxAmounts.size(); ++i)
		{
			_streamedTexturesMaxAmounts[i] += 1;
		}

		_textureCommandPoolID = _transferQFGroup.AddCommandPoolWithIndividualReset(true, initData.transferQueueID, 1, 1 + initData.transferFramesInFlight);
		auto textureCommandPool = _transferQFGroup.GetCommandPoolWithIndividualReset(_textureCommandPoolID);

		_primaryCommandBufferIDs = textureCommandPool.AllocatePrimaryCommandBuffers(static_cast<uint32_t>(initData.transferFramesInFlight));

		auto preloadedCommandBuffers = textureCommandPool.AllocateSecondaryCommandBuffers(1, 0x10);
		_preLoadedCommandBufferID = preloadedCommandBuffers.back();

		if (streamedTexturesUsed)
		{
			_streamedCommandBufferIDs = textureCommandPool.AllocateSecondaryCommandBuffers(static_cast<uint32_t>(initData.transferFramesInFlight));
		}

		VS::DataFormatSetIndependentID format = TranslateToFormat(initData.textureFormat);

		bool is16Bit = Is16Bit(initData.textureFormat);
		bool isRBReversed = IsRBReversed(initData.textureFormat);

		if (is16Bit)
		{
			_textureFormatIs16Bit = Misc::BOOL64_TRUE;
		}
		else
		{
			_textureFormatIs16Bit = Misc::BOOL64_FALSE;
		}

		if (isRBReversed)
		{
			_textureFormatIsRBReversed = Misc::BOOL64_TRUE;
		}
		else
		{
			_textureFormatIsRBReversed = Misc::BOOL64_FALSE;
		}

		size_t preLoadedTexturesStagingBuferSize = static_cast<size_t>(biggestLevelTilePixelCount) * 2 * initData.preLoadedTexturesStagingBufferPageCount;

		if (is16Bit)
			preLoadedTexturesStagingBuferSize *= 8;
		else
			preLoadedTexturesStagingBuferSize *= 4;

		TextureFrameInitData frameInitData;
		frameInitData.frameAmount = 1;
		frameInitData.startingIndex = 0ULL;
		frameInitData.max2DImageSize = initData.max2DImageSize;
		frameInitData.maxImageArrayLayers = initData.maxImageArrayLayers;
		frameInitData.texturesMaxAmounts = _preLoadedTexturesMaxAmounts;
		frameInitData.stagingBufferSize = preLoadedTexturesStagingBuferSize;
		frameInitData.textureFormat = format;

		std::vector<VS::SecondaryIRCommandBuffer> preLoadedSecondaryBuffers;
		preLoadedSecondaryBuffers.push_back(textureCommandPool.GetSecondaryCommandBuffer(_preLoadedCommandBufferID));

		_preLoadedTexturesData = std::make_unique<TextureDataFrameInternal>(frameInitData, _dataBufferList, _imageList, _memoryList,
			preLoadedSecondaryBuffers);

		if (streamedTexturesUsed)
		{
			if (initData.streamedTexturesStagingBufferPageCount == 0)
				throw std::runtime_error("TextureDataMainInternal::TextureDataMainInternal Error: Value of streamed textures buffers pages count cannot be zero if streamed textures are being used");
			size_t streamedTexturesStagingBuferSize = static_cast<size_t>(biggestLevelTilePixelCount) * 2 * initData.streamedTexturesStagingBufferPageCount;

			if (is16Bit)
				streamedTexturesStagingBuferSize *= 8;
			else
				streamedTexturesStagingBuferSize *= 4;

			frameInitData.frameAmount = initData.transferFramesInFlight;
			frameInitData.startingIndex = imagesInTextureArray;
			frameInitData.texturesMaxAmounts = _streamedTexturesMaxAmounts;
			frameInitData.stagingBufferSize = streamedTexturesStagingBuferSize;

			std::vector<VS::SecondaryIRCommandBuffer> streamedSecondaryBuffers;
			streamedSecondaryBuffers.reserve(_streamedCommandBufferIDs.size());

			for (uint64_t i = 0; i < _streamedCommandBufferIDs.size(); ++i)
			{
				streamedSecondaryBuffers.push_back(textureCommandPool.GetSecondaryCommandBuffer(_streamedCommandBufferIDs[i]));
			}

			_streamedTexturesData = std::make_unique<TextureDataFrameInternal>(frameInitData, _dataBufferList, _imageList, _memoryList, streamedSecondaryBuffers);
		}

		std::array<std::vector<unsigned char>, imagesInTextureArray> _defaultTextureDataList;

		if (is16Bit)
			for (size_t i = 0; i < _defaultTextureDataList.size(); ++i)
			{
				_defaultTextureDataList[i] = LoadDefautTexture16Bit(initData.dataFolder, 1U << (skippedSizeLevels + i), isRBReversed);
			}
		else
			for (size_t i = 0; i < _defaultTextureDataList.size(); ++i)
			{
				_defaultTextureDataList[i] = LoadDefautTexture8Bit(initData.dataFolder, 1U << (skippedSizeLevels + i), isRBReversed);
			}

		_fenceList.reserve(initData.transferFramesInFlight);
		_transferSemaphoresList.reserve(initData.transferFramesInFlight);
		_textureBeingUsedSemaphores.resize(initData.transferFramesInFlight);

		for (size_t i = 0; i < initData.transferFramesInFlight; ++i)
		{
			_fenceList.push_back(_synchroList.AddFence(true));
			_transferSemaphoresList.push_back(_synchroList.AddSemaphore());
		}

		_synchroList.ResetFences({ _fenceList[0] });

		_preLoadedTexturesData->LoadDefaultTextures(_defaultTextureDataList, initData.transferQueueID, initData.graphicsQueueID);

		if (_streamedTexturesData)
			_streamedTexturesData->LoadDefaultTextures(_defaultTextureDataList, initData.transferQueueID, initData.graphicsQueueID);

		auto primaryCommandBuffer = textureCommandPool.GetPrimaryCommandBuffer(_primaryCommandBufferIDs[0]);

		primaryCommandBuffer.BeginRecording(VS::CommandBufferUsage::ONE_USE);

		textureCommandPool.RecordExecuteSecondaryBufferCommand(_primaryCommandBufferIDs[0], { _preLoadedCommandBufferID });
		if (_streamedTexturesData)
			textureCommandPool.RecordExecuteSecondaryBufferCommand(_primaryCommandBufferIDs[0], { _streamedCommandBufferIDs[0]});

		primaryCommandBuffer.EndRecording();

		std::vector<VS::CommandBufferSubmissionData> submissionData;

		submissionData.resize(1);
		submissionData[0].commandBufferIDs.resize(1);
		submissionData[0].commandBufferIDs[0].IRPrimaryID.type = VS::CommandBufferIDType::IR_PRIMARY;
		submissionData[0].commandBufferIDs[0].IRPrimaryID.commandPoolID = _textureCommandPoolID;
		submissionData[0].commandBufferIDs[0].IRPrimaryID.commandBufferID = _primaryCommandBufferIDs[0];

		_transferQFGroup.SubmitBuffers(initData.transferQueueID, submissionData, { _fenceList[0] });

		_synchroList.WaitOnFences({ _fenceList[0] }, true);

		{
			size_t totalDescriptorSets = initData.transferFramesInFlight * imagesInAllTextureArrays;

			_transferDescriptorPool = _descriptorDataList.AddNoIndividualFreeingDescriptorPool(static_cast<uint32_t>(totalDescriptorSets),
				{ {VS::DescriptorTypeFlagBits::COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(totalDescriptorSets)} });

			std::vector<IDObject<VS::AutoCleanupDescriptorSetLayout>> textureLayouts;
			textureLayouts.resize(initData.transferFramesInFlight, initData.textureDescriptorSetLayout);

			_textureDescriptorSets = descriptorDataList.AllocateNIFDescriptorSets(_transferDescriptorPool, textureLayouts);

			std::vector<VS::DescriptorSetCombined2DArrayTextureSamplerWriteData> textureWriteDataList;
			textureWriteDataList.resize(initData.transferFramesInFlight);

			auto preloadedTexturesImages = _preLoadedTexturesData->GetImageIDs(0);
			auto preloadedTexturesImageViews = _preLoadedTexturesData->GetImageViewIDs(0);

			for (size_t i = 0; i < textureWriteDataList.size(); ++i)
			{
				auto& textureWriteData = textureWriteDataList[i];

				textureWriteData.descriptorSetID = _textureDescriptorSets[i];
				textureWriteData.binding = 0;
				textureWriteData.startArrayIndex = 0;
				textureWriteData.imageDataList.resize(imagesInAllTextureArrays);

				for (size_t j = 0; j < imagesInTextureArray; ++j)
				{
					auto& imageData = textureWriteData.imageDataList[j];

					VS::Optional2DArrayTextureView texViewID = { {preloadedTexturesImages[j], preloadedTexturesImageViews[j]}};
					VS::Combined2DArrayTextureSamplerIDs combinedIDs = { texViewID, {} };

					imageData.first = combinedIDs;
					imageData.second = VS::ImageLayoutFlags::SHADER_READ_ONLY;
				}

				if (!(_streamedTexturesData))
				{
					for (size_t j = 0; j < imagesInTextureArray; ++j)
					{
						auto& imageData = textureWriteData.imageDataList[j + imagesInTextureArray];

						imageData.first = { {{preloadedTexturesImages[j], preloadedTexturesImageViews[j]}}, {} };
						imageData.second = VS::ImageLayoutFlags::SHADER_READ_ONLY;
					}
				}
				else
				{
					auto streamedTexturesImages = _streamedTexturesData->GetImageIDs(i);
					auto streamedTexturesImageViews = _streamedTexturesData->GetImageViewIDs(i);

					for (size_t j = 0; j < imagesInTextureArray; ++j)
					{
						auto& imageData = textureWriteData.imageDataList[j + imagesInTextureArray];

						imageData.first = { {{streamedTexturesImages[j], streamedTexturesImageViews[j]}}, {} };
						imageData.second = VS::ImageLayoutFlags::SHADER_READ_ONLY;
					}
				}
			}

			_descriptorDataList.WriteNIFDescriptorSetCombined2DArrayTextureSamplerBindings(_transferDescriptorPool, textureWriteDataList);
		}
	}

	TextureDataMainInternal::~TextureDataMainInternal()
	{
	}

	std::vector<std::shared_ptr<TextureReferenceData>> TextureDataMainInternal::GetTextureReference(bool inPreloadedTexturesList, size_t tileImageIndex, size_t referenceIndex)
	{
		if (inPreloadedTexturesList)
			return _preLoadedTexturesData->GetTextureReference(tileImageIndex, referenceIndex);
		else
			return _streamedTexturesData->GetTextureReference(tileImageIndex, referenceIndex);
	}

	IDObject<VS::AutoCleanupNIFDescriptorPool> TextureDataMainInternal::GetTexturesDescriptorSetPool() const
	{
		return _transferDescriptorPool;
	}

	IDObject<VS::AutoCleanupDescriptorSet> TextureDataMainInternal::GetTexturesDescriptorSets(size_t currentTransferFrame)
	{
		if (currentTransferFrame >= _textureDescriptorSets.size())
			throw std::runtime_error("TextureDataMainInternal::GetTexturesDescriptorSets Error: Program tried to access a non-existent texture descriptor set!");

		return _textureDescriptorSets[currentTransferFrame];
	}

	bool TextureDataMainInternal::AreStreamedTextureCreated() const
	{
		return _streamedTexturesData != nullptr;
	}

	std::vector<VS::ImagesMemoryBarrierData> TextureDataMainInternal::GetPreLoadedTransferToGraphicsMemoryBarriers(uint64_t transferQueue, uint64_t graphicsQueue)
	{
		std::vector<VS::ImagesMemoryBarrierData> ret;
		ret.reserve(imagesInTextureArray);

		_preLoadedTexturesData->GetTransferToGraphicsMemoryBarriers(ret, 0, transferQueue, graphicsQueue);

		return ret;
	}

	std::vector<VS::ImagesMemoryBarrierData> TextureDataMainInternal::GetStreamedTransferToGraphicsMemoryBarriers(size_t frameInFlightIndice, uint64_t transferQueue, uint64_t graphicsQueue)
	{
		std::vector<VS::ImagesMemoryBarrierData> ret;
		ret.reserve(imagesInTextureArray);

		if (_streamedTexturesData)
			_streamedTexturesData->GetTransferToGraphicsMemoryBarriers(ret, frameInFlightIndice, transferQueue, graphicsQueue);

		return ret;
	}

	std::vector<VS::ImagesMemoryBarrierData> TextureDataMainInternal::GetPreLoadedGraphicsToTransferMemoryBarriers(uint64_t transferQueue, uint64_t graphicsQueue)
	{
		std::vector<VS::ImagesMemoryBarrierData> ret;
		ret.reserve(imagesInTextureArray);

		_preLoadedTexturesData->GetGraphicsToTransferMemoryBarriers(ret, 0, transferQueue, graphicsQueue);

		return ret;
	}

	std::vector<VS::ImagesMemoryBarrierData> TextureDataMainInternal::GetStreamedGraphicsToTransferMemoryBarriers(size_t frameInFlightIndice, uint64_t transferQueue, uint64_t graphicsQueue)
	{
		std::vector<VS::ImagesMemoryBarrierData> ret;
		ret.reserve(imagesInTextureArray);

		if (_streamedTexturesData)
			_streamedTexturesData->GetGraphicsToTransferMemoryBarriers(ret, frameInFlightIndice, transferQueue, graphicsQueue);

		return ret;
	}

	std::optional<std::pair<size_t, size_t>> TextureDataMainInternal::TryToAddTextureToPreloadedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height)
	{
		if (data.empty())
			throw std::runtime_error("TextureDataMainInternal::TryToAddTextureToPreloadedTexturesTransferList Error: Program tried to add texture with no data!");

		if (width == 0)
			throw std::runtime_error("TextureDataMainInternal::TryToAddTextureToPreloadedTexturesTransferList Error: Program tried to add texture with width of zero!");

		if (height == 0)
			throw std::runtime_error("TextureDataMainInternal::TryToAddTextureToPreloadedTexturesTransferList Error: Program tried to add texture with height of zero!");
		
		assert(_textureFormatIs16Bit == Misc::BOOL64_TRUE || _textureFormatIs16Bit == Misc::BOOL64_FALSE);
		assert(_textureFormatIsRBReversed == Misc::BOOL64_TRUE || _textureFormatIsRBReversed == Misc::BOOL64_FALSE);

		size_t requiredSize = static_cast<size_t>(width) * height;

		if (_textureFormatIs16Bit == Misc::BOOL64_TRUE)
		{
			requiredSize *= 8;
		}
		else
		{
			requiredSize *= 4;
		}

		if (data.size() != requiredSize)
			throw std::runtime_error("TextureDataMainInternal::TryToAddTextureToPreloadedTexturesTransferList: Program was given a data vector with a wrong size!");

		return _preLoadedTexturesData->TryToAddTextureToTransferList(*data.data(), requiredSize, width, height);
	}

	std::optional<std::pair<size_t, size_t>> TextureDataMainInternal::TryToAddTextureToStreamedTexturesTransferList(const std::vector<unsigned char>& data, uint32_t width, uint32_t height)
	{
		if (_streamedTexturesData == nullptr)
			throw std::runtime_error("TextureDataMainInternal::TryToAddTextureToStreamedTexturesTransferList Error: Program tried to add texture to streamed textures despite the streaming textures being unused!");

		if (data.empty())
			throw std::runtime_error("TextureDataMainInternal::TryToAddTextureToStreamedTexturesTransferList Error: Program tried to add texture with no data!");

		if (width == 0)
			throw std::runtime_error("TextureDataMainInternal::TryToAddTextureToStreamedTexturesTransferList Error: Program tried to add texture with width of zero!");

		if (height == 0)
			throw std::runtime_error("TextureDataMainInternal::TryToAddTextureToStreamedTexturesTransferList Error: Program tried to add texture with height of zero!");

		assert(_textureFormatIs16Bit == Misc::BOOL64_TRUE || _textureFormatIs16Bit == Misc::BOOL64_FALSE);
		assert(_textureFormatIsRBReversed == Misc::BOOL64_TRUE || _textureFormatIsRBReversed == Misc::BOOL64_FALSE);

		size_t requiredSize = static_cast<size_t>(width) * height;

		if (_textureFormatIs16Bit == Misc::BOOL64_TRUE)
		{
			requiredSize *= 8;
		}
		else
		{
			requiredSize *= 4;
		}

		if (data.size() != requiredSize)
			throw std::runtime_error("TextureDataMainInternal::TryToAddTextureToStreamedTexturesTransferList: Program was given a data vector with a wrong size!");

		return _streamedTexturesData->TryToAddTextureToTransferList(*data.data(), requiredSize, width, height);
	}

	void TextureDataMainInternal::TransferPreLoadedTexturesData(uint64_t transferQueue, uint64_t graphicsQueue)
	{
		auto transferCommandPool = _transferQFGroup.GetCommandPoolWithIndividualReset(_textureCommandPoolID);
		auto transferCommandBuffer = transferCommandPool.GetPrimaryCommandBuffer(_primaryCommandBufferIDs[0]);

		if (_synchroList.WaitOnFences({ _fenceList[0] }, false, 1'000'000'000ULL) != true)
			throw std::runtime_error("TextureDataMainInternal::TransferPreLoadedTexturesData Error: First waiting on a fence timed out!");
		_synchroList.ResetFences({ _fenceList[0] });

		transferCommandBuffer.ResetCommandBuffer(false);
		transferCommandBuffer.BeginRecording(VS::CommandBufferUsage::ONE_USE);

		_preLoadedTexturesData->RecordTransferBuffer(0, transferQueue, graphicsQueue);
		transferCommandPool.RecordExecuteSecondaryBufferCommand(_primaryCommandBufferIDs[0], { _preLoadedCommandBufferID });

		transferCommandBuffer.EndRecording();

		std::vector<VS::CommandBufferSubmissionData> submissionData;

		submissionData.resize(1);
		submissionData[0].commandBufferIDs.resize(1);
		submissionData[0].commandBufferIDs[0].IRPrimaryID.type = VS::CommandBufferIDType::IR_PRIMARY;
		submissionData[0].commandBufferIDs[0].IRPrimaryID.commandPoolID = _textureCommandPoolID;
		submissionData[0].commandBufferIDs[0].IRPrimaryID.commandBufferID = _primaryCommandBufferIDs[0];
		submissionData[0].signalSemaphores.push_back(_transferSemaphoresList[0]);

		if (_textureBeingUsedSemaphores[0].has_value())
		{
			submissionData[0].waitSemaphores.emplace_back(_textureBeingUsedSemaphores[0].value(), VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER);
			_textureBeingUsedSemaphores[0].reset();
		}

		_transferQFGroup.SubmitBuffers(transferQueue, submissionData, _fenceList[0]);
		if (_synchroList.WaitOnFences({ _fenceList[0] }, false, 1'000'000'000ULL) != true)
			throw std::runtime_error("TextureDataMainInternal::TransferPreLoadedTexturesData Error: Second waiting on a fence timed out!");

		_preLoadedTexturesData->FinishTextureTransfer(0);
	}

	void TextureDataMainInternal::TransferStreamedTexturesData(size_t frameInFlightIndice, uint64_t transferQueue, uint64_t graphicsQueue)
	{
		if (frameInFlightIndice >= _transferSemaphoresList.size())
			throw std::runtime_error("TextureDataMainInternal::TransferStreamedTexturesData Error: Program tried to access a non-existent semaphore!");

		auto transferCommandPool = _transferQFGroup.GetCommandPoolWithIndividualReset(_textureCommandPoolID);
		auto transferCommandBuffer = transferCommandPool.GetPrimaryCommandBuffer(_primaryCommandBufferIDs[frameInFlightIndice]);

		if (_synchroList.WaitOnFences({ _fenceList[frameInFlightIndice] }, false, 1'000'000'000ULL) != true)
			throw std::runtime_error("TextureDataMainInternal::TransferStreamedTexturesData Error: First waiting on a fence timed out!");
		_synchroList.ResetFences({ _fenceList[frameInFlightIndice] });

		transferCommandBuffer.ResetCommandBuffer(false);
		transferCommandBuffer.BeginRecording(VS::CommandBufferUsage::ONE_USE);

		_streamedTexturesData->RecordTransferBuffer(frameInFlightIndice, transferQueue, graphicsQueue);
		transferCommandPool.RecordExecuteSecondaryBufferCommand(_primaryCommandBufferIDs[frameInFlightIndice], { _streamedCommandBufferIDs[frameInFlightIndice]});

		transferCommandBuffer.EndRecording();

		std::vector<VS::CommandBufferSubmissionData> submissionData;

		submissionData.resize(1);
		submissionData[0].commandBufferIDs.resize(1);
		submissionData[0].commandBufferIDs[0].IRPrimaryID.type = VS::CommandBufferIDType::IR_PRIMARY;
		submissionData[0].commandBufferIDs[0].IRPrimaryID.commandPoolID = _textureCommandPoolID;
		submissionData[0].commandBufferIDs[0].IRPrimaryID.commandBufferID = _primaryCommandBufferIDs[frameInFlightIndice];
		submissionData[0].signalSemaphores.push_back(_transferSemaphoresList[frameInFlightIndice]);

		if (_textureBeingUsedSemaphores[frameInFlightIndice].has_value())
		{
			submissionData[0].waitSemaphores.emplace_back(_textureBeingUsedSemaphores[frameInFlightIndice].value(), VS::PipelineStageFlagBits::PIPELINE_STAGE_TRANSFER);
			_textureBeingUsedSemaphores[frameInFlightIndice].reset();
		}

		_transferQFGroup.SubmitBuffers(transferQueue, submissionData, _fenceList[frameInFlightIndice]);

		_streamedTexturesData->FinishTextureTransfer(frameInFlightIndice);
	}

	IDObject<VS::AutoCleanupSemaphore> TextureDataMainInternal::GetTransferFinishedSemaphore(size_t frameInFlightIndice) const
	{
		if (frameInFlightIndice >= _transferSemaphoresList.size())
			throw std::runtime_error("TextureDataMainInternal::GetTransferFinishedSemaphore Error: Program tried to access a non-existent semaphore!");

		return _transferSemaphoresList[frameInFlightIndice];
	}

	void TextureDataMainInternal::SetTextureUseFinishedSemaphore(size_t frameInFlightIndice, IDObject<VS::AutoCleanupSemaphore> semaphore)
	{
		if (frameInFlightIndice >= _transferSemaphoresList.size())
			throw std::runtime_error("TextureDataMainInternal::SetTextureUseFinishedSemaphore Error: Program tried to access a non-existent semaphore slot!");

		assert(!_textureBeingUsedSemaphores[frameInFlightIndice].has_value());

		_textureBeingUsedSemaphores[frameInFlightIndice] = semaphore;
	}

	bool TextureDataMainInternal::PopTextureChangedValues(size_t frameInFlightIndice)
	{
		bool ret = _preLoadedTexturesData->PopTextureDataChangedValue(0);

		if (_streamedTexturesData != nullptr)
		{
			ret = ret || _streamedTexturesData->PopTextureDataChangedValue(frameInFlightIndice);
		}

		return ret;
	}

	bool TextureDataMainInternal::Is16Bit(TextureFormat textureFormat) const
	{
		if (textureFormat == TextureFormat::TEXTURE_FORMAT_RGBA16)
			return true;

		return false;
	}

	bool TextureDataMainInternal::IsRBReversed(TextureFormat textureFormat) const
	{
		if (textureFormat == TextureFormat::TEXTURE_FORMAT_BGRA8)
			return true;

		return false;
	}

	static const std::string defaultTexturesFolderName("DefaultTextures\\");
	static const std::string defaultTexturesName("DefaultTexture");
	static const std::string defaultTexturesExtension(".png");

	static const std::string RGBTexturesName("RGB");
	static const std::string BGRTexturesName("BGR");

	static const std::string eigthBitTexturesName("8");
	static const std::string sixteenBitTexturesName("16");

	std::vector<unsigned char> TextureDataMainInternal::LoadDefautTexture8Bit(std::string dataFolder, uint32_t tileSize, bool RBreversed) const
	{
		std::vector<unsigned char> ret;
		
		std::string filepath = dataFolder + defaultTexturesFolderName + defaultTexturesName;
		filepath += std::to_string(tileSize);

		if (RBreversed)
		{
			filepath += BGRTexturesName;
		}
		else
		{
			filepath += RGBTexturesName;
		}

		filepath += eigthBitTexturesName;
		filepath += defaultTexturesExtension;

		int width = 0;
		int height = 0;
		int channels = 0;

		stbi_uc* pixels = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (pixels == nullptr)
			throw std::runtime_error("TextureDataMainInternal::LoadDefautTexture8Bit Error: Program failed to read a default texture!");

		if (static_cast<uint32_t>(width) != tileSize || static_cast<uint32_t>(height) != tileSize)
			throw std::runtime_error("TextureDataMainInternal::LoadDefautTexture8Bit Error: Program read an incorrect default texture!");

		size_t imageSize = static_cast<size_t>(tileSize) * tileSize * 4;

		ret.resize(imageSize);
		std::memcpy(ret.data(), pixels, imageSize);

		return ret;
	}

	std::vector<unsigned char> TextureDataMainInternal::LoadDefautTexture16Bit(std::string dataFolder, uint32_t tileSize, bool RBreversed) const
	{
		std::vector<unsigned char> ret;

		std::string filepath = dataFolder + defaultTexturesFolderName + defaultTexturesName;
		filepath += std::to_string(tileSize);

		if (RBreversed)
		{
			filepath += BGRTexturesName;
		}
		else
		{
			filepath += RGBTexturesName;
		}

		filepath += sixteenBitTexturesName;
		filepath += defaultTexturesExtension;

		int width = 0;
		int height = 0;
		int channels = 0;

		stbi_us* pixels = stbi_load_16(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (pixels == nullptr)
			throw std::runtime_error("TextureDataMainInternal::LoadDefautTexture8Bit Error: Program failed to read a default texture!");

		if (static_cast<uint32_t>(width) != tileSize || static_cast<uint32_t>(height) != tileSize)
			throw std::runtime_error("TextureDataMainInternal::LoadDefautTexture8Bit Error: Program read an incorrect default texture!");

		size_t imageSize = static_cast<size_t>(tileSize) * tileSize * 8;

		ret.resize(imageSize);
		std::memcpy(ret.data(), pixels, imageSize);

		return ret;
	}

}