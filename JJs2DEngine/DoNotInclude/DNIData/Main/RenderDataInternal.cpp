#include "MainDNIpch.h"
#include "RenderDataInternal.h"

#include "PipelineCacheHeaders.h"
#include "DeviceSettingsInternal.h"

#include "../../../Include/Common/TextureArraySize.h"

#include <fstream>
#include <filesystem>
#include <array>

#include <CPPCRC/cppcrc.h>

#include <Miscellaneous/Bool64.h>

#include <VulkanSimplified/VSDevice/VSDescriptorDataLists.h>
#include <VulkanSimplified/VSDevice/VSShaderLists.h>
#include <VulkanSimplified/VSDevice/VSPipelineDataLists.h>
#include <VulkanSimplified/VSDevice/VSImageDataLists.h>
#include <VulkanSimplified/VSDevice/VSPipelineLayoutCreationData.h>
#include <VulkanSimplified/VSDevice/VSRenderPassDataList.h>
#include <VulkanSimplified/VSDevice/VSSubpassCreationDataWithoutResolving.h>

#include <VulkanSimplified/VSSharedData/VSSharedDescriptorDataList.h>
#include <VulkanSimplified/VSSharedData/VSSharedRenderPassDataList.h>
#include <VulkanSimplified/VSSharedData/VSSharedPipelineDataLists.h>
#include <VulkanSimplified/VSSharedData/VSVertexBindingInputRate.h>
#include <VulkanSimplified/VSSharedData/VSPipelinePrimitiveTopology.h>
#include <VulkanSimplified/VSSharedData/VSPipelinePolygonMode.h>
#include <VulkanSimplified/VSSharedData/VSDepthUsage.h>
#include <VulkanSimplified/VSSharedData/VSColorBlendingComponentFlags.h>
#include <VulkanSimplified/VSSharedData/VSColorBlendingPreset.h>

#include <VulkanSimplified/VSCommon/VSDescriptorTypeFlags.h>
#include <VulkanSimplified/VSCommon/VSShaderTypeFlags.h>
#include <VulkanSimplified/VSCommon/VSRenderPassAttachmentOP.h>
#include <VulkanSimplified/VSCommon/VSImageLayoutFlags.h>
#include <VulkanSimplified/VSCommon/VSImageSampleFlags.h>
#include <VulkanSimplified/VSCommon/VSPipelineStageFlags.h>
#include <VulkanSimplified/VSCommon/VSAccessFlags.h>
#include <VulkanSimplified/VSCommon/VSCompareOperationsType.h>

namespace JJs2DEngine
{
	const std::string pipelineCacheDirectoryName("PipelineCache\\");

	const std::string UIPipelineCacheName("UIPipelineCache");
	const std::string GammaCorrectionPipelineCacheName("GammaCorrectionPipelineCache");

#if defined(_DEBUG) || defined(DEBUG)
	const std::string cacheFiletype(".debug.cache");
#else
	const std::string cacheFiletype(".cache");
#endif
	
	const std::string UIPipelineFullName(pipelineCacheDirectoryName + UIPipelineCacheName + cacheFiletype);
	const std::string GammaCorrectionPipelineFullName(pipelineCacheDirectoryName + GammaCorrectionPipelineCacheName + cacheFiletype);

	const std::string shaderDirectoryName("Shaders\\");

	const std::string uiLayerVertexShaderName("UILayerShader");

	const std::string standardFragmentShaderName("StandardFragmentShader");

	const std::string gammaCorrectionFragmentShaderName("GammaCorrectionFragmentShader");
	const std::string gammaCorrectionVertexShaderName("GammaCorrectionVertexShader");

#if defined(_DEBUG) || defined(DEBUG)
	const std::string vertexShaderExtension(".debug.vert.spv");
	const std::string fragmentShaderExtension(".debug.frag.spv");
#else
	const std::string vertexShaderExtension(".vert.spv");
	const std::string fragmentShaderExtension(".frag.spv");
#endif

	namespace fs = std::filesystem;

