#pragma once

#include <stdint.h>

namespace JJs2DEngine
{
	struct MainInitializationData;

	class MainInternal
	{
	public:
		MainInternal(const MainInitializationData& initData);
		~MainInternal();

	private:
		uint64_t _stump;
	};
}
