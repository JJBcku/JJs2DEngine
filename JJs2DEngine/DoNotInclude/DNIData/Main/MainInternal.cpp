#include "MainDNIpch.h"
#include "MainInternal.h"

#include "../../../Include/Main/MainInitializationData.h"

#include "../../../Include/Common/EngineVersion.h"
#include "../../../Include/Common/DeviceData.h"

#include "../Common/VersionDataInternal.h"

#include "DeviceSettingsInternal.h"

#include "WindowDataInternal.h"
#include "RenderDataInternal.h"
#include "InputDataListInternal.h"
#include "TextureDataMainInternal.h"

#include <Miscellaneous/Bool64.h>

#include <VulkanSimplified/VSMain/VSMainInitData.h>
#include <VulkanSimplified/VSMain/VSInstanceExtensionPacksList.h>
#include <VulkanSimplified/VSMain/VSInstanceLayerPacksList.h>

#include <VulkanSimplified/VSMain/EventHandler/SdlEventHandler.h>

#include <VulkanSimplified/VSInstance/VSInstanceCreationData.h>
#include <VulkanSimplified/VSInstance/VSDeviceSwapchainSupportData.h>
#include <VulkanSimplified/VSInstance/VSDevicesSupportedFormats.h>
#include <VulkanSimplified/VSInstance/VSPhysicalDeviceData.h>

#include <VulkanSimplified/VSInstance/VSDeviceVulkan10Properties.h>
#include <VulkanSimplified/VSInstance/VSDeviceVulkan10FeatureFlags.h>
#include <VulkanSimplified/VSInstance/VSDeviceQueueFamilyData.h>

#include <VulkanSimplified/VSInstance/VSInstance.h>
#include <VulkanSimplified/VSInstance/VSLogicalDeviceCreateInfo.h>

#include <VulkanSimplified/VSDevice/VSDeviceInitialCapacitiesList.h>
#include <VulkanSimplified/VSDevice/VSDeviceMain.h>
#include <VulkanSimplified/VSDevice/VSDeviceCore.h>
#include <VulkanSimplified/VSDevice/VSSynchronizationDataLists.h>
#include <VulkanSimplified/VSDevice/VSWindowList.h>

#include <VulkanSimplified/VSCommon/VSMemoryDataList.h>
#include <VulkanSimplified/VSCommon/VSMemoryHeapProperties.h>
#include <VulkanSimplified/VSCommon/VSMemoryTypeProperties.h>

#include <VulkanSimplified/VSCommon/VSCompositeAlphaFlags.h>
#include <VulkanSimplified/VSCommon/VSSurfacePresentModes.h>
#include <VulkanSimplified/VSCommon/VSSurfaceTransformFlags.h>
#include <VulkanSimplified/VSCommon/VSImageUsageFlags.h>
#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>

namespace JJs2DEngine
{
	static bool CheckFormatSwapchainSupport(const VS::FormatsSupportedImageFeaturesList& supportedImageFormats,
		const VS::SurfaceSupportedColorspaceFormatsLists& surfaceColorspace, const VS::DataFormatSetIndependentID& formatID);
	static DeviceData CompileUsefullDeviceData(const VS::PhysicalDeviceData& deviceData, uint64_t deviceIndex, uint64_t max2DImagesDimensions, uint64_t maxLayersInImageArray);
	static bool CheckDevicesUsefullness(const DeviceData& deviceData, VersionData minVulkanVersion);

	MainInternal::MainInternal(const MainInitializationData& initData)
	{
		VS::MainInitData VSInitData;

		_VSMain = std::make_unique<VS::Main>(VSInitData);

		_minVulkanVersion = initData.minVulkanVersion;
		_maxVulkanVersion = initData.maxVulkanVersion;

		_dataFolder = initData.dataFolder;

		_windowClosed = Misc::BOOL64_FALSE;

		auto eventHandler = _VSMain->GetSdlEventHandler();
		_quitRegistrationID = eventHandler.RegisterQuitEventCallback(MainInternal::HandleQuitEventStatic, this, 0);

		CreateInstance(initData);
		EnumerateDevices();

		_currentTime = std::chrono::high_resolution_clock::now();
	}

