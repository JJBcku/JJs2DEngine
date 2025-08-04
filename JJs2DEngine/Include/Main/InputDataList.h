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

		const SpecialKeysDataList& GetSpecialKeyList() const;

		void ResetKeyPresses();

	private:
		InputDataListInternal& _internal;
	};
}
