#pragma once

#include <stdint.h>
#include <memory>

#include <VulkanSimplified/VSMain/VSMain.h>

namespace JJs2DEngine
{
	struct MainInitializationData;

	class MainInternal
	{
	public:
		MainInternal(const MainInitializationData& initData);
		~MainInternal();

	private:
		std::unique_ptr<VS::Main> VSMain;

		void CreateInstance(const MainInitializationData& initData);
	};
}
