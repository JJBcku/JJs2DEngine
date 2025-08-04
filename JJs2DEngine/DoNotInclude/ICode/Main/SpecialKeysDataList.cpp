#include "MainIpch.h"
#include "../../../Include/Main/SpecialKeysDataList.h"

#include <VulkanSimplified/VSMain/EventHandler/SdlKeyboardEventsData.h>

namespace JJs2DEngine
{

	SpecialKeysDataList::SpecialKeysDataList() : ESCkey(VS::SDLK_DATA_ESCAPE), lAltkey(VS::SDLK_DATA_LALT), lCrtlkey(VS::SDLK_DATA_LCTRL), lGuikey(VS::SDLK_DATA_LGUI),
		lShiftkey(VS::SDLK_DATA_LSHIFT), rAltkey(VS::SDLK_DATA_RALT), rCrtlkey(VS::SDLK_DATA_RCTRL), rGuikey(VS::SDLK_DATA_RGUI), rShiftkey(VS::SDLK_DATA_RSHIFT),
		f1Key(VS::SDLK_DATA_F1), f2Key(VS::SDLK_DATA_F2), f3Key(VS::SDLK_DATA_F3), f4Key(VS::SDLK_DATA_F4), f5Key(VS::SDLK_DATA_F5), f6Key(VS::SDLK_DATA_F6),
		f7Key(VS::SDLK_DATA_F7), f8Key(VS::SDLK_DATA_F8), f9Key(VS::SDLK_DATA_F9), f10Key(VS::SDLK_DATA_F10), f11Key(VS::SDLK_DATA_F11), f12Key(VS::SDLK_DATA_F12)
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

		f1Key.ReleaseKey();
		f2Key.ReleaseKey();
		f3Key.ReleaseKey();
		f4Key.ReleaseKey();

		f5Key.ReleaseKey();
		f6Key.ReleaseKey();
		f7Key.ReleaseKey();
		f8Key.ReleaseKey();

		f9Key.ReleaseKey();
		f10Key.ReleaseKey();
		f11Key.ReleaseKey();
		f12Key.ReleaseKey();
	}

}