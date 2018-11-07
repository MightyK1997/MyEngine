#include "ControllerInput.h"

#include <cmath>
namespace
{
	uint8_t g_NumberOfConnectedControllers = 0;
	float g_elapsedTime;

	struct ControllerState
	{
		XINPUT_STATE state;
	};
	ControllerState g_Controllers[XUSER_MAX_COUNT];
}

//Check For new controllers
void CheckForNewControllers()
{
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		dwResult = XInputGetState(i, &state);
		if (dwResult == ERROR_SUCCESS)
		{
			g_NumberOfConnectedControllers++;
		}
	}
}

bool eae6320::UserInput::ControllerInput::IsKeyPressed(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0)
{
	Update();
	XINPUT_STATE state = g_Controllers[i_ControllerNumber].state;
	if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
	{
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER))
		{
			return state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		}
		else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
		{
			return state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		}
	}
	else
	{
		return (state.Gamepad.wButtons & i_KeyCode) != 0;
	}
	return false;
}

float eae6320::UserInput::ControllerInput::GetTriggerDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0)
{
	Update();
	XINPUT_STATE state = g_Controllers[i_ControllerNumber].state;
	if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
	{
		//Check for deadzones
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER))
		{
			float def = state.Gamepad.bLeftTrigger;

			if (def > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				return def - XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
			}
			else
			{
				return 0;
			}
		}
		else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
		{
			float def = state.Gamepad.bRightTrigger;

			if (def > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				return def - XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
			}
			else
			{
				return 0;
			}
		}
	}
	else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK))
	{
		//Check for deadzones
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK))
		{
			float LX = state.Gamepad.sThumbLX;
			float LY = state.Gamepad.sThumbLY;

			//determine how far the controller is pushed
			float magnitude = sqrt(LX*LX + LY * LY);

			//determine the direction the controller is pushed
			float normalizedLX = LX / magnitude;
			float normalizedLY = LY / magnitude;

			float normalizedMagnitude = 0;

			//check if the controller is outside a circular dead zone
			if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				//clip the magnitude at its expected maximum value
				if (magnitude > 32767) magnitude = 32767;

				//adjust magnitude relative to the end of the dead zone
				magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

				//optionally normalize the magnitude with respect to its expected range
				//giving a magnitude value of 0.0 to 1.0
				normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			}
			else //if the controller is in the deadzone zero out the magnitude
			{
				magnitude = 0.0;
				normalizedMagnitude = 0.0;
			}
			return magnitude;
		}
		else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK))
		{
			float RX = state.Gamepad.sThumbRX;
			float RY = state.Gamepad.sThumbRY;

			//determine how far the controller is pushed
			float magnitude = sqrt(RX*RX + RY * RY);

			//determine the direction the controller is pushed
			float normalizedRX = RX / magnitude;
			float normalizedRY = RY / magnitude;

			float normalizedMagnitude = 0;

			//check if the controller is outside a circular dead zone
			if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				//clip the magnitude at its expected maximum value
				if (magnitude > 32767) magnitude = 32767;

				//adjust magnitude relative to the end of the dead zone
				magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

				//optionally normalize the magnitude with respect to its expected range
				//giving a magnitude value of 0.0 to 1.0
				normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			}
			else //if the controller is in the deadzone zero out the magnitude
			{
				magnitude = 0.0;
				normalizedMagnitude = 0.0;
			}
			return magnitude;
		}
	}
	return 0;
}

float eae6320::UserInput::ControllerInput::GetNormalizedTriggerDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0)
{
	Update();
	XINPUT_STATE state = g_Controllers[i_ControllerNumber].state;
	if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
	{
		//Check for deadzones
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER))
		{
			float def = state.Gamepad.bLeftTrigger;

			if (def > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				return (def - XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / (255 - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
			}
			else
			{
				return 0;
			}
		}
		else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
		{
			float def = state.Gamepad.bRightTrigger;

			if (def > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				return (def - XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / (255 - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
			}
			else
			{
				return 0;
			}
		}
	}
	else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK))
	{
		//Check for deadzones
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK))
		{
			float LX = state.Gamepad.sThumbLX;
			float LY = state.Gamepad.sThumbLY;

			//determine how far the controller is pushed
			float magnitude = sqrt(LX*LX + LY * LY);

			//determine the direction the controller is pushed
			float normalizedLX = LX / magnitude;
			float normalizedLY = LY / magnitude;

			float normalizedMagnitude = 0;

			//check if the controller is outside a circular dead zone
			if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				//clip the magnitude at its expected maximum value
				if (magnitude > 32767) magnitude = 32767;

				//adjust magnitude relative to the end of the dead zone
				magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

				//optionally normalize the magnitude with respect to its expected range
				//giving a magnitude value of 0.0 to 1.0
				normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			}
			else //if the controller is in the deadzone zero out the magnitude
			{
				magnitude = 0.0;
				normalizedMagnitude = 0.0;
			}
			return normalizedMagnitude;
		}
		else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK))
		{
			float RX = state.Gamepad.sThumbRX;
			float RY = state.Gamepad.sThumbRY;

			//determine how far the controller is pushed
			float magnitude = sqrt(RX*RX + RY * RY);

			//determine the direction the controller is pushed
			float normalizedRX = RX / magnitude;
			float normalizedRY = RY / magnitude;

			float normalizedMagnitude = 0;

			//check if the controller is outside a circular dead zone
			if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				//clip the magnitude at its expected maximum value
				if (magnitude > 32767) magnitude = 32767;

				//adjust magnitude relative to the end of the dead zone
				magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

				//optionally normalize the magnitude with respect to its expected range
				//giving a magnitude value of 0.0 to 1.0
				normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			}
			else //if the controller is in the deadzone zero out the magnitude
			{
				magnitude = 0.0;
				normalizedMagnitude = 0.0;
			}
			return normalizedMagnitude;
		}
	}
}

