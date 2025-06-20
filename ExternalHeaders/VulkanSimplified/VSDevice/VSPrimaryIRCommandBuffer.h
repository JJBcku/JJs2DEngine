#pragma once

#include "VSPrimaryNIRCommandBuffer.h"

namespace VulkanSimplified
{
	class PrimaryIRCommandBufferInternal;
}

namespace VulkanSimplified
{
	class PrimaryIRCommandBuffer : public PrimaryNIRCommandBuffer
	{
	public:
		PrimaryIRCommandBuffer(PrimaryIRCommandBufferInternal& ref);
		~PrimaryIRCommandBuffer();

		PrimaryIRCommandBuffer& operator=(const PrimaryIRCommandBuffer& rhs) noexcept = delete;

		void ResetCommandBuffer(bool freeResources);

	private:
		PrimaryIRCommandBufferInternal& _internal;
	};
}