	RenderDataInternal::RenderDataInternal(size_t currentPipelineSettings, const std::vector<PipelineSettings>& preInitializedPipelineSettings, const std::string& dataFolder,
		VS::DeviceMain device, VS::SharedDataMainList sharedData) : _device(device), _sharedData(sharedData), _currentPipelineSettings(currentPipelineSettings)
	{
		bool uiCacheCreateMainHeader = false;

		if (dataFolder != "" && !fs::exists(dataFolder))
			throw std::runtime_error("RenderDataInternal Constructor Error: Program was given an non-existent data folder!");

		if (!fs::exists(dataFolder + pipelineCacheDirectoryName))
		{
			fs::create_directory(dataFolder + pipelineCacheDirectoryName);

			uiCacheCreateMainHeader = true;
		}

		if (fs::exists(dataFolder + UIPipelineFullName))
		{
			LoadUIPipelineCacheFile(dataFolder);
		}
		else
		{
			CreateUIPipelineCacheFile(dataFolder);
		}

		if (fs::exists(dataFolder + GammaCorrectionPipelineFullName))
		{
			LoadGammaCorrectionPipelineCacheFile(dataFolder);
		}
		else
		{
			CreateGammaCorrectionPipelineCacheFile(dataFolder);
		}

		auto shaderList = _device.GetShaderLists();
		auto standardFragmentShaderData = LoadShaderFile(dataFolder + standardFragmentShaderName + fragmentShaderExtension);
		_standardFragmentShaderID = shaderList.CreateFragmentShaderModule(*standardFragmentShaderData.data(), standardFragmentShaderData.size(), 0x10);

		auto imageList = _device.GetImageDataLists();
		auto sampler = imageList.AddSampler(false, false, false, false, false, false, 0.0f, 0.0f, 0.0f, 0.0f);

		auto sharedDescriptorData = _sharedData.GetSharedDescriptorDataList();
		auto textureDescriptorBinding = sharedDescriptorData.AddUniqueDescriptorSetLayoutBindingsData(VS::DescriptorTypeFlagBits::COMBINED_IMAGE_SAMPLER, imagesInAllTextureArrays,
			VS::ShaderTypeFlagBit::SHADER_TYPE_FRAGMENT, 0x10);

		auto deviceDescriptorList = _device.GetDescriptorDataLists();
		auto devicePipelineList = _device.GetPipelineDataLists();

		{
			_renderPassList.reserve(preInitializedPipelineSettings.size());

			for (size_t i = 0; i < preInitializedPipelineSettings.size(); ++i)
			{
				_renderPassList.push_back(CreateRenderPass(preInitializedPipelineSettings[i]));
			}
		}

		std::vector<VS::GraphicsPipelineCreationData> creationDataList;
		creationDataList.reserve(preInitializedPipelineSettings.size());

		{
			auto uiLayerVertexShaderData = LoadShaderFile(dataFolder + uiLayerVertexShaderName + vertexShaderExtension);
			_uiVertexShaderID = shaderList.CreateVertexShaderModule(*uiLayerVertexShaderData.data(), uiLayerVertexShaderData.size(), 0x10);

			_textureDescriptorSetLayout = deviceDescriptorList.AddDescriptorSetLayout(0, { {textureDescriptorBinding,
				std::vector<decltype(sampler)>(imagesInAllTextureArrays, sampler)} }, 0x10);

			VS::PipelineLayoutCreationData uiPipelineLayoutCreationData;
			uiPipelineLayoutCreationData._descriptorSets = { _textureDescriptorSetLayout };

			_uiPipelineLayout = devicePipelineList.AddPipelineLayout(uiPipelineLayoutCreationData, 0x10);

			for (size_t i = 0; i < preInitializedPipelineSettings.size(); ++i)
			{
				const auto& pipelineSettings = preInitializedPipelineSettings[i];

				creationDataList.push_back(CompileUIPipelinesCreationData(pipelineSettings.renderWidth, GetHeight(pipelineSettings.renderWidth, pipelineSettings.aspectRatio),
					_renderPassList[i]));
			}

			_uiPipelineList = devicePipelineList.AddGraphicPipelines(creationDataList, _uiPipelineCache, creationDataList.size() * 8);
			SaveUIPipelineCacheFile(dataFolder);
		}

		{
			auto gammaCorrectionFragmentShaderData = LoadShaderFile(dataFolder + gammaCorrectionFragmentShaderName + fragmentShaderExtension);
			_gammaCorrectionFragmentShaderID = shaderList.CreateFragmentShaderModule(*gammaCorrectionFragmentShaderData.data(), gammaCorrectionFragmentShaderData.size(), 0x10);

			auto gammaCorrectionVertexShaderData = LoadShaderFile(dataFolder + gammaCorrectionVertexShaderName + vertexShaderExtension);
			_gammaCorrectionVertexShaderID = shaderList.CreateVertexShaderModule(*gammaCorrectionVertexShaderData.data(), gammaCorrectionVertexShaderData.size(), 0x10);

			auto gammaCorrectionDescriptorBinding = sharedDescriptorData.AddUniqueDescriptorSetLayoutBindingsData(VS::DescriptorTypeFlagBits::INPUT_ATTACHMENT, 1,
				VS::ShaderTypeFlagBit::SHADER_TYPE_FRAGMENT, 0x10);

			_gammaCorrectionDescriptorSetLayout = deviceDescriptorList.AddDescriptorSetLayout(0, { {gammaCorrectionDescriptorBinding, {}} }, 0x10);

			VS::PipelineLayoutCreationData gammaCorrectionPipelineLayoutCreationData;
			gammaCorrectionPipelineLayoutCreationData._descriptorSets = { _gammaCorrectionDescriptorSetLayout };
			_gammaCorrectionPipelineLayout = devicePipelineList.AddPipelineLayout(gammaCorrectionPipelineLayoutCreationData, 0x10);
			creationDataList.clear();

			for (size_t i = 0; i < preInitializedPipelineSettings.size(); ++i)
			{
				const auto& pipelineSettings = preInitializedPipelineSettings[i];

				creationDataList.push_back(CompileGammaCorrectionPipelinesCreationData(pipelineSettings.renderWidth,
					GetHeight(pipelineSettings.renderWidth, pipelineSettings.aspectRatio), _renderPassList[i]));
			}

			_gammaCorrectionPipelineList = devicePipelineList.AddGraphicPipelines(creationDataList, _gammaCorrectionPipelineCache, creationDataList.size() * 8);
			SaveGammaCorrectionPipelineCacheFile(dataFolder);
		}
	}

	RenderDataInternal::~RenderDataInternal()
	{
	}

	IDObject<VS::AutoCleanupRenderPass> RenderDataInternal::GetCurrentRenderPass() const
	{
		return _renderPassList[_currentPipelineSettings];
	}

	IDObject<VS::AutoCleanupDescriptorSetLayout> RenderDataInternal::GetTextureDescriptorSetLayout() const
	{
		return _textureDescriptorSetLayout;
	}

