#include "MainIpch.h"
#include "../../../Include/Main/WindowCreationData.h"

#include "../../../Include/Main/AspectRatio.h"

#include <Miscellaneous/Bool64.h>

namespace JJs2DEngine
{
	WindowCreationData::WindowCreationData()
	{
		windowTitle = "Unnamed";
		windowWidth = 0;
		aspectRatio = AspectRatio::UNSET;
		fullscreenWindow = Misc::BOOL64_FALSE;
	}
}