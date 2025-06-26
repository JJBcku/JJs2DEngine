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
	if (compared.maxVulkanVersion > currentBest.maxVulkanVersion)
		return true;

	if (compared.deviceLocalMemory > currentBest.deviceLocalMemory)
		return true;

	if (compared.queueSupport.noVideoCodingGraphicQueueFamily.has_value() && !currentBest.queueSupport.noVideoCodingGraphicQueueFamily.has_value())
		return true;

	if (compared.queueSupport.transferQueueFamily.has_value() && !currentBest.queueSupport.transferQueueFamily.has_value())
		return true;
	else if ((!compared.queueSupport.transferQueueFamily.has_value() && !currentBest.queueSupport.transferQueueFamily.has_value()) &&
		(compared.queueSupport.computeQueueFamily.has_value() && !currentBest.queueSupport.computeQueueFamily.has_value()))
		return true;

	if (compared.swapchainSupport.Support16BitSwapchain() && !currentBest.swapchainSupport.Support16BitSwapchain())
		return true;
	else if ((!compared.swapchainSupport.Support16BitSwapchain() && !currentBest.swapchainSupport.Support16BitSwapchain()) &&
		(compared.swapchainSupport.Support10BitSwapchain() && !currentBest.swapchainSupport.Support10BitSwapchain()))
		return true;

	if (compared.textureSupport.Support16BitUncompressedTextures() && !currentBest.textureSupport.Support16BitUncompressedTextures())
		return true;

	if (compared.depthStencilSupport.SupportFloatDepth() && !currentBest.depthStencilSupport.SupportFloatDepth())
		return true;

	return false;
}
