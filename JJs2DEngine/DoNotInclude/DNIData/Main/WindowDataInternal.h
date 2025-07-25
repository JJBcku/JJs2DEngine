#pragma once

namespace JJs2DEngine
{
	struct WindowInitializationData;

	class WindowDataInternal
	{
	public:
		WindowDataInternal(const WindowInitializationData& initData, uint32_t framesInFlight);
		~WindowDataInternal();

	private:
		uint64_t stub;
	};
}