	MainInternal::~MainInternal()
	{
		if (_VSMain)
		{
			auto eventHandler = _VSMain->GetSdlEventHandler();
			eventHandler.UnRegisterQuitEventCallback(_quitRegistrationID, false);
		}
	}

	const std::vector<DeviceData>& MainInternal::GetDeviceList() const
	{
		return _deviceList;
	}

	void MainInternal::CreateDevice(const DeviceSettings& deviceSettings)
	{
		if (_currentDevicesSettings.has_value())
			throw std::runtime_error("MainInternal::CreateDevice Error: Function tried to create already existing devices!");

		if (deviceSettings.deviceIndex >= _deviceList.size())
			throw std::runtime_error("MainInternal::CreateDevice Error: Function was given an erroneous device index value!");

		if (deviceSettings.currentPipelineSettings >= deviceSettings.preInitializedPipelineSettings.size())
			throw std::runtime_error("MainInternal::CreateDevice Error: Function was given an erroneous current pipeline index value!");

		const auto& deviceData = _deviceList[deviceSettings.deviceIndex];

		if (deviceSettings.graphicsFramesInFlight < deviceData.swapchainSupport.minFramesInFlight)
			throw std::runtime_error("MainInternal::CreateDevice Error: Function was given frames in flight value below the minimum!");

		if (deviceSettings.graphicsFramesInFlight > deviceData.swapchainSupport.maxFramesInFlight)
			throw std::runtime_error("MainInternal::CreateDevice Error: Function was given frames in flight value above the maximum!");

		if (!CheckTexturesFormatAvailability(deviceSettings.textureFormat, deviceData.textureSupport))
			throw std::runtime_error("MainInternal::CreateDevice Error: The choosen texture format is unsupported by the device!");

		for (auto& setting : deviceSettings.preInitializedPipelineSettings)
		{
			if (!CheckSwapchainFormatAvailability(setting.swapchainFormat, deviceData.swapchainSupport))
				throw std::runtime_error("MainInternal::CreateDevice Error: One of preinitialized pipelines has a swapchain format unsupported by the device!");

			if (!CheckDepthFormatAvailability(setting.depthFormat, deviceData.depthStencilSupport))
				throw std::runtime_error("MainInternal::CreateDevice Error: One of preinitialized pipelines has a depth format unsupported by the device!");
		}
		
		if (deviceSettings.preLoadedTexturesStagingBuffersPageCount == 0)
			throw std::runtime_error("MainInternal::CreateDevice Error: Value for preloaded textures staging buffer page count cannot be zero!");

		if (deviceSettings.streamedTexturesStagingBuffersPageCount == 0 && deviceSettings.transferFramesInFlight > 0)
			throw std::runtime_error("MainInternal::CreateDevice Error: Value for streamed textures staging buffer page count cannot be zero!");

		VS::LogicalDeviceCreationData creationData;
		creationData.physicalGPUIndex = deviceData.deviceIndex;
		creationData.queuesCreationInfo.reserve(2);

		VS::QueueCreationData queueData;
		queueData.queuesPriorities.push_back(std::numeric_limits<uint16_t>::max());

		if (deviceData.queueSupport.noVideoCodingGraphicQueueFamily.has_value())
		{
			queueData.queuesFamily = deviceData.queueSupport.noVideoCodingGraphicQueueFamily.value();
		}
		else
		{
			queueData.queuesFamily = deviceData.queueSupport.videoCodingGraphicQueueFamily.value();
		}

		creationData.queuesCreationInfo.push_back(queueData);
		_graphicsQueueIndex = 0;

		if (deviceData.queueSupport.transferQueueFamily.has_value())
		{
			queueData.queuesFamily = deviceData.queueSupport.transferQueueFamily.value();
			creationData.queuesCreationInfo.push_back(queueData);
			_transferOnlyQueueIndex = 1;
		}
		else if (deviceData.queueSupport.computeQueueFamily.has_value())
		{
			queueData.queuesFamily = deviceData.queueSupport.computeQueueFamily.value();
			creationData.queuesCreationInfo.push_back(queueData);
			_transferOnlyQueueIndex = 1;
		}

		creationData.requestedExtensionPacks.swapchainBase = true;

		auto instance = _VSMain->GetInstance();
		instance.CreateLogicalDevice(creationData, {});

		_currentDevicesSettings = deviceSettings;

		_lastCurrentTime = _currentTime;
		_currentTime = std::chrono::high_resolution_clock::now();

		auto device = instance.GetChoosenDevicesMainClass();

		_renderDataList = std::make_unique<RenderDataInternal>(deviceSettings.currentPipelineSettings, deviceSettings.preInitializedPipelineSettings, _dataFolder,
			device, _VSMain->GetSharedDataMainList());

		{
			auto& currentPipeline = deviceSettings.preInitializedPipelineSettings[deviceSettings.currentPipelineSettings];

			RenderImagesData windowRenderData;
			windowRenderData.framesInFlight = deviceSettings.graphicsFramesInFlight;
			windowRenderData.colorFormat = TranslateToFormat(currentPipeline.swapchainFormat);
			windowRenderData.depthFormat = TranslateToFormat(currentPipeline.depthFormat);

			windowRenderData.renderImagesWidth = currentPipeline.renderWidth;
			windowRenderData.renderImagesHeight = GetHeight(currentPipeline.renderWidth, currentPipeline.aspectRatio);
			windowRenderData.renderPassID = _renderDataList->GetCurrentRenderPass();

			_windowData = std::make_unique<WindowDataInternal>(deviceSettings.windowData, windowRenderData, device.GetWindowList(), device.GetSynchronizationDataLists(),
				device.GetImageDataLists(), device.GetMemoryObjectsList());
		}

		_inputDataList = std::make_unique<InputDataListInternal>(_currentTime, _VSMain->GetSdlEventHandler());

		{
			auto& preLoadedTexturesMaxAmounts = _currentDevicesSettings.value().preLoadedTexturesMaxAmounts;
			auto& streamedTexturesMaxAmounts = _currentDevicesSettings.value().streamedTexturesMaxAmounts;

			auto physicalDevice = instance.GetPhysicalDeviceData(_deviceList[_currentDevicesSettings.value().deviceIndex].deviceIndex);
			auto& imageLimits = physicalDevice.GetVulkan10Properties().limits.maxImageSizes;

			TextureDataMainInitData textureInitData;
			textureInitData.transferFramesInFlight = _currentDevicesSettings.value().transferFramesInFlight;
			textureInitData.max2DImageSize = imageLimits.maxImageDimension2D;
			textureInitData.maxImageArrayLayers = imageLimits.maxImageArrayLayers;
			textureInitData.dataFolder = _dataFolder;
			textureInitData.preLoadedTexturesMaxAmounts = preLoadedTexturesMaxAmounts;
			textureInitData.streamedTexturesMaxAmounts = streamedTexturesMaxAmounts;
			textureInitData.preLoadedTexturesStagingBufferPageCount = _currentDevicesSettings.value().preLoadedTexturesStagingBuffersPageCount;
			textureInitData.streamedTexturesStagingBufferPageCount = _currentDevicesSettings.value().streamedTexturesStagingBuffersPageCount;
			textureInitData.textureFormat = _currentDevicesSettings.value().textureFormat;

			_textureDataMain = std::make_unique<TextureDataMainInternal>(textureInitData, device.GetDataBufferLists(), device.GetImageDataLists(), device.GetMemoryObjectsList());
		}
	}

