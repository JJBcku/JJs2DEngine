#pragma once

#include "VSSecondaryNIRCommandBuffer.h"

namespace VulkanSimplified
{
	class SecondaryIRCommandBufferInternal;
}

namespace VulkanSimplified
{
	class SecondaryIRCommandBuffer : public SecondaryNIRCommandBuffer
	{
	public:
		SecondaryIRCommandBuffer(SecondaryIRCommandBufferInternal& ref);
		~SecondaryIRCommandBuffer();

		SecondaryIRCommandBuffer& operator=(const SecondaryIRCommandBuffer&) noexcept = delete;

		void ResetCommandBuffer(bool freeResources);

	private:
		SecondaryIRCommandBufferInternal& _internal;
	};
}