#pragma once

#include "EventHandler/SdlEventHandlerInitData.h"
#include "../VSSharedData/VSSharedDataListsCapacities.h"

#include "../VSCommon/VSVersionData.h"

#include <string>

namespace VulkanSimplified
{
	struct MainInitData
	{
		SdlEventHandlerInitData eventHandlerData;
		SharedDataListsCapacities sharedDataCapabilities;

		MainInitData();
		~MainInitData();
	};
}
