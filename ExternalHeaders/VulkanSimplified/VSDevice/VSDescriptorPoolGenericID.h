#pragma once

#include <CustomLists/IDObject.h>

#include "VSDescriptorDataListDef.h"

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

		DescriptorPoolGenericID();
		DescriptorPoolGenericID(IDObject<AutoCleanupNIFDescriptorPool> ID);
		DescriptorPoolGenericID(IDObject<AutoCleanupIFDescriptorPool> ID);
	};
}