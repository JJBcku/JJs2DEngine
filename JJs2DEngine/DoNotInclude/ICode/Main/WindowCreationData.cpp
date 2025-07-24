#include "MainIpch.h"
#include "../../../Include/Main/WindowCreationData.h"

#include "../../../Include/Main/AspectRatio.h"

namespace JJs2DEngine
{
	WindowCreationData::WindowCreationData()
	{
		windowTitle = "Unnamed";
		windowWidth = 0;
		aspectRatio = AspectRatio::UNSET;
		fullscreenWindow = false;
	}
}