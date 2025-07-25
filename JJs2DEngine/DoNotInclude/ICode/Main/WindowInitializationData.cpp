#include "MainIpch.h"
#include "../../../Include/Main/WindowInitializationData.h"

#include "../../../Include/Main/AspectRatio.h"

#include <Miscellaneous/Bool64.h>

namespace JJs2DEngine
{
	WindowInitializationData::WindowInitializationData()
	{
		windowTitle = "Unnamed";
		windowWidth = 0;
		aspectRatio = AspectRatio::UNSET;
		fullscreenWindow = Misc::BOOL64_FALSE;
	}
}