	void MainInternal::UpdateCurrentTime()
	{
		_lastCurrentTime = _currentTime;
		_currentTime = std::chrono::high_resolution_clock::now();

		_inputDataList->UpdateCurrentTime(_currentTime);
	}

	Misc::Bool64 MainInternal::IsWindowClosed() const
	{
		return _windowClosed;
	}

	void MainInternal::HandleEvents()
	{
		auto eventHandler = _VSMain->GetSdlEventHandler();
		eventHandler.HandleEvents();
	}

	void MainInternal::WaitForIdleDevice() const
	{
		auto deviceCore = _VSMain->GetInstance().GetChoosenDevicesMainClass().GetDeviceCore();
		deviceCore.WaitIdle();
	}

	void MainInternal::ChangeFullscreen(Misc::Bool64Values newFullscreen)
	{
		_windowData->ChangeFullscreen(newFullscreen);
	}

	InputDataListInternal& MainInternal::GetInputDataList()
	{
		return *_inputDataList;
	}

	const InputDataListInternal& MainInternal::GetInputDataList() const
	{
		return *_inputDataList;
	}

	/*void MainInternal::RecreateDevice(const DeviceSettings& deviceSettings)
	{
		if (_currentDevicesSettings.has_value())
		{
			_currentDevicesSettings.reset();
			_pipelineList.reset();
		}

		if (deviceSettings.deviceIndex >= _deviceList.size())
			throw std::runtime_error("MainInternal::RecreateDevice Error: Function was given an erroneous device index value!");

		const auto& deviceData = _deviceList[deviceSettings.deviceIndex];

		VS::LogicalDeviceCreationData creationData;
		creationData.physicalGPUIndex = deviceData.deviceIndex;
		creationData.queuesCreationInfo.reserve(2);

		VS::QueueCreationData queueData;
		queueData.queuesPriorities.push_back(std::numeric_limits<uint16_t>::max());
		_graphicsQueueIndex = 0;

		_transferOnlyQueueIndex.reset();

		if (deviceData.queueSupport.noVideoCodingGraphicQueueFamily.has_value())
		{
			queueData.queuesFamily = deviceData.queueSupport.noVideoCodingGraphicQueueFamily.value();
			_transferOnlyQueueIndex = 1;
		}
		else
		{
			queueData.queuesFamily = deviceData.queueSupport.videoCodingGraphicQueueFamily.value();
			_transferOnlyQueueIndex = 1;
		}

		creationData.queuesCreationInfo.push_back(queueData);

		if (deviceData.queueSupport.transferQueueFamily.has_value())
		{
			queueData.queuesFamily = deviceData.queueSupport.transferQueueFamily.value();
			creationData.queuesCreationInfo.push_back(queueData);
		}
		else if (deviceData.queueSupport.computeQueueFamily.has_value())
		{
			queueData.queuesFamily = deviceData.queueSupport.computeQueueFamily.value();
			creationData.queuesCreationInfo.push_back(queueData);
		}

		creationData.requestedExtensionPacks.swapchainBase = true;

		auto instance = _VSMain->GetInstance();
		instance.CreateLogicalDevice(creationData, {});

		_currentDevicesSettings = deviceSettings;
		_pipelineList = std::make_unique<RenderDataInternal>(deviceSettings.currentPipelineSettings, deviceSettings.preInitializedPipelineSettings,
			instance.GetChoosenDevicesMainClass(), _VSMain->GetSharedDataMainList());
	}*/

