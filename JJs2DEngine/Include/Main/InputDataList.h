#pragma once

namespace JJs2DEngine
{
	class InputDataListInternal;

	class InputDataList
	{
	public:
		InputDataList(InputDataListInternal& ref);
		~InputDataList();

		InputDataList& operator=(const InputDataList& rhs) noexcept = delete;

	private:
		InputDataListInternal& _internal;
	};
}
