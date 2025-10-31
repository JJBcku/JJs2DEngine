#include "MainIpch.h"
#include "../../../Include/Main/KeyPressData.h"

#include <Miscellaneous/Bool64.h>
#include <VulkanSimplified/VSMain/EventHandler/SdlKeyboardEventsData.h>

namespace JJs2DEngine
{
	KeyPressData::KeyPressData()
	{
		scanCode = 0;
		keyCode = 0;
		keyMods = 0;

		keyPressed = false;
		keyRepeat = false;
	}

	KeyPressData::~KeyPressData()
	{
	}

}