#include "MainDNIpch.h"
#include "WindowDataInternal.h"

#include "../../../Include/Main/WindowInitializationData.h"

namespace JJs2DEngine
{
	WindowDataInternal::WindowDataInternal(const WindowInitializationData& initData, uint32_t framesInFlight) : stub(initData.windowWidth)
	{
	}

	WindowDataInternal::~WindowDataInternal()
	{
	}

}