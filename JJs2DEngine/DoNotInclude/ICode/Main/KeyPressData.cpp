#include "MainIpch.h"
#include "../../../Include/Main/KeyPressData.h"

#include <Miscellaneous/Bool64.h>
#include <VulkanSimplified/VSMain/EventHandler/SdlKeyboardEventsData.h>

namespace JJs2DEngine
{
	KeyModifiers::KeyModifiers()
	{
		lShiftPressed = Misc::BOOL64_FALSE;
		rShiftPressed = Misc::BOOL64_FALSE;

		lCtrlPressed = Misc::BOOL64_FALSE;
		rCtrlPressed = Misc::BOOL64_FALSE;

		lAltPressed = Misc::BOOL64_FALSE;
		rAltPressed = Misc::BOOL64_FALSE;

		lGuiPressed = Misc::BOOL64_FALSE;
		rGuiPressed = Misc::BOOL64_FALSE;

		NumPressed = Misc::BOOL64_FALSE;
		CapsPressed = Misc::BOOL64_FALSE;
		ModePressed = Misc::BOOL64_FALSE;
		ScrollPressed = Misc::BOOL64_FALSE;
	}

	KeyModifiers::~KeyModifiers()
	{
	}

	KeyPressData::KeyPressData(uint64_t unicode)
	{
		_unicode = unicode;
		_keyPressList.reserve(0x8);
	}

	KeyPressData::~KeyPressData()
	{
	}

	void KeyPressData::PressKey(uint16_t mod, std::chrono::high_resolution_clock::time_point pressTime)
	{
		_currentModifiers = CompileModifiers(mod);

		if (!_currentKeyPressBeginning.has_value())
		{
			_currentKeyPressBeginning = pressTime;
		}

		AddKeyPress(_currentModifiers.value());
	}

	void KeyPressData::ReleaseKey()
	{
		if (_currentModifiers.has_value())
		{
			_currentKeyPressBeginning.reset();
			_currentModifiers.reset();
		}
	}

	const std::vector<KeyModifiers>& KeyPressData::GetKeyPressList() const
	{
		return _keyPressList;
	}

	void KeyPressData::ClearKeyPressList()
	{
		_keyPressList.clear();
	}

	std::optional<std::chrono::high_resolution_clock::time_point> KeyPressData::GetCurrentKeyPressBegginingTime() const
	{
		return _currentKeyPressBeginning;
	}

	std::optional<KeyModifiers> KeyPressData::GetCurrentModifiers() const
	{
		return _currentModifiers;
	}

	KeyModifiers KeyPressData::CompileModifiers(uint16_t mod) const
	{
		KeyModifiers ret;

		if ((mod & VS::KMOD_DATA_LSHIFT) == VS::KMOD_DATA_LSHIFT)
			ret.lShiftPressed = Misc::BOOL64_TRUE;

		if ((mod & VS::KMOD_DATA_RSHIFT) == VS::KMOD_DATA_RSHIFT)
			ret.rShiftPressed = Misc::BOOL64_TRUE;

		if ((mod & VS::KMOD_DATA_LCTRL) == VS::KMOD_DATA_LCTRL)
			ret.lCtrlPressed = Misc::BOOL64_TRUE;

		if ((mod & VS::KMOD_DATA_RCTRL) == VS::KMOD_DATA_RCTRL)
			ret.rCtrlPressed = Misc::BOOL64_TRUE;

		if ((mod & VS::KMOD_DATA_LALT) == VS::KMOD_DATA_LALT)
			ret.lAltPressed = Misc::BOOL64_TRUE;

		if ((mod & VS::KMOD_DATA_RALT) == VS::KMOD_DATA_RALT)
			ret.rAltPressed = Misc::BOOL64_TRUE;

		if ((mod & VS::KMOD_DATA_LGUI) == VS::KMOD_DATA_LGUI)
			ret.lGuiPressed = Misc::BOOL64_TRUE;

		if ((mod & VS::KMOD_DATA_RGUI) == VS::KMOD_DATA_RGUI)
			ret.rGuiPressed = Misc::BOOL64_TRUE;

		if ((mod & VS::KMOD_DATA_NUM) == VS::KMOD_DATA_NUM)
			ret.NumPressed = Misc::BOOL64_TRUE;

		if ((mod & VS::KMOD_DATA_CAPS) == VS::KMOD_DATA_CAPS)
			ret.CapsPressed = Misc::BOOL64_TRUE;

		if ((mod & VS::KMOD_DATA_MODE) == VS::KMOD_DATA_MODE)
			ret.ModePressed = Misc::BOOL64_TRUE;

		if ((mod & VS::KMOD_DATA_SCROLL) == VS::KMOD_DATA_SCROLL)
			ret.ScrollPressed = Misc::BOOL64_TRUE;

		return ret;
	}

	void KeyPressData::AddKeyPress(KeyModifiers mod)
	{
		if (_keyPressList.size() == _keyPressList.capacity())
		{
			_keyPressList.reserve(_keyPressList.capacity() << 1);
		}

		_keyPressList.push_back(mod);
	}

}