#include "pch.h"
#include "CreateMainClass.h"

#include "MainDataCollection.h"

#include <MainInitializationData.h>
#include <Main.h>

#include <EngineVersion.h>
#include <DeviceData.h>

static bool CompareDevices(const JJ2DE::DeviceData& currentBest, const JJ2DE::DeviceData& compared);

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

	if (currentBest.depthStencilSupport.SupportFloatDepth() && !compared.depthStencilSupport.SupportFloatDepth())
		return false;

	return true;
}
