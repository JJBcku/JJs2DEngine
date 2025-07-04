#include "MainDNIpch.h"
#include "MainInternal.h"

#include "../../../Include/Main/MainInitializationData.h"

#include "../../../Include/Common/EngineVersion.h"
#include "../../../Include/Common/DeviceData.h"

#include "../Common/VersionDataInternal.h"

#include <VulkanSimplified/VSMain/VSMainInitData.h>
#include <VulkanSimplified/VSMain/VSInstanceExtensionPacksList.h>
#include <VulkanSimplified/VSMain/VSInstanceLayerPacksList.h>

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

#include <VulkanSimplified/VSCommon/VSMemoryDataList.h>
#include <VulkanSimplified/VSCommon/VSMemoryHeapProperties.h>
#include <VulkanSimplified/VSCommon/VSMemoryTypeProperties.h>

namespace JJs2DEngine
{
	static bool CheckFormatSwapchainSupport(const VS::FormatsSupportedImageFeaturesList& supportedImageFormats,
		const VS::SurfaceSupportedColorspaceFormatsLists& surfaceColorspace, const VS::DataFormatSetIndependentID& formatID);
	static DeviceData CompileUsefullDeviceData(const VS::PhysicalDeviceData& deviceData, uint64_t deviceIndex);
	static bool CheckDevicesUsefullness(const DeviceData& deviceData, VersionData minVulkanVersion);

	MainInternal::MainInternal(const MainInitializationData& initData)
	{
		VS::MainInitData VSInitData;

		_VSMain = std::make_unique<VS::Main>(VSInitData);

		_minVulkanVersion = initData.minVulkanVersion;
		_maxVulkanVersion = initData.maxVulkanVersion;

		CreateInstance(initData);
		EnumerateDevices();
	}

	MainInternal::~MainInternal()
	{
	}

	const std::vector<DeviceData>& MainInternal::GetDeviceList() const
	{
		return _deviceList;
	}

	void MainInternal::CreateDevice(size_t deviceIndex, const DeviceSettings& deviceSettings)
	{
		if (_currentDevicesSettings.has_value())
			throw std::runtime_error("MainInternal::CreateDevice Error: Function tried to create already existing devices!");

		if (deviceIndex >= _deviceList.size())
			throw std::runtime_error("MainInternal::CreateDevice Error: Function was given an erroneous device index value!");

		const auto& deviceData = _deviceList[deviceIndex];

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

		auto instance = _VSMain->GetInstance();
		instance.CreateLogicalDevice(creationData, {});

		creationData.requestedExtensionPacks.swapchainBase = true;

		_currentDevicesSettings = deviceSettings;
	}

	void MainInternal::RecreateDevice(size_t deviceIndex, const DeviceSettings& deviceSettings)
	{
		if (_currentDevicesSettings.has_value())
			_currentDevicesSettings.reset();

		if (deviceIndex >= _deviceList.size())
			throw std::runtime_error("MainInternal::RecreateDevice Error: Function was given an erroneous device index value!");

		const auto& deviceData = _deviceList[deviceIndex];

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
	}

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

			auto deviceData = CompileUsefullDeviceData(physicalData, i);

			if (CheckDevicesUsefullness(deviceData, _minVulkanVersion))
			{
				deviceData.maxVulkanVersion = std::min(deviceData.maxVulkanVersion, _maxVulkanVersion);
				_deviceList.push_back(deviceData);
			}
		}

		if (_deviceList.empty())
			throw std::runtime_error("MainInternal::EnumerateDevices Error: Program found no usefull GPU's!");
	}

	static bool CheckFormatSwapchainSupport(const VS::FormatsSupportedImageFeaturesList& supportedImageFormats,
		const VS::SurfaceSupportedColorspaceFormatsLists& surfaceColorspace, const VS::DataFormatSetIndependentID& formatID)
	{
		return VS::CheckFormatSupport(supportedImageFormats.blitSrc, formatID) && VS::CheckFormatSupport(supportedImageFormats.blitDst, formatID) &&
			VS::CheckFormatSupport(supportedImageFormats.colorAttachment, formatID) && VS::CheckFormatSupport(supportedImageFormats.sampledImage, formatID) &&
			VS::CheckFormatSupport(surfaceColorspace.srgbNonlinearColorspace, formatID);
	}

	static DeviceData CompileUsefullDeviceData(const VS::PhysicalDeviceData& deviceData, uint64_t deviceIndex)
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

			if ((family.queueTypes & VS::QUEUE_TYPE_GRAPHICS) == VS::QUEUE_TYPE_GRAPHICS && family.presentationSupport == VS::BOOL64_TRUE)
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

			ret.swapchainSupport.swapchainRGB16Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_RGB16_UNORM);
			ret.swapchainSupport.swapchainRGBA16Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_RGBA16_UNORM);

			ret.swapchainSupport.swapchainA2RGB10Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_A2_BGR10_UNORM_PACK32);
			ret.swapchainSupport.swapchainA2BGR10Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_A2_RGB10_UNORM_PACK32);

			ret.swapchainSupport.swapchainRGB8Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_RGB8_UNORM);
			ret.swapchainSupport.swapchainRGBA8Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_RGBA8_UNORM);

			ret.swapchainSupport.swapchainBGR8Unorm = CheckFormatSwapchainSupport(formatImageSupport, formatSwapchainSupport, VS::DATA_FORMAT_BGR8_UNORM);
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