	void RenderDataInternal::CreateUIPipelineCacheFile(const std::string& dataFolder)
	{
		auto pipelineDataList = _device.GetPipelineDataLists();

		std::fstream uiPipelineCacheFile;

		uiPipelineCacheFile.open(dataFolder + UIPipelineFullName, std::ios_base::binary | std::ios_base::in | std::ios_base::out | std::ios_base::trunc);

		if (!uiPipelineCacheFile.is_open())
			throw std::runtime_error("RenderDataInternal::CreateUIPipelineCacheFile Error: Program failed to open the pipeline cache file!");

		PipelineCacheMainHeader uiMainHeader;

		uiPipelineCacheFile.write(reinterpret_cast<char*>(&uiMainHeader), sizeof(uiMainHeader));
		if (!uiPipelineCacheFile.good())
			throw std::runtime_error("RenderDataInternal::CreateUIPipelineCacheFile Error: Program failed to save the header to the newly created pipeline cache file!");

		auto uiPipelineCacheID = pipelineDataList.AddPipelineCache({}, 0x10);

		if (uiPipelineCacheID.has_value())
		{
			_uiPipelineCache = uiPipelineCacheID.value();
		} else
			throw std::runtime_error("RenderDataInternal::CreateUIPipelineCacheFile Error: Program failed to create the pipeline cache!");

		uiPipelineCacheFile.close();
	}

	void RenderDataInternal::LoadUIPipelineCacheFile(const std::string& dataFolder)
	{
		auto pipelineDataList = _device.GetPipelineDataLists();

		std::ifstream uiPipelineCacheFile;

		uiPipelineCacheFile.open(dataFolder + UIPipelineFullName, std::ios_base::binary | std::ios_base::in | std::ios_base::out);

		if (!uiPipelineCacheFile.is_open())
			throw std::runtime_error("RenderDataInternal::LoadUIPipelineCacheFile Error: Program failed to open the pipeline cache file!");

		PipelineCacheMainHeader uiMainHeader;

		uiPipelineCacheFile.read(reinterpret_cast<char*>(&uiMainHeader), sizeof(uiMainHeader));
		if (!uiPipelineCacheFile.good())
			throw std::runtime_error("RenderDataInternal::LoadUIPipelineCacheFile Error: Program failed to read the header from the pipeline cache file!");
		else if (uiMainHeader.magicNumbers != pipelineHeaderCorrectMagicNumbers)
			throw std::runtime_error("RenderDataInternal::LoadUIPipelineCacheFile Error: pipeline cache file is corrupted!");

		bool foundCompatibleData = false;
		std::vector<unsigned char> uiPipelineCompatibleData;

		for (uint64_t i = 0; i < uiMainHeader.elementCount; ++i)
		{
			uint64_t currentPos = uiPipelineCacheFile.tellg();

			PipelineCacheElementHeader elementHeader;
			uiPipelineCacheFile.read(reinterpret_cast<char*>(&elementHeader), sizeof(elementHeader));
			if (!uiPipelineCacheFile.good())
				throw std::runtime_error("RenderDataInternal::LoadUIPipelineCacheFile Error: Program failed to read a pipeline cache's element header!");

			if (elementHeader.deleted != Misc::BOOL64_FALSE)
			{
				uiPipelineCacheFile.seekg(elementHeader.elementSize, std::ios_base::cur);
				continue;
			}

			uiPipelineCompatibleData.resize(elementHeader.elementSize);
			uiPipelineCacheFile.read(reinterpret_cast<char*>(uiPipelineCompatibleData.data()), elementHeader.elementSize);
			if (!uiPipelineCacheFile.good())
				throw std::runtime_error("RenderDataInternal::LoadUIPipelineCacheFile Error: Program failed to read a pipeline cache's element data!");

			uint64_t currentDataCRC64WE = CRC64::WE::calc(uiPipelineCompatibleData.data(), uiPipelineCompatibleData.size());

			if (currentDataCRC64WE != elementHeader.elementCRC64WE)
				throw std::runtime_error("RenderDataInternal::LoadUIPipelineCacheFile Error: At least one of the elements of pipeline cache file is corrupted!");

			auto uiPipelineCacheID = pipelineDataList.AddPipelineCache(uiPipelineCompatibleData, 0x10);

			if (uiPipelineCacheID.has_value())
			{
				_uiPipelineCache = uiPipelineCacheID.value();
				foundCompatibleData = true;

				_uiPipelineCompatibleSavedPos = currentPos;
				_uiPipelineCompatibleSavedSize = elementHeader.elementSize;
				_uiPipelineCompatibleSavedCRC64WE = elementHeader.elementCRC64WE;

				break;
			}
		}

		if (!foundCompatibleData)
		{
			auto uiPipelineCacheID = pipelineDataList.AddPipelineCache({}, 0x10);

			if (uiPipelineCacheID.has_value())
			{
				_uiPipelineCache = uiPipelineCacheID.value();
			}
			else
				throw std::runtime_error("RenderDataInternal::LoadUIPipelineCacheFile Error: Program failed to create the pipeline cache!");
		}

		uiPipelineCacheFile.close();
	}

