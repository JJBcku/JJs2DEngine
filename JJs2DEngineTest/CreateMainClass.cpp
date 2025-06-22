#include "pch.h"
#include "CreateMainClass.h"

#include "MainDataCollection.h"

#include <MainInitializationData.h>
#include <Main.h>

#include <EngineVersion.h>

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
}
