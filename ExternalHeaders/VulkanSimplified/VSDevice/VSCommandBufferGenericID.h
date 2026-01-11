#pragma once

#include <CustomLists/IDObject.h>

#include "VSCommandPoolQFGroupListDef.h"
#include "VSNIRCommandPoolDef.h"
#include "VSIRCommandPoolDef.h"

#include <memory>
#include <stdint.h>

namespace VulkanSimplified
{
	enum class CommandBufferIDType : uint64_t
	{
		UNKNOWN = 0,
		NIR_PRIMARY = 0x1,
		NIR_SECONDARY = 0x2,
		IR_PRIMARY = 0x4,
		IR_SECONDARY = 0x8,
	};

	union CommandBufferGenericID
	{
		CommandBufferIDType type;

		struct
		{
			CommandBufferIDType type;
			IDObject<std::unique_ptr<NIRCommandPoolInternal>> commandPoolID;
			IDObject<std::unique_ptr<PrimaryNIRCommandBufferInternal>> commandBufferID;
		} NIRPrimaryID;

		struct
		{
			CommandBufferIDType type;
			IDObject<std::unique_ptr<NIRCommandPoolInternal>> commandPoolID;
			IDObject<std::unique_ptr<SecondaryNIRCommandBufferInternal>> commandBufferID;
		} NIRSecondaryID;

		struct
		{
			CommandBufferIDType type;
			IDObject<std::unique_ptr<IRCommandPoolInternal>> commandPoolID;
			IDObject<std::unique_ptr<PrimaryIRCommandBufferInternal>> commandBufferID;
		} IRPrimaryID;

		struct
		{
			CommandBufferIDType type;
			IDObject<std::unique_ptr<IRCommandPoolInternal>> commandPoolID;
			IDObject<std::unique_ptr<SecondaryIRCommandBufferInternal>> commandBufferID;
		} IRSecondaryID;

		CommandBufferGenericID() noexcept;
		CommandBufferGenericID(const CommandBufferGenericID& rhs) noexcept;
		CommandBufferGenericID(CommandBufferGenericID&& rhs) noexcept;
		~CommandBufferGenericID();

		CommandBufferGenericID& operator=(const CommandBufferGenericID& rhs) noexcept;
		CommandBufferGenericID& operator=(CommandBufferGenericID&& rhs) noexcept;
	};
}