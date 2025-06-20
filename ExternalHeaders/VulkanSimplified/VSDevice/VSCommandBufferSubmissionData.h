#pragma once

#include "../VSCommon/VSPipelineStageFlags.h"

#include "VSCommandBufferGenericID.h"

#include <CustomLists/IDObjectDef.h>
#include <vector>
#include <utility>

namespace VulkanSimplified
{
	class AutoCleanupSemaphore;
}

namespace VulkanSimplified
{
	struct CommandBufferSubmissionData
	{
		std::vector<std::pair<IDObject<AutoCleanupSemaphore>, PipelineStageFlags>> waitSemaphores;
		std::vector<CommandBufferGenericID> commandBufferIDs;
		std::vector<IDObject<AutoCleanupSemaphore>> signalSemaphores;

		CommandBufferSubmissionData() = default;
	};
}