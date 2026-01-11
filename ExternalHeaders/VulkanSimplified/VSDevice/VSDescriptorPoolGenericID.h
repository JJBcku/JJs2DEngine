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

		struct
		{
			DescriptorPoolIDType type;
			IDObject<AutoCleanupNIFDescriptorPool> ID;
		} NifID;

		struct
		{
			DescriptorPoolIDType type;
			IDObject<AutoCleanupIFDescriptorPool> ID;
		} IfID;

		DescriptorPoolGenericID() noexcept;
		DescriptorPoolGenericID(IDObject<AutoCleanupNIFDescriptorPool> ID) noexcept;
		DescriptorPoolGenericID(IDObject<AutoCleanupIFDescriptorPool> ID) noexcept;

		DescriptorPoolGenericID(const DescriptorPoolGenericID& rhs) noexcept;
		DescriptorPoolGenericID(DescriptorPoolGenericID&& rhs) noexcept;
		~DescriptorPoolGenericID();

		DescriptorPoolGenericID& operator=(const DescriptorPoolGenericID& rhs) noexcept;
		DescriptorPoolGenericID& operator=(DescriptorPoolGenericID&& rhs) noexcept;
	};
}