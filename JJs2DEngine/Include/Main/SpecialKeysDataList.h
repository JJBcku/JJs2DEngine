#pragma once

#include "KeyPressData.h"

namespace JJs2DEngine
{
	struct SpecialKeysDataList
	{
		KeyPressData ESCkey;

		KeyPressData lAltkey;
		KeyPressData lCrtlkey;
		KeyPressData lGuikey;
		KeyPressData lShiftkey;

		KeyPressData rAltkey;
		KeyPressData rCrtlkey;
		KeyPressData rGuikey;
		KeyPressData rShiftkey;

		SpecialKeysDataList();
		~SpecialKeysDataList();

		void OnFocusLost();
	};
}
