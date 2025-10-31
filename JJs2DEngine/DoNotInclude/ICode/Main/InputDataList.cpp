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

	const std::vector<KeyPressData>& InputDataList::GetEventList() const
	{
		return _internal.GetEventList();
	}

	void InputDataList::ClearEventList()
	{
		_internal.ClearEventList();
	}

	bool InputDataList::WasFocusLost() const
	{
		return _internal.WasFocusLost();
	}

	void InputDataList::ResetFocusLost()
	{
		_internal.ResetFocusLost();
	}

}