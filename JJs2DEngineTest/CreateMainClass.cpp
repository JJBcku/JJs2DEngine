#include "pch.h"
#include "CreateMainClass.h"

#include "MainDataCollection.h"

#include <Miscellaneous/Bool64.h>

#include <MainInitializationData.h>
#include <Main.h>

#include <EngineVersion.h>
#include <DeviceData.h>

#include <DeviceSettings.h>
#include <AspectRatio.h>

static bool CompareDevices(const JJ2DE::DeviceData& currentBest, const JJ2DE::DeviceData& compared);
static JJ2DE::DeviceSettings CreateDeviceSettings(const JJ2DE::DeviceData& device, size_t deviceIndex);

void CreateMainClass(MainDataCollection& data)
{
	JJ2DE::MainInitializationData mainInitData;

	mainInitData.appName = "JJs2DEngine Test";

#if defined(_DEBUG) || defined(DEBUG)
	mainInitData.appVariantName = "x64 Debug";
#else
	mainInitData.appVariantName = "x64 Release";
#endif

	mainInitData.appVersion = JJ2DE::engineVersion;

	mainInitData.minVulkanVersion = { 1,0,0,0 };
	mainInitData.maxVulkanVersion = { 1,0,0,0 };

	data.main = std::make_unique<JJ2DE::Main>(mainInitData);

	auto& deviceList = data.main->GetDeviceList();

	size_t currentBest = 0;

	for (size_t i = 1; i < deviceList.size(); ++i)
	{
		if (CompareDevices(deviceList[currentBest], deviceList[i]))
			currentBest = i;
	}

	auto deviceSettings = CreateDeviceSettings(deviceList[currentBest], currentBest);

	if (deviceSettings.textureFormat == JJ2DE::TextureFormat::TEXTURE_FORMAT_RGBA16)
		data.is16Bit = true;

	if (deviceSettings.textureFormat == JJ2DE::TextureFormat::TEXTURE_FORMAT_BGRA8)
		data.isRBReversed = true;

	data.main->CreateDevice(deviceSettings);
}

bool CompareDevices(const JJ2DE::DeviceData& currentBest, const JJ2DE::DeviceData& compared)
{
	if (currentBest.maxVulkanVersion >= compared.maxVulkanVersion)
		return false;

	if (currentBest.deviceLocalMemory >= compared.deviceLocalMemory)
		return false;

	if (currentBest.queueSupport.noVideoCodingGraphicQueueFamily.has_value() && !compared.queueSupport.noVideoCodingGraphicQueueFamily.has_value())
		return false;

	if (currentBest.queueSupport.transferQueueFamily.has_value() && !compared.queueSupport.transferQueueFamily.has_value())
		return false;
	else if ((!currentBest.queueSupport.transferQueueFamily.has_value() && !compared.queueSupport.transferQueueFamily.has_value()) &&
		(currentBest.queueSupport.computeQueueFamily.has_value() && !compared.queueSupport.computeQueueFamily.has_value()))
		return false;

	if (currentBest.swapchainSupport.Support16BitSwapchain() && !compared.swapchainSupport.Support16BitSwapchain())
		return false;
	else if ((!currentBest.swapchainSupport.Support16BitSwapchain() && !compared.swapchainSupport.Support16BitSwapchain()) &&
		(currentBest.swapchainSupport.Support10BitSwapchain() && !compared.swapchainSupport.Support10BitSwapchain()))
		return false;

	if (currentBest.textureSupport.Support16BitUncompressedTextures() && !compared.textureSupport.Support16BitUncompressedTextures())
		return false;

	if (currentBest.depthStencilSupport.D32Float && !compared.depthStencilSupport.D32Float)
		return false;

	return true;
}

JJ2DE::DeviceSettings CreateDeviceSettings(const JJ2DE::DeviceData& device, size_t deviceIndex)
{
	JJ2DE::DeviceSettings ret;
	ret.deviceIndex = deviceIndex;

	ret.graphicsFramesInFlight = std::min(device.swapchainSupport.minFramesInFlight + 1, device.swapchainSupport.maxFramesInFlight);
	ret.transferFramesInFlight = std::min(ret.graphicsFramesInFlight - 1, 2U);

	{
		if (device.textureSupport.textureRGBA16UNORM)
			ret.textureFormat = JJ2DE::TextureFormat::TEXTURE_FORMAT_RGBA16;
		else if (device.textureSupport.textureRGBA8UNORM)
			ret.textureFormat = JJ2DE::TextureFormat::TEXTURE_FORMAT_RGBA8;
		else
			ret.textureFormat = JJ2DE::TextureFormat::TEXTURE_FORMAT_BGRA8;
	}

	//ret.preLoadedTexturesMaxAmounts[6] = 1;
	ret.streamedTexturesMaxAmounts[6] = 1;

	ret.preLoadedTexturesStagingBuffersPageCount = 1;
	ret.streamedTexturesStagingBuffersPageCount = 1;

	ret.windowData.windowTitle = "JJ2DEngine Test Window";
	ret.windowData.windowWidth = 1280;
	ret.windowData.aspectRatio = JJ2DE::AspectRatio::ASPECT_RATIO_16_9;
	ret.windowData.fullscreenWindow = Misc::BOOL64_FALSE;

	JJ2DE::PipelineSettings currentPipelineSettings;

	currentPipelineSettings.renderWidth = 1920;
	currentPipelineSettings.aspectRatio = JJ2DE::AspectRatio::ASPECT_RATIO_16_9;

	{
		if (device.swapchainSupport.swapchainRGBA16Unorm)
			currentPipelineSettings.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_RGBA16;
		else if (device.swapchainSupport.swapchainA2RGB10Unorm)
			currentPipelineSettings.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_A2RGB10;
		else if (device.swapchainSupport.swapchainA2BGR10Unorm)
			currentPipelineSettings.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_A2BGR10;
		else if (device.swapchainSupport.swapchainRGBA8Unorm)
			currentPipelineSettings.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_RGBA8;
		else if (device.swapchainSupport.swapchainBGRA8Unorm)
			currentPipelineSettings.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_BGRA8;
		else
			currentPipelineSettings.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_ABGR8;
	}

	{
		if (device.depthStencilSupport.D32Float)
			currentPipelineSettings.depthFormat = JJ2DE::DepthFormat::DEPTH_FORMAT_D32;
		else
			currentPipelineSettings.depthFormat = JJ2DE::DepthFormat::DEPTH_FORMAT_D32_S8;
	}

	ret.preInitializedPipelineSettings.push_back(currentPipelineSettings);
	ret.currentPipelineSettings = 0;

	return ret;
}
