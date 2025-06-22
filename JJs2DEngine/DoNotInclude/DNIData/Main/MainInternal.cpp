#include "MainDNIpch.h"
#include "MainInternal.h"

#include "../../../Include/Main/MainInitializationData.h"

#include "../../../Include/Common/EngineVersion.h"

#include "../Common/VersionDataInternal.h"

#include <VulkanSimplified/VSMain/VSMainInitData.h>
#include <VulkanSimplified/VSMain/VSInstanceExtensionPacksList.h>
#include <VulkanSimplified/VSMain/VSInstanceLayerPacksList.h>
#include <VulkanSimplified/VSInstance/VSInstanceCreationData.h>

namespace JJs2DEngine
{
	MainInternal::MainInternal(const MainInitializationData& initData)
	{
		VS::MainInitData VSInitData;

		VSMain = std::make_unique<VS::Main>(VSInitData);

		CreateInstance(initData);
	}

	MainInternal::~MainInternal()
	{
	}

	void MainInternal::CreateInstance(const MainInitializationData& initData)
	{
		auto availableExtensionPacks = VSMain->GetAvailableInstanceExtensionPacks();
		auto availableLayerPacks = VSMain->GetAvailableInstanceLayerPacks();

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

		JJs2DEngine::VersionData maxVulkanVersion = GetVersionData(VSMain->GetMaxAvailableVulkanVersion());

		if (initData.minVulkanVersion > maxVulkanVersion)
			throw std::runtime_error("MainInternal::CreateInstance Error: Highest available Vulkan version is below minimal requirements!");

		instanceInit.usedVulkanApiVersion = GetVSVersionData(std::min(initData.maxVulkanVersion, maxVulkanVersion));
		instanceInit.enabledExtensionPacks.sdlRequiredExtensions = true;

#if defined(_DEBUG) || defined(DEBUG) || defined(DEBUG_UTILS)
		instanceInit.enabledExtensionPacks.debugUtils = true;

		instanceInit.enabledLayerPacks.debugUtils = true;
#endif

		VSMain->CreateInstance(instanceInit);
	}

}