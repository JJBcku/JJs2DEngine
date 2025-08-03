#pragma once

namespace JJs2DEngine
{
	class InputDataListInternal;

	struct SpecialKeysDataList;

	class InputDataList
	{
	public:
		InputDataList(InputDataListInternal& ref);
		~InputDataList();

		InputDataList& operator=(const InputDataList& rhs) noexcept = delete;

		SpecialKeysDataList& GetSpecialKeyList();
		const SpecialKeysDataList& GetSpecialKeyList() const;

	private:
		InputDataListInternal& _internal;
	};
}
