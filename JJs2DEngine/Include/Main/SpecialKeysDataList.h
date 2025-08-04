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

		KeyPressData f1Key;
		KeyPressData f2Key;
		KeyPressData f3Key;
		KeyPressData f4Key;

		KeyPressData f5Key;
		KeyPressData f6Key;
		KeyPressData f7Key;
		KeyPressData f8Key;

		KeyPressData f9Key;
		KeyPressData f10Key;
		KeyPressData f11Key;
		KeyPressData f12Key;

		SpecialKeysDataList();
		~SpecialKeysDataList();

		void OnFocusLost();
	};
}
