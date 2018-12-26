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

bool eae6320::UserInput::IsKeyPressed( const uint_fast8_t i_keyCode )
{
	/*const auto keyState = GetAsyncKeyState(i_keyCode);
	const short isKeyDownMask = ~1;
	return ( keyState & isKeyDownMask ) != 0;*/

	return ((keyboardData.Flags == 0) && (keyboardData.VKey == i_keyCode));
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

eae6320::Math::sVector eae6320::UserInput::GetMouseScreenCoordinates()
{
	return Math::sVector(static_cast<float>(mouseData.lLastX), static_cast<float>(mouseData.lLastY), 0);
}
