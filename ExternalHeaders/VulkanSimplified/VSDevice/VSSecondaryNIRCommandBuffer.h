#pragma once

#include "VSCommandBufferBase.h"

namespace VulkanSimplified
{
	class SecondaryNIRCommandBufferInternal;
}

namespace VulkanSimplified
{
	class SecondaryNIRCommandBuffer : public CommandBufferBase
	{
	public:
		SecondaryNIRCommandBuffer(SecondaryNIRCommandBufferInternal& ref);
		~SecondaryNIRCommandBuffer();

		SecondaryNIRCommandBuffer& operator=(const SecondaryNIRCommandBuffer&) noexcept = delete;

	private:
		SecondaryNIRCommandBufferInternal& _internal;
	};
}