	void MainInternal::CreateInstance(const MainInitializationData& initData)
	{
		auto availableExtensionPacks = _VSMain->GetAvailableInstanceExtensionPacks();
		auto availableLayerPacks = _VSMain->GetAvailableInstanceLayerPacks();

		if (!availableExtensionPacks.sdlRequiredExtensions)
			throw std::runtime_error("MainInternal::CreateInstance Error: Instance extensions required by SDL are not available!");

#if defined(_DEBUG) || defined(DEBUG) || defined(DEBUG_UTILS)
		if (!availableExtensionPacks.debugUtils)
			throw std::runtime_error("MainInternal::CreateInstance Error: Debugging instance extensions are not available!");

		if (!availableLayerPacks.debugUtils)
			throw std::runtime_error("MainInternal::CreateInstance Error: Debugging layers are not available!");
#endif

		VS::InstanceCreationData instanceInit;
		instanceInit.appName = initData.appName;
		instanceInit.appVariantName = initData.appVariantName;

		instanceInit.engineName = "JJs2DEngine";
		instanceInit.engineVersion = GetVSVersionData(engineVersion);

		JJs2DEngine::VersionData maxVulkanVersion = GetVersionData(_VSMain->GetMaxAvailableVulkanVersion());

		if (initData.minVulkanVersion > maxVulkanVersion)
			throw std::runtime_error("MainInternal::CreateInstance Error: Highest available Vulkan version is below minimal requirements!");

		instanceInit.usedVulkanApiVersion = GetVSVersionData(std::min(initData.maxVulkanVersion, maxVulkanVersion));
		instanceInit.enabledExtensionPacks.sdlRequiredExtensions = true;

#if defined(_DEBUG) || defined(DEBUG) || defined(DEBUG_UTILS)
		instanceInit.enabledExtensionPacks.debugUtils = true;

		instanceInit.enabledLayerPacks.debugUtils = true;
#endif

		_VSMain->CreateInstance(instanceInit);
	}

