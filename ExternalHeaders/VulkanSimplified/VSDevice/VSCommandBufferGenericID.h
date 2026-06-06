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

		struct NIRPrimaryID
		{
			CommandBufferIDType type;
			IDObject<std::unique_ptr<NIRCommandPoolInternal>> commandPoolID;
			IDObject<std::unique_ptr<PrimaryNIRCommandBufferInternal>> commandBufferID;

			NIRPrimaryID(const IDObject<std::unique_ptr<NIRCommandPoolInternal>>& commandPoolID,
				const IDObject<std::unique_ptr<PrimaryNIRCommandBufferInternal>>& commandBufferID) noexcept;
			~NIRPrimaryID() = default;
		} NIRPrimaryID;

		struct NIRSecondaryID
		{
			CommandBufferIDType type;
			IDObject<std::unique_ptr<NIRCommandPoolInternal>> commandPoolID;
			IDObject<std::unique_ptr<SecondaryNIRCommandBufferInternal>> commandBufferID;

			NIRSecondaryID(const IDObject<std::unique_ptr<NIRCommandPoolInternal>>& commandPoolID,
				const IDObject<std::unique_ptr<SecondaryNIRCommandBufferInternal>>& commandBufferID) noexcept;
			~NIRSecondaryID() = default;
		} NIRSecondaryID;

		struct IRPrimaryID
		{
			CommandBufferIDType type;
			IDObject<std::unique_ptr<IRCommandPoolInternal>> commandPoolID;
			IDObject<std::unique_ptr<PrimaryIRCommandBufferInternal>> commandBufferID;

			IRPrimaryID(const IDObject<std::unique_ptr<IRCommandPoolInternal>>& commandPoolID,
				const IDObject<std::unique_ptr<PrimaryIRCommandBufferInternal>>& commandBufferID) noexcept;
			~IRPrimaryID() = default;
		} IRPrimaryID;

		struct IRSecondaryID
		{
			CommandBufferIDType type;
			IDObject<std::unique_ptr<IRCommandPoolInternal>> commandPoolID;
			IDObject<std::unique_ptr<SecondaryIRCommandBufferInternal>> commandBufferID;

			IRSecondaryID(const IDObject<std::unique_ptr<IRCommandPoolInternal>>& commandPoolID,
				const IDObject<std::unique_ptr<SecondaryIRCommandBufferInternal>>& commandBufferID) noexcept;
			~IRSecondaryID() = default;
		} IRSecondaryID;

		CommandBufferGenericID() noexcept;
		CommandBufferGenericID(const IDObject<std::unique_ptr<NIRCommandPoolInternal>>& commandPoolID,
			const IDObject<std::unique_ptr<PrimaryNIRCommandBufferInternal>>& commandBufferID) noexcept;
		CommandBufferGenericID(const IDObject<std::unique_ptr<NIRCommandPoolInternal>>& commandPoolID,
			const IDObject<std::unique_ptr<SecondaryNIRCommandBufferInternal>>& commandBufferID) noexcept;
		CommandBufferGenericID(const IDObject<std::unique_ptr<IRCommandPoolInternal>>& commandPoolID,
			const IDObject<std::unique_ptr<PrimaryIRCommandBufferInternal>>& commandBufferID) noexcept;
		CommandBufferGenericID(const IDObject<std::unique_ptr<IRCommandPoolInternal>>& commandPoolID,
			const IDObject<std::unique_ptr<SecondaryIRCommandBufferInternal>>& commandBufferID) noexcept;
		CommandBufferGenericID(const CommandBufferGenericID& rhs) noexcept;
		CommandBufferGenericID(CommandBufferGenericID&& rhs) noexcept;
		~CommandBufferGenericID();

		CommandBufferGenericID& operator=(const CommandBufferGenericID& rhs) noexcept;
		CommandBufferGenericID& operator=(CommandBufferGenericID&& rhs) noexcept;
	};
}