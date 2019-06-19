#include "UserInput.h"

namespace
{
	RAWHID otherHIDData;
	RAWMOUSE mouseData;
	RAWKEYBOARD keyboardData;
}

namespace UserInput
{
	void StoreRawInputData(const LPBYTE& i_RawInput)
	{
		const auto actualInputData = reinterpret_cast<RAWINPUT*>(i_RawInput);
		switch (actualInputData->header.dwType)
		{
		case RIM_TYPEKEYBOARD:
			keyboardData = actualInputData->data.keyboard;
			break;
		case RIM_TYPEMOUSE:
			mouseData = actualInputData->data.mouse;
			break;
		default:
			otherHIDData = actualInputData->data.hid;
			break;
		}
	}

	bool IsKeyboardKeyPressed(const uint8_t& i_KeyCode)
	{
		return ((keyboardData.Flags == 0) && (keyboardData.VKey == i_KeyCode));
	}

	bool IsMouseKeyPressed(const uint8_t& i_MouseKeyCode)
	{
		return ((mouseData.usButtonFlags ^ i_MouseKeyCode) == 0);
	}
}