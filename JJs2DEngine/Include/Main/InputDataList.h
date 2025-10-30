#pragma once

namespace JJs2DEngine
{
	class InputDataListInternal;
	class KeyPressData;

	struct SpecialKeysDataList;

	class InputDataList
	{
	public:
		InputDataList(InputDataListInternal& ref);
		~InputDataList();

		InputDataList& operator=(const InputDataList& rhs) noexcept = delete;

		const KeyPressData& GetKeyPressData(size_t scanCode) const;

		void ClearKeyPressesLists();

	private:
		InputDataListInternal& _internal;
	};
}