eae6320::Math::sVector eae6320::UserInput::ControllerInput::GetStickDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0)
{
	Update();
	XINPUT_STATE state = g_Controllers[i_ControllerNumber].state;
	if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK))
	{
		//Check for deadzones
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK))
		{
			float LX = state.Gamepad.sThumbLX;
			float LY = state.Gamepad.sThumbLY;

			//determine how far the controller is pushed
			float magnitude = sqrt(LX*LX + LY * LY);

			//determine the direction the controller is pushed
			float normalizedLX = LX / magnitude;
			float normalizedLY = LY / magnitude;

			float normalizedMagnitude = 0;

			//check if the controller is outside a circular dead zone
			if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				//clip the magnitude at its expected maximum value
				if (magnitude > 32767) magnitude = 32767;

				//adjust magnitude relative to the end of the dead zone
				magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

				//optionally normalize the magnitude with respect to its expected range
				//giving a magnitude value of 0.0 to 1.0
				normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			}
			else //if the controller is in the deadzone zero out the magnitude
			{
				magnitude = 0.0;
				normalizedMagnitude = 0.0;
			}
			return eae6320::Math::sVector(LX, LY, 0);
		}
		else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK))
		{
			float RX = state.Gamepad.sThumbRX;
			float RY = state.Gamepad.sThumbRY;

			//determine how far the controller is pushed
			float magnitude = sqrt(RX*RX + RY * RY);

			//determine the direction the controller is pushed
			float normalizedRX = RX / magnitude;
			float normalizedRY = RY / magnitude;

			float normalizedMagnitude = 0;

			//check if the controller is outside a circular dead zone
			if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				//clip the magnitude at its expected maximum value
				if (magnitude > 32767) magnitude = 32767;

				//adjust magnitude relative to the end of the dead zone
				magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

				//optionally normalize the magnitude with respect to its expected range
				//giving a magnitude value of 0.0 to 1.0
				normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			}
			else //if the controller is in the deadzone zero out the magnitude
			{
				magnitude = 0.0;
				normalizedMagnitude = 0.0;
			}
			return eae6320::Math::sVector(RX, RY, 0);
		}
	}
}

eae6320::Math::sVector eae6320::UserInput::ControllerInput::GetNormalizedStickDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0)
{
	Update();
	XINPUT_STATE state = g_Controllers[i_ControllerNumber].state;
	if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK))
	{
		//Check for deadzones
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_STICK))
		{
			float LX = state.Gamepad.sThumbLX;
			float LY = state.Gamepad.sThumbLY;

			//determine how far the controller is pushed
			float magnitude = sqrt(LX*LX + LY * LY);

			//determine the direction the controller is pushed
			float normalizedLX = LX / magnitude;
			float normalizedLY = LY / magnitude;
			return eae6320::Math::sVector(normalizedLX, normalizedLY, 0);
		}
		else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_STICK))
		{
			float RX = state.Gamepad.sThumbRX;
			float RY = state.Gamepad.sThumbRY;

			//determine how far the controller is pushed
			float magnitude = sqrt(RX*RX + RY * RY);

			//determine the direction the controller is pushed
			float normalizedRX = RX / magnitude;
			float normalizedRY = RY / magnitude;
			return eae6320::Math::sVector(normalizedRX, normalizedRY, 0);
		}
	}
}

void eae6320::UserInput::ControllerInput::SetVibrationEffects(uint16_t i_LowFrequencySpeed, uint16_t i_HighFrequencySpeed, uint8_t i_ControllerNumber = 0)
{
	XINPUT_VIBRATION vib;
	ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));
	vib.wLeftMotorSpeed = i_LowFrequencySpeed;
	vib.wRightMotorSpeed = i_HighFrequencySpeed;
	XInputSetState(i_ControllerNumber, &vib);
}

void eae6320::UserInput::ControllerInput::SetVibrationEffects(uint16_t i_Speed, uint8_t i_ControllerNumber = 0)
{
	XINPUT_VIBRATION vib;
	ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));
	vib.wLeftMotorSpeed = i_Speed;
	vib.wRightMotorSpeed = i_Speed;
	XInputSetState(i_ControllerNumber, &vib);
}




uint8_t eae6320::UserInput::ControllerInput::GetNumberOfConnectedControllers()
{
	return g_NumberOfConnectedControllers;
}

void eae6320::UserInput::ControllerInput::Update()
{
	DWORD dwResult;
	CheckForNewControllers();
	for (DWORD i = 0; i < g_NumberOfConnectedControllers; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		dwResult = XInputGetState(i, &state);
		if (dwResult == ERROR_SUCCESS)
		{
			//if (g_Controllers[i].state.dwPacketNumber != state.dwPacketNumber)
			//{
			g_Controllers[i].state = state;
			//}
		}
	}
}