	void RenderDataInternal::SaveUIPipelineCacheFile(const std::string& dataFolder)
	{
		auto pipelineDataList = _device.GetPipelineDataLists();
		std::ifstream uiPipelineCacheInFile;
		std::ofstream uiPipelineCacheOutFile;

		auto dataToSave = pipelineDataList.GetPipelineCacheData(_uiPipelineCache);
		if (dataToSave.empty())
			return;

		bool saveData = false;

		uint64_t currentDataCRC64WE = CRC64::WE::calc(dataToSave.data(), dataToSave.size());

		if (!_uiPipelineCompatibleSavedSize.has_value() || dataToSave.size() != _uiPipelineCompatibleSavedSize.value())
		{
			saveData = true;
		}
		else
		{
			saveData = currentDataCRC64WE != _uiPipelineCompatibleSavedCRC64WE.value();
		}

		if (!saveData)
			return;

		PipelineCacheElementHeader elementHeader, oldElementHeader;
		uiPipelineCacheInFile.open(dataFolder + UIPipelineFullName, std::ios_base::binary | std::ios_base::in);

		if (!uiPipelineCacheInFile.is_open())
			throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to open the pipeline cache in file!");

		if (_uiPipelineCompatibleSavedPos.has_value())
		{
			uiPipelineCacheInFile.seekg(_uiPipelineCompatibleSavedPos.value());
			if (!uiPipelineCacheInFile.good())
				throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to move reading position to the old header!");

			uiPipelineCacheInFile.read(reinterpret_cast<char*>(&oldElementHeader), sizeof(oldElementHeader));
			if (!uiPipelineCacheInFile.good())
				throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to read the old header!");
		}

		PipelineCacheMainHeader uiMainHeader;

		uiPipelineCacheInFile.seekg(uiMainHeader.magicNumbers.size());
		if (!uiPipelineCacheInFile.good())
			throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to move reading position to the main header's element count!");

		uiPipelineCacheInFile.read(reinterpret_cast<char*>(&uiMainHeader.elementCount), sizeof(uiMainHeader.elementCount));
		if (!uiPipelineCacheInFile.good())
			throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to read the old main header!");

		if (!uiPipelineCacheInFile.good())
			throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to flush all writes!");

		uiPipelineCacheInFile.close();

		uiPipelineCacheOutFile.open(dataFolder + UIPipelineFullName, std::ios_base::binary | std::ios_base::out | std::ios_base::in);

		if (!uiPipelineCacheOutFile.is_open())
			throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to open the pipeline cache out file!");

		if (_uiPipelineCompatibleSavedPos.has_value())
		{
			uiPipelineCacheOutFile.seekp(_uiPipelineCompatibleSavedPos.value());
			if (!uiPipelineCacheOutFile.good())
				throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to move writing position to the old header!");

			oldElementHeader.deleted = Misc::BOOL64_TRUE;

			uiPipelineCacheOutFile.write(reinterpret_cast<const char*>(&oldElementHeader), sizeof(oldElementHeader));
			if (!uiPipelineCacheOutFile.good())
				throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to write to the old header!");
		}

		uiPipelineCacheOutFile.seekp(0, std::ios_base::end);
		if (!uiPipelineCacheOutFile.good())
			throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to move writing position to the file's end!");

		elementHeader.elementSize = dataToSave.size();
		elementHeader.elementCRC64WE = currentDataCRC64WE;
		elementHeader.deleted = Misc::BOOL64_FALSE;

		uiPipelineCacheOutFile.write(reinterpret_cast<const char*>(&elementHeader), sizeof(elementHeader));
		if (!uiPipelineCacheOutFile.good())
			throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to write the new header!");

		uiPipelineCacheOutFile.write(reinterpret_cast<const char*>(dataToSave.data()), dataToSave.size());
		if (!uiPipelineCacheOutFile.good())
			throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to write the new data!");

		uiPipelineCacheOutFile.seekp(uiMainHeader.magicNumbers.size());
		if (!uiPipelineCacheOutFile.good())
			throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to move writing position to the main header's element count!");

		uiMainHeader.elementCount += 1;
		uiPipelineCacheOutFile.write(reinterpret_cast<const char*>(&uiMainHeader.elementCount), sizeof(uiMainHeader.elementCount));
		if (!uiPipelineCacheOutFile.good())
			throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to write the updated main header!");

		uiPipelineCacheOutFile.flush();
		if (!uiPipelineCacheOutFile.good())
			throw std::runtime_error("RenderDataInternal::SaveUIPipelineCacheFile: Program failed to flush the writes!");

		uiPipelineCacheOutFile.close();
	}

	void RenderDataInternal::CreateGammaCorrectionPipelineCacheFile(const std::string& dataFolder)
	{
		auto pipelineDataList = _device.GetPipelineDataLists();

		std::fstream gammaCorrectionPipelineCacheFile;

		gammaCorrectionPipelineCacheFile.open(dataFolder + GammaCorrectionPipelineFullName, std::ios_base::binary | std::ios_base::in | std::ios_base::out | std::ios_base::trunc);

		if (!gammaCorrectionPipelineCacheFile.is_open())
			throw std::runtime_error("RenderDataInternal::CreateGammaCorrectionPipelineCacheFile Error: Program failed to open the pipeline cache file!");

		PipelineCacheMainHeader uiMainHeader;

		gammaCorrectionPipelineCacheFile.write(reinterpret_cast<char*>(&uiMainHeader), sizeof(uiMainHeader));
		if (!gammaCorrectionPipelineCacheFile.good())
			throw std::runtime_error("RenderDataInternal::CreateGammaCorrectionPipelineCacheFile Error: Program failed to save the header to the newly created pipeline cache file!");

		auto gammaCorrectionCacheID = pipelineDataList.AddPipelineCache({}, 0x10);

		if (gammaCorrectionCacheID.has_value())
		{
			_gammaCorrectionPipelineCache = gammaCorrectionCacheID.value();
		}
		else
			throw std::runtime_error("RenderDataInternal::CreateGammaCorrectionPipelineCacheFile Error: Program failed to create the pipeline cache!");

		gammaCorrectionPipelineCacheFile.close();
	}

