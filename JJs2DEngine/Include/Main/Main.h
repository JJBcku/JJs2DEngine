#pragma once

#include <memory>

namespace JJs2DEngine
{
	struct MainInitializationData;
	class MainInternal;

	class Main
	{
	public:
		Main(const MainInitializationData& initData);
		~Main();

	private:
		std::unique_ptr<MainInternal> _internal;
	};
}