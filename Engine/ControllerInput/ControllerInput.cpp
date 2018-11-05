#include "ControllerInput.h"

#include <cmath>
namespace
{
	uint8_t g_NumberOfConnectedControllers = 0;
}

bool eae6320::UserInput::ControllerInput::IsKeyPressed(ControllerKeyCodes i_KeyCode)
{
	return (g_Controllers[0].state.Gamepad.wButtons & i_KeyCode) != 0;
}

float eae6320::UserInput::ControllerInput::GetDeflection(ControllerKeyCodes i_KeyCode)
{
	XINPUT_STATE state = g_Controllers[0].state;
	if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
	{
		//Check for deadzones
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER))
		{
			float def = state.Gamepad.bLeftTrigger;

			if (def > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				return def;
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
				return def;
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

void eae6320::UserInput::ControllerInput::SetVibrationEffects(float i_LowFrequencySpeed, float i_HighFrequencySpeed)
{
	XINPUT_VIBRATION vib;
	ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));
	vib.wLeftMotorSpeed = i_LowFrequencySpeed;
	vib.wRightMotorSpeed = i_HighFrequencySpeed;
	XInputSetState(0, &vib);
}

void eae6320::UserInput::ControllerInput::SetVibrationEffects(float i_Speed)
{
	XINPUT_VIBRATION vib;
	ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));
	vib.wLeftMotorSpeed = i_Speed;
	vib.wRightMotorSpeed = i_Speed;
	XInputSetState(0, &vib);
}



void eae6320::UserInput::ControllerInput::Initialize()
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
			if (g_Controllers[i].state.dwPacketNumber != state.dwPacketNumber)
			{
				g_Controllers[i].state = state;
			}
		}
	}
}

void eae6320::UserInput::ControllerInput::Update()
{
	DWORD dwResult;
	for (DWORD i = 0; i < g_NumberOfConnectedControllers; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		dwResult = XInputGetState(i, &state);
		if (dwResult == ERROR_SUCCESS)
		{
			if (g_Controllers[i].state.dwPacketNumber != state.dwPacketNumber)
			{
				g_Controllers[i].state = state;
			}
		}
	}
}