	void RenderDataInternal::LoadGammaCorrectionPipelineCacheFile(const std::string& dataFolder)
	{
		auto pipelineDataList = _device.GetPipelineDataLists();

		std::ifstream gammaCorrectionCacheFile;

		gammaCorrectionCacheFile.open(dataFolder + GammaCorrectionPipelineFullName, std::ios_base::binary | std::ios_base::in | std::ios_base::out);

		if (!gammaCorrectionCacheFile.is_open())
			throw std::runtime_error("RenderDataInternal::LoadGammaCorrectionPipelineCacheFile Error: Program failed to open the pipeline cache file!");

		PipelineCacheMainHeader uiMainHeader;

		gammaCorrectionCacheFile.read(reinterpret_cast<char*>(&uiMainHeader), sizeof(uiMainHeader));
		if (!gammaCorrectionCacheFile.good())
			throw std::runtime_error("RenderDataInternal::LoadGammaCorrectionPipelineCacheFile Error: Program failed to read the header from the pipeline cache file!");
		else if (uiMainHeader.magicNumbers != pipelineHeaderCorrectMagicNumbers)
			throw std::runtime_error("RenderDataInternal::LoadGammaCorrectionPipelineCacheFile Error: pipeline cache file is corrupted!");

		bool foundCompatibleData = false;
		std::vector<unsigned char> uiPipelineCompatibleData;

		for (uint64_t i = 0; i < uiMainHeader.elementCount; ++i)
		{
			uint64_t currentPos = gammaCorrectionCacheFile.tellg();

			PipelineCacheElementHeader elementHeader;
			gammaCorrectionCacheFile.read(reinterpret_cast<char*>(&elementHeader), sizeof(elementHeader));
			if (!gammaCorrectionCacheFile.good())
				throw std::runtime_error("RenderDataInternal::LoadGammaCorrectionPipelineCacheFile Error: Program failed to read a pipeline cache's element header!");

			if (elementHeader.deleted != Misc::BOOL64_FALSE)
			{
				gammaCorrectionCacheFile.seekg(elementHeader.elementSize, std::ios_base::cur);
				continue;
			}

			uiPipelineCompatibleData.resize(elementHeader.elementSize);
			gammaCorrectionCacheFile.read(reinterpret_cast<char*>(uiPipelineCompatibleData.data()), elementHeader.elementSize);
			if (!gammaCorrectionCacheFile.good())
				throw std::runtime_error("RenderDataInternal::LoadGammaCorrectionPipelineCacheFile Error: Program failed to read a pipeline cache's element data!");

			uint64_t currentDataCRC64WE = CRC64::WE::calc(uiPipelineCompatibleData.data(), uiPipelineCompatibleData.size());

			if (currentDataCRC64WE != elementHeader.elementCRC64WE)
				throw std::runtime_error("RenderDataInternal::LoadGammaCorrectionPipelineCacheFile Error: At least one of the elements of pipeline cache file is corrupted!");

			auto gammaCorrectionCacheID = pipelineDataList.AddPipelineCache(uiPipelineCompatibleData, 0x10);

			if (gammaCorrectionCacheID.has_value())
			{
				_gammaCorrectionPipelineCache = gammaCorrectionCacheID.value();
				foundCompatibleData = true;

				_gammaCorrectionPipelineCompatibleSavedPos = currentPos;
				_gammaCorrectionPipelineCompatibleSavedSize = elementHeader.elementSize;
				_gammaCorrectionPipelineCompatibleSavedCRC64WE = elementHeader.elementCRC64WE;

				break;
			}
		}

		if (!foundCompatibleData)
		{
			auto gammaCorrectionCacheID = pipelineDataList.AddPipelineCache({}, 0x10);

			if (gammaCorrectionCacheID.has_value())
			{
				_gammaCorrectionPipelineCache = gammaCorrectionCacheID.value();
			}
			else
				throw std::runtime_error("RenderDataInternal::LoadGammaCorrectionPipelineCacheFile Error: Program failed to create the pipeline cache!");
		}

		gammaCorrectionCacheFile.close();
	}

