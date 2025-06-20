#pragma once

#include <stdint.h>

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

		size_t GetSwapchainImageAmount() const;

		bool IsHidden() const;
		bool IsMinimized() const;
		bool IsQuitingRequested() const;

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

	private:
		WindowInternal& _internal;
	};
}