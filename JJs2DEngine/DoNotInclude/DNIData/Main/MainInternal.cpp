#include "MainDNIpch.h"
#include "MainInternal.h"

#include "../../../Include/Main/MainInitializationData.h"

#include <VulkanSimplified/VSMain/VSMainInitData.h>

namespace JJs2DEngine
{
	MainInternal::MainInternal(const MainInitializationData& initData)
	{
		VS::MainInitData VSInitData;

		VSMain = std::make_unique<VS::Main>(VSInitData);
	}

	MainInternal::~MainInternal()
	{
	}

}