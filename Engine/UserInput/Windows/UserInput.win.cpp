// Includes
//=========

#include "../UserInput.h"

// Interface
//==========

namespace
{
	RAWMOUSE mouseData;

	RAWKEYBOARD keyboardData;
}

void eae6320::UserInput::SetRawInputData(LPBYTE input)
{
	RAWINPUT* data = reinterpret_cast<RAWINPUT*>(input);
	if (data->header.dwType == RIM_TYPEKEYBOARD)
	{
		keyboardData = data->data.keyboard;
	}
	else if (data->header.dwType == RIM_TYPEMOUSE)
	{
		mouseData = data->data.mouse;
	}
}

bool eae6320::UserInput::IsKeyPressed( const uint_fast8_t i_keyCode )
{
	return ((keyboardData.Flags == 0) && (keyboardData.VKey == i_keyCode));
}

eae6320::Math::sVector eae6320::UserInput::GetMouseScreenCoordinates()
{
	return Math::sVector(static_cast<float>(mouseData.lLastX), static_cast<float>(mouseData.lLastY), 0);
}

bool eae6320::UserInput::IsMouseKeyDown(KeyCodes::eMouseKeyCodes i_KeyCode)
{
	return ((mouseData.usButtonFlags ^ i_KeyCode) == 0);
}
