#include "pch.h"
#include "CreateMainClass.h"

#include "MainDataCollection.h"

#include <MainInitializationData.h>
#include <Main.h>

#include <EngineVersion.h>
#include <DeviceData.h>

#include <DeviceSettings.h>

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

	data.main->CreateDevice(CreateDeviceSettings(deviceList[currentBest], currentBest));
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

	ret.windowWidth = 1080;
	ret.aspectRatio = JJ2DE::AspectRatio::ASPECT_RATIO_16_9;

	ret.framesInFlight = std::min(device.swapchainSupport.minFramesInFlight + 1, device.swapchainSupport.maxFramesInFlight);

	{
		if (device.swapchainSupport.swapchainRGB16Unorm)
			ret.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_RGB16;
		else if (device.swapchainSupport.swapchainRGBA16Unorm)
			ret.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_RGBA16;
		else if (device.swapchainSupport.swapchainA2RGB10Unorm)
			ret.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_A2RGB10;
		else if (device.swapchainSupport.swapchainA2BGR10Unorm)
			ret.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_A2BGR10;
		else if (device.swapchainSupport.swapchainRGB8Unorm)
			ret.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_RGB8;
		else if (device.swapchainSupport.swapchainBGR8Unorm)
			ret.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_BGR8;
		else if (device.swapchainSupport.swapchainRGBA8Unorm)
			ret.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_RGBA8;
		else if (device.swapchainSupport.swapchainBGRA8Unorm)
			ret.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_BGRA8;
		else
			ret.swapchainFormat = JJ2DE::SwapchainFormat::SWAPCHAIN_FORMAT_ABGR8;
	}

	{
		if (device.textureSupport.textureRGBA16UNORM)
			ret.textureFormat = JJ2DE::TextureFormat::TEXTURE_FORMAT_RGBA16;
		else if (device.textureSupport.textureRGBA8UNORM)
			ret.textureFormat = JJ2DE::TextureFormat::TEXTURE_FORMAT_RGBA8;
		else
			ret.textureFormat = JJ2DE::TextureFormat::TEXTURE_FORMAT_BGRA8;
	}

	{
		if (device.depthStencilSupport.D32Float)
			ret.depthFormat = JJ2DE::DepthFormat::DEPTH_FORMAT_D32;
		else
			ret.depthFormat = JJ2DE::DepthFormat::DEPTH_FORMAT_D32_S8;
	}

	return ret;
}
