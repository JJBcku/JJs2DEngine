#pragma once

#include "VSSynchronizationDataListsDef.h"

#include <Miscellaneous/Bool64Def.h>
#include <CustomLists/IDObjectDef.h>

#include <stdint.h>
#include <optional>

namespace VulkanSimplified
{
	class WindowInternal;
}

namespace VulkanSimplified
{
	struct SwapchainCreationData;

	class Window
	{
	public:
		Window(WindowInternal& ref);
		~Window();

		void CreateSwapchain(const SwapchainCreationData& creationData, bool throwOnSwapchainExist);

		bool AcquireNextImage(uint64_t timeout, std::optional<IDObject<AutoCleanupSemaphore>> semaphoreID, std::optional<IDObject<AutoCleanupFence>> fenceID,
			uint32_t& returnIndex);

		size_t GetSwapchainImageAmount() const;

		bool IsHidden() const;
		bool IsMinimized() const;
		bool IsQuitingRequested() const;

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		void SetFullscreen(Misc::Bool64Values newFullscreenValue);

	private:
		WindowInternal& _internal;
	};
}