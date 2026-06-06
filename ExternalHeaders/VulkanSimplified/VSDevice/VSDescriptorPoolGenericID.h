#pragma once

#include <CustomLists/IDObject.h>

#include "VSDescriptorDataListsDef.h"

namespace VulkanSimplified
{
	enum class DescriptorPoolIDType : uint64_t
	{
		UNKNOWN = 0,
		NIF = 1,
		IF = 2,
	};

	union DescriptorPoolGenericID
	{
		DescriptorPoolIDType type;

		struct NifID
		{
			DescriptorPoolIDType type;
			IDObject<AutoCleanupNIFDescriptorPool> ID;

			NifID(const IDObject<AutoCleanupNIFDescriptorPool>& ID) noexcept;
			~NifID() = default;
		} NifID;

		struct IfID
		{
			DescriptorPoolIDType type;
			IDObject<AutoCleanupIFDescriptorPool> ID;

			IfID(const IDObject<AutoCleanupIFDescriptorPool>& ID) noexcept;
			~IfID() = default;
		} IfID;

		DescriptorPoolGenericID() noexcept;
		DescriptorPoolGenericID(const IDObject<AutoCleanupNIFDescriptorPool>& ID) noexcept;
		DescriptorPoolGenericID(const IDObject<AutoCleanupIFDescriptorPool>& ID) noexcept;

		DescriptorPoolGenericID(const DescriptorPoolGenericID& rhs) noexcept;
		DescriptorPoolGenericID(DescriptorPoolGenericID&& rhs) noexcept;
		~DescriptorPoolGenericID();

		DescriptorPoolGenericID& operator=(const DescriptorPoolGenericID& rhs) noexcept;
		DescriptorPoolGenericID& operator=(DescriptorPoolGenericID&& rhs) noexcept;
	};
}