	void RenderDataInternal::SaveGammaCorrectionPipelineCacheFile(const std::string& dataFolder)
	{
		auto pipelineDataList = _device.GetPipelineDataLists();
		std::ifstream gammaCorrectionPipelineCacheInFile;
		std::ofstream gammaCorrectionPipelineCacheOutFile;

		auto dataToSave = pipelineDataList.GetPipelineCacheData(_gammaCorrectionPipelineCache);
		if (dataToSave.empty())
			return;

		bool saveData = false;

		uint64_t currentDataCRC64WE = CRC64::WE::calc(dataToSave.data(), dataToSave.size());

		if (!_gammaCorrectionPipelineCompatibleSavedSize.has_value() || dataToSave.size() != _gammaCorrectionPipelineCompatibleSavedSize.value())
		{
			saveData = true;
		}
		else
		{
			saveData = currentDataCRC64WE != _gammaCorrectionPipelineCompatibleSavedCRC64WE.value();
		}

		if (!saveData)
			return;

		PipelineCacheElementHeader elementHeader, oldElementHeader;
		gammaCorrectionPipelineCacheInFile.open(dataFolder + GammaCorrectionPipelineFullName, std::ios_base::binary | std::ios_base::in);

		if (!gammaCorrectionPipelineCacheInFile.is_open())
			throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to open the pipeline cache in file!");

		if (_gammaCorrectionPipelineCompatibleSavedPos.has_value())
		{
			gammaCorrectionPipelineCacheInFile.seekg(_gammaCorrectionPipelineCompatibleSavedPos.value());
			if (!gammaCorrectionPipelineCacheInFile.good())
				throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to move reading position to the old header!");

			gammaCorrectionPipelineCacheInFile.read(reinterpret_cast<char*>(&oldElementHeader), sizeof(oldElementHeader));
			if (!gammaCorrectionPipelineCacheInFile.good())
				throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to read the old header!");
		}

		PipelineCacheMainHeader gammaCorrectionMainHeader;

		gammaCorrectionPipelineCacheInFile.seekg(gammaCorrectionMainHeader.magicNumbers.size());
		if (!gammaCorrectionPipelineCacheInFile.good())
			throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to move reading position to the main header's element count!");

		gammaCorrectionPipelineCacheInFile.read(reinterpret_cast<char*>(&gammaCorrectionMainHeader.elementCount), sizeof(gammaCorrectionMainHeader.elementCount));
		if (!gammaCorrectionPipelineCacheInFile.good())
			throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to read the old main header!");

		if (!gammaCorrectionPipelineCacheInFile.good())
			throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to flush all writes!");

		gammaCorrectionPipelineCacheInFile.close();

		gammaCorrectionPipelineCacheOutFile.open(dataFolder + GammaCorrectionPipelineFullName, std::ios_base::binary | std::ios_base::out | std::ios_base::in);

		if (!gammaCorrectionPipelineCacheOutFile.is_open())
			throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to open the pipeline cache out file!");

		if (_gammaCorrectionPipelineCompatibleSavedPos.has_value())
		{
			gammaCorrectionPipelineCacheOutFile.seekp(_gammaCorrectionPipelineCompatibleSavedPos.value());
			if (!gammaCorrectionPipelineCacheOutFile.good())
				throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to move writing position to the old header!");

			oldElementHeader.deleted = Misc::BOOL64_TRUE;

			gammaCorrectionPipelineCacheOutFile.write(reinterpret_cast<const char*>(&oldElementHeader), sizeof(oldElementHeader));
			if (!gammaCorrectionPipelineCacheOutFile.good())
				throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to write to the old header!");
		}

		gammaCorrectionPipelineCacheOutFile.seekp(0, std::ios_base::end);
		if (!gammaCorrectionPipelineCacheOutFile.good())
			throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to move writing position to the file's end!");

		elementHeader.elementSize = dataToSave.size();
		elementHeader.elementCRC64WE = currentDataCRC64WE;
		elementHeader.deleted = Misc::BOOL64_FALSE;

		gammaCorrectionPipelineCacheOutFile.write(reinterpret_cast<const char*>(&elementHeader), sizeof(elementHeader));
		if (!gammaCorrectionPipelineCacheOutFile.good())
			throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to write the new header!");

		gammaCorrectionPipelineCacheOutFile.write(reinterpret_cast<const char*>(dataToSave.data()), dataToSave.size());
		if (!gammaCorrectionPipelineCacheOutFile.good())
			throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to write the new data!");

		gammaCorrectionPipelineCacheOutFile.seekp(gammaCorrectionMainHeader.magicNumbers.size());
		if (!gammaCorrectionPipelineCacheOutFile.good())
			throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to move writing position to the main header's element count!");

		gammaCorrectionMainHeader.elementCount += 1;
		gammaCorrectionPipelineCacheOutFile.write(reinterpret_cast<const char*>(&gammaCorrectionMainHeader.elementCount), sizeof(gammaCorrectionMainHeader.elementCount));
		if (!gammaCorrectionPipelineCacheOutFile.good())
			throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to write the updated main header!");

		gammaCorrectionPipelineCacheOutFile.flush();
		if (!gammaCorrectionPipelineCacheOutFile.good())
			throw std::runtime_error("RenderDataInternal::SaveGammaCorrectionPipelineCacheFile: Program failed to flush the writes!");

		gammaCorrectionPipelineCacheOutFile.close();
	}

	std::vector<char> RenderDataInternal::LoadShaderFile(std::string shaderName)
	{
		std::vector<char> ret;

		std::ifstream file;

		std::string fullFilename(shaderDirectoryName + shaderName);

		if (!fs::exists(fullFilename))
			throw std::runtime_error("RenderDataInternal::LoadShaderFile Error: Shader module's file does not exist!");

		file.open(fullFilename, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);
		if (!file.is_open())
			throw std::runtime_error("RenderDataInternal::LoadShaderFile Error: Program failed to open the shader module's file!");

		uint64_t filesize = file.tellg();
		file.seekg(0, std::ios_base::beg);
		if (!file.good())
			throw std::runtime_error("RenderDataInternal::LoadShaderFile Error: Program failed to move reading position to the beggining of the file!");

		ret.resize(filesize);
		file.read(ret.data(), filesize);
		if (!file.good())
			throw std::runtime_error("RenderDataInternal::LoadShaderFile Error: Program failed to read shader module file's data!");

		file.close();

		return ret;
	}

