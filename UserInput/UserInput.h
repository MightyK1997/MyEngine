#pragma once

#include "../Common/Includes.h"
#include <cstdint>

namespace UserInput
{
	namespace KeyCodes
	{
		// These values are what the Windows-specific function expects, for simplicity
		enum eKeyCodes
		{
			Left = 0x25,
			Up = 0x26,
			Right = 0x27,
			Down = 0x28,

			Space = 0x20,

			Escape = 0x1b,

			Shift = 0x10,
			Control = 0x11,
			Alt = 0x12,

			Tab = 0x09,
			CapsLock = 0x14,

			BackSpace = 0x08,
			Enter = 0x0d,
			Delete = 0x2e,

			PageUp = 0x21,
			PageDown = 0x22,
			End = 0x23,
			Home = 0x24,

			F1 = 0x70,
			F2 = 0x71,
			F3 = 0x72,
			F4 = 0x73,
			F5 = 0x74,
			F6 = 0x75,
			F7 = 0x76,
			F8 = 0x77,
			F9 = 0x78,
			F10 = 0x79,
			F11 = 0x7a,
			F12 = 0x7b,
		};

		enum eMouseKeyCodes
		{
			MouseLeft = 0x0001,
			MouseRight = 0x0004,
			MouseMiddle = 0x0010,
			MouseExtraButton1 = 0x0020,
			MouseExtraButton2 = 0x100
		};
	}

	void StoreRawInputData(const LPBYTE& i_RawInput);

	bool IsKeyboardKeyPressed(const uint8_t& i_KeyCode);

	bool IsMouseKeyPressed(const uint8_t& i_MouseKeyCode);
}
