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

	const SpecialKeysDataList& InputDataList::GetSpecialKeyList() const
	{
		return _internal.GetSpecialKeyList();
	}

	void InputDataList::ClearKeyPressesLists()
	{
		_internal.ClearKeyPressesLists();
	}

}