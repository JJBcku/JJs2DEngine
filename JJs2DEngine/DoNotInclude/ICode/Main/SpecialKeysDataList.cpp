#include "MainIpch.h"
#include "../../../Include/Main/SpecialKeysDataList.h"

#include <VulkanSimplified/VSMain/EventHandler/SdlKeyboardEventsData.h>

namespace JJs2DEngine
{

	SpecialKeysDataList::SpecialKeysDataList() : ESCkey(VS::SdlKeycode::SDLK_DATA_ESCAPE), lAltkey(VS::SdlKeycode::SDLK_DATA_LALT), lCrtlkey(VS::SdlKeycode::SDLK_DATA_LCTRL),
		lGuikey(VS::SdlKeycode::SDLK_DATA_LGUI), lShiftkey(VS::SdlKeycode::SDLK_DATA_LSHIFT), rAltkey(VS::SdlKeycode::SDLK_DATA_RALT), rCrtlkey(VS::SdlKeycode::SDLK_DATA_RCTRL),
		rGuikey(VS::SdlKeycode::SDLK_DATA_RGUI), rShiftkey(VS::SdlKeycode::SDLK_DATA_RSHIFT)
	{
	}

	SpecialKeysDataList::~SpecialKeysDataList()
	{
	}

	void SpecialKeysDataList::OnFocusLost()
	{
		ESCkey.ReleaseKey();

		lAltkey.ReleaseKey();
		lCrtlkey.ReleaseKey();
		lShiftkey.ReleaseKey();
		lGuikey.ReleaseKey();

		rAltkey.ReleaseKey();
		rCrtlkey.ReleaseKey();
		rShiftkey.ReleaseKey();
		rGuikey.ReleaseKey();
	}

}