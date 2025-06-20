#pragma once

#include "VSCommandPoolMainListDef.h"

namespace VulkanSimplified
{
	class CommandPoolMainListInternal;
}

namespace VulkanSimplified
{
	class CommandPoolQFGroupList;

	struct CommandPoolQFGroupListsInitialCapacities;

	class CommandPoolMainList
	{
	public:
		CommandPoolMainList(CommandPoolMainListInternal& ref);
		~CommandPoolMainList();

		CommandPoolMainList& operator=(const CommandPoolMainList&) noexcept = delete;

		IDObject<CommandPoolQFGroupPointer> AddQueueFamiliesPoolGroup(uint32_t queueFamily, const CommandPoolQFGroupListsInitialCapacities& initialCapacities,
			size_t addOnReserving = 0);

		CommandPoolQFGroupList GetQueueFamiliesPoolGroup(IDObject<CommandPoolQFGroupPointer> qfGroupID);

		const CommandPoolQFGroupList GetQueueFamiliesPoolGroup(IDObject<CommandPoolQFGroupPointer> qfGroupID) const;

	private:
		CommandPoolMainListInternal& _internal;
	};
}