	IDObject<VS::AutoCleanupRenderPass> RenderDataInternal::CreateRenderPass(const PipelineSettings& pipelineSettings)
	{
		IDObject<VS::AutoCleanupRenderPass> ret;

		auto sharedRenderPassData = _sharedData.GetSharedRenderPassDataList();

		auto colorAttachment = sharedRenderPassData.AddUniqueRenderPassAttachment(TranslateToFormat(pipelineSettings.swapchainFormat), VS::SAMPLE_1,
			VS::RenderPassAttachmentLoadOP::CLEAR, VS::RenderPassAttachmentStoreOP::STORE, VS::ImageLayoutFlags::UNDEFINED, VS::ImageLayoutFlags::TRANSFER_SOURCE, 0x10);

		auto depthAttachment = sharedRenderPassData.AddUniqueRenderPassAttachment(TranslateToFormat(pipelineSettings.depthFormat), VS::SAMPLE_1,
			VS::RenderPassAttachmentLoadOP::CLEAR, VS::RenderPassAttachmentStoreOP::IGNORE, VS::ImageLayoutFlags::UNDEFINED, VS::ImageLayoutFlags::DEPTH_STENCIL_READ_WRITE, 0x10);

		std::vector<IDObject<VS::RenderPassAttachmentData>> renderPassAttachments;
		renderPassAttachments.reserve(2);
		renderPassAttachments.push_back(colorAttachment);
		renderPassAttachments.push_back(depthAttachment);

		auto colorAttachmentReference = sharedRenderPassData.AddUniqueRenderPassAttachmentReference(0, VS::ImageLayoutFlags::COLOR_ATTACHMENT, 0x10);
		auto depthAttachmentReference = sharedRenderPassData.AddUniqueRenderPassAttachmentReference(1, VS::ImageLayoutFlags::DEPTH_STENCIL_READ_WRITE, 0x10);
		auto gammaCorrectionAttachmentReference = sharedRenderPassData.AddUniqueRenderPassAttachmentReference(0, VS::ImageLayoutFlags::GENERAL, 0x10);

		auto colorSubpassDependency = sharedRenderPassData.AddUniqueSubpassDependency(VS::externalSubpass, 0, VS::PipelineStageFlagBits::PIPELINE_STAGE_TOP_OF_PIPE,
			VS::PipelineStageFlagBits::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, 0, VS::AccessFlagBits::ACCESS_COLOR_ATTACHMENT_WRITE, 0x10);

		auto gammaCorrectionSubpassDependency = sharedRenderPassData.AddUniqueSubpassDependency(0, 1, VS::PipelineStageFlagBits::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT,
			VS::PipelineStageFlagBits::PIPELINE_STAGE_FRAGMENT_SHADER, VS::ACCESS_COLOR_ATTACHMENT_WRITE, VS::ACCESS_COLOR_ATTACHMENT_READ, 0x10);

		VS::SubpassCreationDataWithoutResolving colorSubpass;
		colorSubpass.colorAttachments = { colorAttachmentReference };
		colorSubpass.depthStencilAttachment = depthAttachmentReference;

		VS::SubpassCreationDataWithoutResolving gammaCorrectionSubpass;
		gammaCorrectionSubpass.colorAttachments = { gammaCorrectionAttachmentReference };
		gammaCorrectionSubpass.inputAttachments = { gammaCorrectionAttachmentReference };

		auto deviceRenderPassData = _device.GetRenderPassList();
		ret = deviceRenderPassData.AddRenderPassWithoutResolveAttachments(renderPassAttachments, { colorSubpass, gammaCorrectionSubpass },
			{colorSubpassDependency, gammaCorrectionSubpassDependency});

		return ret;
	}

	VS::GraphicsPipelineCreationData RenderDataInternal::CompileUIPipelinesCreationData(uint32_t width, uint32_t height, IDObject<VS::AutoCleanupRenderPass> renderPass)
	{
		VS::GraphicsPipelineCreationData ret;

		auto sharedPipelineList = _sharedData.GetSharedPipelineDataListss();

		auto vertexShader = sharedPipelineList.AddUniqueSharedShaderPipelineData("main", VS::SHADER_TYPE_VERTEX, 0x10);
		auto fragmentShader = sharedPipelineList.AddUniqueSharedShaderPipelineData("main", VS::SHADER_TYPE_FRAGMENT, 0x10);

		ret.shaderStages.resize(2);
		ret.shaderStages[0].sharedData = vertexShader;
		ret.shaderStages[0].shaderDeviceID.vertexShader.type = VS::SHADER_TYPE_VERTEX;
		ret.shaderStages[0].shaderDeviceID.vertexShader.vertexShaderID = _uiVertexShaderID;
		ret.shaderStages[1].sharedData = fragmentShader;
		ret.shaderStages[1].shaderDeviceID.fragmentShader.type = VS::SHADER_TYPE_FRAGMENT;
		ret.shaderStages[1].shaderDeviceID.fragmentShader.fragmentShaderID = _standardFragmentShaderID;

		auto vertexInTexCoord = sharedPipelineList.AddUniqueVertexAttributeDescriptionData(0, VS::DATA_FORMAT_RG32_SFLOAT, 0x10);
		auto vertexInTexSize = sharedPipelineList.AddUniqueVertexAttributeDescriptionData(8, VS::DATA_FORMAT_RG32_SFLOAT, 0x10);
		auto vertexInTexLayer = sharedPipelineList.AddUniqueVertexAttributeDescriptionData(16, VS::DATA_FORMAT_R32_UINT, 0x10);
		auto vertexInTexIndex = sharedPipelineList.AddUniqueVertexAttributeDescriptionData(20, VS::DATA_FORMAT_R32_UINT, 0x10);
		auto vertexInSize = sharedPipelineList.AddUniqueVertexAttributeDescriptionData(24, VS::DATA_FORMAT_RGBA32_SFLOAT, 0x10);

		auto vertexInstanceBinding = sharedPipelineList.AddUniqueVertexBindingData(40, VS::VertexBindingInputRate::INSTANCE,
			{ vertexInTexCoord, vertexInTexSize, vertexInTexLayer,  vertexInTexIndex, vertexInSize}, 0x10);

		ret.vertexInputData = sharedPipelineList.AddUniqueVertexInputSharedPipelineData({ vertexInstanceBinding }, 0x10);
		ret.inputAssemblyData = sharedPipelineList.AddUniquePipelineInputAssemblyData(VS::PipelinePrimitiveTopology::TRIANGLE_LIST, false, 0x10);

		ret.viewportData.resize(1);
		ret.viewportData[0].scissor = sharedPipelineList.AddUniquePipelineScissorData(0, 0, width, height, 0x10);
		ret.viewportData[0].viewport = sharedPipelineList.AddUniquePipelineViewportData(0, 0, width, height, 0.0f, 1.0f, 0x10);

		ret.rasterizationData = sharedPipelineList.AddUniquePipelineRasterizationData(VS::PipelinePolygonMode::FILL, true, false, 0x10);
		ret.samplingData = sharedPipelineList.AddUniquePipelineMultisampleData(VS::SAMPLE_1, {}, 0x10);
		ret.depthStencilData = sharedPipelineList.AddUniquePipelineDepthStencilStateData(VS::DepthUsage::TEST_AND_WRITE, VS::CompareOperationsType::COMPARE_OPERATION_LESS, 0.0f, 1.0f,
			0x10);
		ret.colorBlendingData.push_back(sharedPipelineList.AddUniquePipelineColorBlendAttachment(VS::ColorBlendingComponentBits::COLOR_COMPONENT_R |
			VS::ColorBlendingComponentBits::COLOR_COMPONENT_G | VS::ColorBlendingComponentBits::COLOR_COMPONENT_B | VS::ColorBlendingComponentBits::COLOR_COMPONENT_A,
			VS::ColorBlendingPreset::ALPHA_BLENDING, 0x10));

		ret.pipelineLayout = _uiPipelineLayout;
		ret.renderPass = renderPass;
		ret.subpassIndex = 0;
		ret.pipelineDerrivationData.settings = VS::PipelineDerrivationSettings::DO_NOT_DERRIVE;

		return ret;
	}