	void MainInternal::EnumerateDevices()
	{
		auto instance = _VSMain->GetInstance();

		size_t deviceAmount = instance.GetAvailableDevicesCount();
		_deviceList.reserve(deviceAmount);

		for (size_t i = 0; i < deviceAmount; ++i)
		{
			auto physicalData = instance.GetPhysicalDeviceData(i);

			if (!physicalData.GetDeviceExtensionPacks().swapchainBase)
				continue;

			auto& surfaceSupport = physicalData.GetSurfaceSupport();

			auto& properties = physicalData.GetVulkan10Properties();
			auto& maxImageSizes = properties.limits.maxImageSizes;
			
			if (maxImageSizes.maxImageDimension2D < 8192)
				continue;

			if (!surfaceSupport.has_value())
				continue;

			if ((surfaceSupport.value().surfaceUsageFlags & VS::IMAGE_USAGE_TRANSFER_DST) != VS::IMAGE_USAGE_TRANSFER_DST)
				continue;

			if ((surfaceSupport.value().surfaceSupportedCompositeAlphaModes & VS::COMPOSITE_ALPHA_OPAQUE) != VS::COMPOSITE_ALPHA_OPAQUE)
				continue;

			if ((surfaceSupport.value().surfaceSupportedTransformations & VS::SURFACE_TRASFORM_IDENTITY) != VS::SURFACE_TRASFORM_IDENTITY)
				continue;

			if ((surfaceSupport.value().surfacePresentModes & VS::PRESENT_MODE_FIFO_STRICT) != VS::PRESENT_MODE_FIFO_STRICT)
				continue;

			auto deviceData = CompileUsefullDeviceData(physicalData, i, maxImageSizes.maxImageDimension2D, maxImageSizes.maxImageArrayLayers);

			if (CheckDevicesUsefullness(deviceData, _minVulkanVersion))
			{
				deviceData.maxVulkanVersion = std::min(deviceData.maxVulkanVersion, _maxVulkanVersion);
				_deviceList.push_back(deviceData);
			}
		}

		if (_deviceList.empty())
			throw std::runtime_error("MainInternal::EnumerateDevices Error: Program found no usefull GPU's!");
	}

	bool MainInternal::CheckSwapchainFormatAvailability(SwapchainFormat format, const DeviceSwapchainSupport& deviceData)
	{
		bool ret = false;

		switch (format)
		{
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_UNSET:
			break;
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_RGBA16:
			ret = deviceData.swapchainRGBA16Unorm;
			break;
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_A2RGB10:
			ret = deviceData.swapchainA2RGB10Unorm;
			break;
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_A2BGR10:
			ret = deviceData.swapchainA2BGR10Unorm;
			break;
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_RGBA8:
			ret = deviceData.swapchainRGBA8Unorm;
			break;
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_BGRA8:
			ret = deviceData.swapchainBGRA8Unorm;
			break;
		case JJs2DEngine::SwapchainFormat::SWAPCHAIN_FORMAT_ABGR8:
			ret = deviceData.swapchainABGR8Unorm;
			break;
		default:
			break;
		}

		return ret;
	}

	bool MainInternal::CheckTexturesFormatAvailability(TextureFormat format, const DeviceTextureSupport& deviceData)
	{
		bool ret = false;

		switch (format)
		{
		case JJs2DEngine::TextureFormat::TEXTURE_FORMAT_UNSET:
			break;
		case JJs2DEngine::TextureFormat::TEXTURE_FORMAT_RGBA16:
			ret = deviceData.textureRGBA16UNORM;
			break;
		case JJs2DEngine::TextureFormat::TEXTURE_FORMAT_RGBA8:
			ret = deviceData.textureRGBA8UNORM;
			break;
		case JJs2DEngine::TextureFormat::TEXTURE_FORMAT_BGRA8:
			ret = deviceData.textureBGRA8UNORM;
			break;
		default:
			break;
		}

		return ret;
	}

