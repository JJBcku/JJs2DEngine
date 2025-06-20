#pragma once

namespace VulkanSimplified
{
	struct CommandPoolQFGroupListsInitialCapacities
	{
		size_t noIndividualResetCommandPoolListInitialReservation;
		size_t individualResetCommandPoolListInitialReservation;

		CommandPoolQFGroupListsInitialCapacities();
	};
}