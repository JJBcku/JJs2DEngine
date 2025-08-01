#pragma once

#include "KeyPressData.h"

namespace JJs2DEngine
{
	struct SpecialKeysDataList
	{
		KeyPressData ESCkey;
		KeyPressData CTRLkey;
		KeyPressData ALTLkey;
		KeyPressData SHIFTLkey;

		SpecialKeysDataList();
		~SpecialKeysDataList();
	};
}