	bool MainInternal::CheckDepthFormatAvailability(DepthFormat format, const DeviceDepthStencilSupport& deviceData)
	{
		bool ret = false;

		switch (format)
		{
		case JJs2DEngine::DepthFormat::DEPTH_FORMAT_UNSET:
			break;
		case JJs2DEngine::DepthFormat::DEPTH_FORMAT_D32:
			ret = deviceData.D32Float;
			break;
		case JJs2DEngine::DepthFormat::DEPTH_FORMAT_D32_S8:
			ret = deviceData.D32FloatS8Int;
			break;
		default:
			break;
		}

		return ret;
	}

	bool MainInternal::HandleQuitEvent()
	{
		_windowClosed = Misc::BOOL64_TRUE;
		return true;
	}

	bool MainInternal::HandleQuitEventStatic(const VS::SdlQuitEventData&, void* instancePointer)
	{
		if (instancePointer == nullptr)
			throw std::runtime_error("MainInternal::HandleQuitEventStatic Error: Program tried to dereference null pointer!");

		return static_cast<MainInternal*>(instancePointer)->HandleQuitEvent();
	}

	static bool CheckFormatSwapchainSupport(const VS::FormatsSupportedImageFeaturesList& supportedImageFormats,
		const VS::SurfaceSupportedColorspaceFormatsLists& surfaceColorspace, const VS::DataFormatSetIndependentID& formatID)
	{
		return VS::CheckFormatSupport(supportedImageFormats.blitSrc, formatID) &&
			VS::CheckFormatSupport(supportedImageFormats.blitDst, formatID) &&
			VS::CheckFormatSupport(supportedImageFormats.colorAttachment, formatID) &&
			VS::CheckFormatSupport(supportedImageFormats.sampledImage, formatID) &&
			VS::CheckFormatSupport(surfaceColorspace.srgbNonlinearColorspace, formatID);
	}

