#pragma once

namespace JJ2DE
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
