#pragma once

namespace JJs2DEngine
{
	class InputDataListInternal;
	struct KeyPressData;

	struct SpecialKeysDataList;

	class InputDataList
	{
	public:
		InputDataList(InputDataListInternal& ref);
		~InputDataList();

		InputDataList& operator=(const InputDataList& rhs) noexcept = delete;

		const std::vector<KeyPressData>& GetEventList() const;
		void ClearEventList();

		bool FocusWasLost() const;
		void ResetFocusLost();

	private:
		InputDataListInternal& _internal;
	};
}