	static DeviceData CompileUsefullDeviceData(const VS::PhysicalDeviceData& deviceData, uint64_t deviceIndex, uint64_t max2DImagesDimensions, uint64_t maxLayersInImageArray)
	{
		DeviceData ret;
		ret.deviceIndex = deviceIndex;

		auto& deviceMemory = deviceData.GetAvailableMemoryDataList();
		auto& deviceFeatures = deviceData.GetVulkan10Features();

		for (size_t i = 0; i < deviceMemory.heapAmount; ++i)
		{
			auto& memoryHeap = deviceMemory.memoryHeaps[i];

			if ((memoryHeap.properties & VS::MemoryHeapPropertyFlags::DEVICE_LOCAL_HEAP) != VS::MemoryHeapPropertyFlags::DEVICE_LOCAL_HEAP)
				continue;

			bool foundShared = false;
			bool foundNotShared = false;
			bool foundLazilyAllocated = false;

			for (size_t j = 0; j < memoryHeap.memoryTypeAmount; ++j)
			{
				if ((memoryHeap.memoryTypes[j].properties & VS::MemoryTypePropertyFlags::HOST_VISIBLE) == VS::MemoryTypePropertyFlags::HOST_VISIBLE && !foundShared)
				{
					foundShared = true;
					
					ret.sharedMemory += memoryHeap.size;
				}
				else if ((memoryHeap.memoryTypes[j].properties & VS::MemoryTypePropertyFlags::LAZILY_ALLOCATED) == VS::MemoryTypePropertyFlags::HOST_VISIBLE && !foundLazilyAllocated)
				{
					foundLazilyAllocated = true;

					ret.lazilyAllocatedMemory += memoryHeap.size;
				}
				else if (!foundNotShared)
				{
					foundNotShared = true;

					ret.deviceLocalMemory += memoryHeap.size;
				}

				if (foundShared && foundNotShared && foundLazilyAllocated)
					break;
			}
		}

		ret.maxVulkanVersion = GetVersionData(VS::VersionData(deviceData.GetVulkan10Properties().apiMaxSupportedVersion));

		auto& queueData = deviceData.GetVulkanQueueFamiliesData();

		for (size_t i = 0; i < queueData.size(); ++i)
		{
			auto& family = queueData[i];

			auto biggestGranularity = std::max(std::max(family.minImageTransferGranularityWidth, family.minImageTransferGranularityHeight), family.minImageTransferGranularityDepth);
			bool better = false;

			if ((family.queueTypes & VS::QUEUE_TYPE_GRAPHICS) == VS::QUEUE_TYPE_GRAPHICS && family.presentationSupport == Misc::BOOL64_TRUE)
			{
				if ((family.queueTypes & VS::QUEUE_TYPE_VIDEO_ENCODE) != VS::QUEUE_TYPE_VIDEO_ENCODE &&
					(family.queueTypes & VS::QUEUE_TYPE_VIDEO_DECODE) != VS::QUEUE_TYPE_VIDEO_DECODE)
				{
					if (!ret.queueSupport.noVideoCodingGraphicQueueFamily.has_value())
					{
						better = true;
					}
					else if (ret.queueSupport.noVideoCodingGraphicQueueFamilyImageGranularity > biggestGranularity)
					{
						better = true;
					}
					else if (ret.queueSupport.noVideoCodingGraphicQueueFamilyTimestampGranularity < family.timespampValidBits)
					{
						better = true;
					}

					if (better)
					{
						ret.queueSupport.noVideoCodingGraphicQueueFamily = static_cast<uint32_t>(i);
						ret.queueSupport.noVideoCodingGraphicQueueFamilyImageGranularity = biggestGranularity;
						ret.queueSupport.noVideoCodingGraphicQueueFamilyTimestampGranularity = family.timespampValidBits;
					}
				}
				else
				{
					if (!ret.queueSupport.videoCodingGraphicQueueFamily.has_value())
					{
						better = true;
					}
					else if (ret.queueSupport.videoCodingGraphicQueueFamilyImageGranularity > biggestGranularity)
					{
						better = true;
					}
					else if (ret.queueSupport.videoCodingGraphicQueueFamilyTimestampGranularity < family.timespampValidBits)
					{
						better = true;
					}

					if (better)
					{
						ret.queueSupport.videoCodingGraphicQueueFamily = static_cast<uint32_t>(i);
						ret.queueSupport.videoCodingGraphicQueueFamilyImageGranularity = biggestGranularity;
						ret.queueSupport.videoCodingGraphicQueueFamilyTimestampGranularity = family.timespampValidBits;
					}
				}
			}
			else if ((family.queueTypes & VS::QUEUE_TYPE_COMPUTE) == VS::QUEUE_TYPE_COMPUTE)
			{
				if (!ret.queueSupport.computeQueueFamily.has_value())
				{
					better = true;
				}
				else if (ret.queueSupport.computeQueueFamilyImageGranularity > biggestGranularity)
				{
					better = true;
				}
				else if (ret.queueSupport.computeQueueFamilyTimestampGranularity < family.timespampValidBits)
				{
					better = true;
				}

				if (better)
				{
					ret.queueSupport.computeQueueFamily = static_cast<uint32_t>(i);
					ret.queueSupport.computeQueueFamilyImageGranularity = biggestGranularity;
					ret.queueSupport.computeQueueFamilyTimestampGranularity = family.timespampValidBits;
				}
			}
			else if ((family.queueTypes & VS::QUEUE_TYPE_TRANSFER) == VS::QUEUE_TYPE_TRANSFER)
			{
				if (!ret.queueSupport.transferQueueFamily.has_value())
				{
					better = true;
				}
				else if (ret.queueSupport.transferQueueFamilyImageGranularity > biggestGranularity)
				{
					better = true;
				}
				else if (ret.queueSupport.transferQueueFamilyTimestampGranularity < family.timespampValidBits)
				{
					better = true;
				}

				if (better)
				{
					ret.queueSupport.transferQueueFamily = static_cast<uint32_t>(i);
					ret.queueSupport.transferQueueFamilyImageGranularity = biggestGranularity;
					ret.queueSupport.transferQueueFamilyTimestampGranularity = family.timespampValidBits;
				}
			}
		}

		auto& formatImageSupport = deviceData.GetFormatsSupportedFeatures().formatFeaturesOptimalImageSupport;
		auto& deviceSwapchainSupport = deviceData.GetSurfaceSupport().value();
		auto& formatSwapchainSupport = deviceSwapchainSupport.surfaceSupportedSwapchainFormats;
		
		{
			ret.swapchainSupport.minFramesInFlight = deviceSwapchainSupport.minImageCount;
			ret.swapchainSupport.maxFramesInFlight = deviceSwapchainSupport.maxImageCount;

			ret.swapchainSupport.swapchainRGBA16Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_RGBA16_UNORM);

			ret.swapchainSupport.swapchainA2RGB10Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_A2_RGB10_UNORM_PACK32);
			ret.swapchainSupport.swapchainA2BGR10Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_A2_BGR10_UNORM_PACK32);

			ret.swapchainSupport.swapchainRGBA8Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_RGBA8_UNORM);
			ret.swapchainSupport.swapchainBGRA8Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_BGRA8_UNORM);
			ret.swapchainSupport.swapchainABGR8Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_ABGR8_UNORM_PACK32);
		}

		{
			ret.textureSupport.textureRGBA16UNORM = VS::CheckFormatSupport(formatImageSupport.sampledImage, VS::DATA_FORMAT_RGBA16_UNORM);

			ret.textureSupport.textureRGBA8UNORM = VS::CheckFormatSupport(formatImageSupport.sampledImage, VS::DATA_FORMAT_RGBA8_UNORM);
			ret.textureSupport.textureBGRA8UNORM = VS::CheckFormatSupport(formatImageSupport.sampledImage, VS::DATA_FORMAT_BGRA8_UNORM);

			ret.textureSupport.textureBC = (deviceFeatures & VS::DEVICE_VULKAN10_FEATURE_TEXTURE_COMPRESSION_BC) == VS::DEVICE_VULKAN10_FEATURE_TEXTURE_COMPRESSION_BC;
			ret.textureSupport.textureETC2 = (deviceFeatures & VS::DEVICE_VULKAN10_FEATURE_TEXTURE_COMPRESSION_ETC2) == VS::DEVICE_VULKAN10_FEATURE_TEXTURE_COMPRESSION_ETC2;
			ret.textureSupport.textureASTC = (deviceFeatures & VS::DEVICE_VULKAN10_FEATURE_TEXTURE_COMPRESSION_ASTC_LDR) == VS::DEVICE_VULKAN10_FEATURE_TEXTURE_COMPRESSION_ASTC_LDR;
		}

		{
			ret.depthStencilSupport.D32Float = VS::CheckFormatSupport(formatImageSupport.depthStencilAttachment, VS::DATA_FORMAT_D32_SFLOAT);
			ret.depthStencilSupport.D32FloatS8Int = VS::CheckFormatSupport(formatImageSupport.depthStencilAttachment, VS::DATA_FORMAT_D32_SFLOAT_S8_UINT);
		}

		for (size_t i = 0; i < ret.maxTexturesInAtlases.size(); ++i)
		{
			size_t textureSize = 1ULL << i;

			size_t maxTextures = max2DImagesDimensions / textureSize;
			maxTextures *= maxTextures;
			maxTextures *= maxLayersInImageArray;

			ret.maxTexturesInAtlases[i] = maxTextures - 1;
		}

		return ret;
	}

	bool CheckDevicesUsefullness(const DeviceData& deviceData, VersionData minVulkanVersion)
	{
		if (deviceData.maxVulkanVersion < minVulkanVersion)
			return false;

		if (!deviceData.queueSupport.HasAnyGraphicsQueue())
			return false;

		if (!deviceData.swapchainSupport.SupportAnySwapchainFormatSize())
			return false;

		if (!deviceData.textureSupport.SupportAnyUncompressedTextures())
			return false;

		if (!deviceData.depthStencilSupport.SupportAnyDepthFormat())
			return false;

		return true;
	}

}