	VS::GraphicsPipelineCreationData RenderDataInternal::CompileGammaCorrectionPipelinesCreationData(uint32_t width, uint32_t height, IDObject<VS::AutoCleanupRenderPass> renderPass)
	{
		VS::GraphicsPipelineCreationData ret;

		auto sharedPipelineList = _sharedData.GetSharedPipelineDataListss();

		auto vertexShader = sharedPipelineList.AddUniqueSharedShaderPipelineData("main", VS::SHADER_TYPE_VERTEX, 0x10);
		auto fragmentShader = sharedPipelineList.AddUniqueSharedShaderPipelineData("main", VS::SHADER_TYPE_FRAGMENT, 0x10);

		ret.shaderStages.resize(2);
		ret.shaderStages[0].sharedData = vertexShader;
		ret.shaderStages[0].shaderDeviceID.vertexShader.type = VS::SHADER_TYPE_VERTEX;
		ret.shaderStages[0].shaderDeviceID.vertexShader.vertexShaderID = _gammaCorrectionVertexShaderID;
		ret.shaderStages[1].sharedData = fragmentShader;
		ret.shaderStages[1].shaderDeviceID.fragmentShader.type = VS::SHADER_TYPE_FRAGMENT;
		ret.shaderStages[1].shaderDeviceID.fragmentShader.fragmentShaderID = _gammaCorrectionFragmentShaderID;

		ret.vertexInputData = sharedPipelineList.AddUniqueVertexInputSharedPipelineData({}, 0x10);
		ret.inputAssemblyData = sharedPipelineList.AddUniquePipelineInputAssemblyData(VS::PipelinePrimitiveTopology::TRIANGLE_LIST, false, 0x10);

		ret.viewportData.resize(1);
		ret.viewportData[0].scissor = sharedPipelineList.AddUniquePipelineScissorData(0, 0, width, height, 0x10);
		ret.viewportData[0].viewport = sharedPipelineList.AddUniquePipelineViewportData(0, 0, width, height, 0.0f, 1.0f, 0x10);

		ret.rasterizationData = sharedPipelineList.AddUniquePipelineRasterizationData(VS::PipelinePolygonMode::FILL, true, false, 0x10);
		ret.samplingData = sharedPipelineList.AddUniquePipelineMultisampleData(VS::SAMPLE_1, {}, 0x10);
		ret.depthStencilData = sharedPipelineList.AddUniquePipelineDepthStencilStateData(VS::DepthUsage::TEST_AND_WRITE, VS::CompareOperationsType::COMPARE_OPERATION_LESS, 0.0f, 1.0f,
			0x10);
		ret.colorBlendingData.push_back(sharedPipelineList.AddUniquePipelineColorBlendAttachment(VS::ColorBlendingComponentBits::COLOR_COMPONENT_R |
			VS::ColorBlendingComponentBits::COLOR_COMPONENT_G | VS::ColorBlendingComponentBits::COLOR_COMPONENT_B | VS::ColorBlendingComponentBits::COLOR_COMPONENT_A,
			VS::ColorBlendingPreset::ALPHA_BLENDING, 0x10));

		ret.pipelineLayout = _gammaCorrectionPipelineLayout;
		ret.renderPass = renderPass;
		ret.subpassIndex = 1;
		ret.pipelineDerrivationData.settings = VS::PipelineDerrivationSettings::DO_NOT_DERRIVE;

		return ret;
	}

}