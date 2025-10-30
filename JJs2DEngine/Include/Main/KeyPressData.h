#pragma once

#include <Miscellaneous/Bool64Def.h>

#include <stdint.h>
#include <optional>
#include <chrono>
#include <compare>

namespace JJs2DEngine
{
	struct KeyModifiers
	{
		Misc::Bool64Values lShiftPressed;
		Misc::Bool64Values rShiftPressed;

		Misc::Bool64Values lCtrlPressed;
		Misc::Bool64Values rCtrlPressed;

		Misc::Bool64Values lAltPressed;
		Misc::Bool64Values rAltPressed;

		Misc::Bool64Values lGuiPressed;
		Misc::Bool64Values rGuiPressed;

		Misc::Bool64Values NumPressed;
		Misc::Bool64Values CapsPressed;
		Misc::Bool64Values ModePressed;
		Misc::Bool64Values ScrollPressed;

		KeyModifiers();
		~KeyModifiers();

		std::strong_ordering operator<=>(const KeyModifiers& rhs) const noexcept = default;
		bool operator==(const KeyModifiers& rhs) const noexcept = default;
	};

	class KeyPressData
	{
	public:
		KeyPressData();
		~KeyPressData();

		KeyPressData(const KeyPressData& rhs) noexcept = default;
		KeyPressData(KeyPressData&& rhs) noexcept = default;

		KeyPressData& operator=(const KeyPressData& rhs) noexcept = default;
		KeyPressData& operator=(KeyPressData&& rhs) noexcept = default;

		std::strong_ordering operator<=>(const KeyPressData& rhs) const noexcept = default;
		bool operator==(const KeyPressData& rhs) const noexcept = default;

		void PressKey(uint16_t mod, std::chrono::high_resolution_clock::time_point pressTime);
		void ReleaseKey();

		const std::vector<KeyModifiers>& GetKeyPressList() const;
		void ClearKeyPressList();

		void OnFocusLost();

		std::optional<std::chrono::high_resolution_clock::time_point> GetCurrentKeyPressBegginingTime() const;
		std::optional<KeyModifiers> GetCurrentModifiers() const;

	private:
		std::vector<KeyModifiers> _keyPressList;
		std::optional<std::chrono::high_resolution_clock::time_point> _currentKeyPressBeginning;
		std::optional<KeyModifiers> _currentModifiers;

		KeyModifiers CompileModifiers(uint16_t mod) const;
		void AddKeyPress(KeyModifiers mod);
	};
}
