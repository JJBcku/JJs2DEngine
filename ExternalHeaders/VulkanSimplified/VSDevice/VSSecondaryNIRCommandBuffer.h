#pragma once

#include "VSCommandBufferBase.h"

#include <optional>

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

		void BeginRecording(CommandBufferUsage usage, std::optional<std::pair<IDObject<AutoCleanupRenderPass>, uint32_t>> renderpass,
			std::optional<IDObject<AutoCleanupFramebuffer>> framebuffer, bool continueRenderPass);

	private:
		SecondaryNIRCommandBufferInternal& _internal;
	};
}