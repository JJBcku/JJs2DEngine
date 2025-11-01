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

	const std::vector<InputEvent>& InputDataList::GetEventList() const
	{
		return _internal.GetEventList();
	}

	void InputDataList::ClearEventList()
	{
		_internal.ClearEventList();
	}

	bool InputDataList::FocusWasLost() const
	{
		return _internal.FocusWasLost();
	}

	void InputDataList::ResetFocusLost()
	{
		_internal.ResetFocusLost();
	}

}