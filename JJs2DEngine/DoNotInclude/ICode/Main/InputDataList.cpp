#include "MainIpch.h"
#include "../../../Include/Main/InputDataList.h"

#include "../../DNIData/Main/InputDataListInternal.h"

namespace JJs2DEngine
{
	InputDataList::InputDataList(InputDataListInternal& ref) : _internal(ref)
	{
	}

	InputDataList::~InputDataList()
	{
	}

	const KeyPressData& InputDataList::GetKeyPressData(size_t scanCode) const
	{
		return _internal.GetKeyPressData(scanCode);
	}

	void InputDataList::ClearKeyPressesLists()
	{
		_internal.ClearKeyPressesLists();
	}

}