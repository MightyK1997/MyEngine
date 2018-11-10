#include "ControllerInput.h"
#include <cmath>
#include <map>
#include "Engine/Time/Time.h"
#include "Engine/Logging/Logging.h"
namespace
{
	uint8_t g_NumberOfConnectedControllers = 0;
	bool g_bIsCalledFirstTime = true;
	float g_elapsedTime;
	struct ControllerState
	{
		XINPUT_STATE state;
	};
	uint64_t g_PreviousTickCount;
	ControllerState g_Controllers[XUSER_MAX_COUNT];

	//For Function callback
	std::map<uint8_t, std::map < eae6320::UserInput::ControllerInput::ControllerKeyCodes, std::function<void()>>> g_FunctionLookupTable;

	//Threading Variables
	HANDLE g_UpdateThreadHandle = NULL;
	DWORD g_UpdateThreadID;
	bool g_IsThreadRunning = false;
}

//Check For new controllers
void CheckForNewControllers()
{
	DWORD dwResult;
	uint8_t temp = 0;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		dwResult = XInputGetState(i, &state);
		if (dwResult == ERROR_SUCCESS)
		{
			temp++;
		}
	}
	g_NumberOfConnectedControllers = temp;
}

void CallCallbackFunction(eae6320::UserInput::ControllerInput::ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber = 0)
{
	if (g_FunctionLookupTable[i_ControllerNumber][i_KeyCode])
	{
		g_FunctionLookupTable[i_ControllerNumber][i_KeyCode]();
		g_FunctionLookupTable[i_ControllerNumber].erase(i_KeyCode);
	}
}

bool eae6320::UserInput::ControllerInput::IsKeyPressed(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber)
{
	if ((i_ControllerNumber >= 0 && i_ControllerNumber < 4) && g_NumberOfConnectedControllers > 0)
	{
		XINPUT_STATE state = g_Controllers[i_ControllerNumber].state;
		if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER) || (i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
		{
			if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::LEFT_TRIGGER))
			{
				if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
				{
					CallCallbackFunction(i_KeyCode, i_ControllerNumber);
					return true;
				}
			}
			else if ((i_KeyCode == eae6320::UserInput::ControllerInput::ControllerKeyCodes::RIGHT_TRIGGER))
			{
				if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
				{
					CallCallbackFunction(i_KeyCode, i_ControllerNumber);
					return true;
				}
			}
		}
		else
		{
			if ((state.Gamepad.wButtons & i_KeyCode) != 0)
			{
				CallCallbackFunction(i_KeyCode, i_ControllerNumber);
				return true;
			}
		}
	}
	return false;
}

float eae6320::UserInput::ControllerInput::GetTriggerDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber)
{
	if (i_ControllerNumber >= 0 && i_ControllerNumber < 4)
	{
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
	}
	return 0;
}

float eae6320::UserInput::ControllerInput::GetNormalizedTriggerDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber)
{

	if (i_ControllerNumber >= 0 && i_ControllerNumber < 4)
	{
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

	return 0;
}

eae6320::Math::sVector eae6320::UserInput::ControllerInput::GetStickDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber)
{
	if (i_ControllerNumber >= 0 && i_ControllerNumber < 4)
	{
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

	return eae6320::Math::sVector();
}

eae6320::Math::sVector eae6320::UserInput::ControllerInput::GetNormalizedStickDeflection(ControllerKeyCodes i_KeyCode, uint8_t i_ControllerNumber)
{
	if (i_ControllerNumber >= 0 && i_ControllerNumber < 4)
	{
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

	return eae6320::Math::sVector();
}

void eae6320::UserInput::ControllerInput::SetVibrationEffects(uint16_t i_LowFrequencySpeed, uint16_t i_HighFrequencySpeed, uint8_t i_ControllerNumber)
{
	if ((i_ControllerNumber >= 0 && i_ControllerNumber < 4) && g_NumberOfConnectedControllers > 0)
	{
		XINPUT_VIBRATION vib;
		ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));
		vib.wLeftMotorSpeed = i_LowFrequencySpeed;
		vib.wRightMotorSpeed = i_HighFrequencySpeed;
		XInputSetState(i_ControllerNumber, &vib);
	}
}

void eae6320::UserInput::ControllerInput::SetVibrationEffects(uint16_t i_Speed, uint8_t i_ControllerNumber)
{
	if ((i_ControllerNumber >= 0 && i_ControllerNumber < 4) && g_NumberOfConnectedControllers > 0)
	{
		XINPUT_VIBRATION vib;
		ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));
		vib.wLeftMotorSpeed = i_Speed;
		vib.wRightMotorSpeed = i_Speed;
		XInputSetState(i_ControllerNumber, &vib);
	}
}

void eae6320::UserInput::ControllerInput::RegisterFunctionForCallback(ControllerKeyCodes i_KeyCode, std::function<void()> i_CallbackFunction, uint8_t i_ControllerNumber)
{
	std::map < eae6320::UserInput::ControllerInput::ControllerKeyCodes, std::function<void()>> temp;
	temp[i_KeyCode] = i_CallbackFunction;
	g_FunctionLookupTable[i_ControllerNumber] = temp;
}

uint8_t eae6320::UserInput::ControllerInput::GetNumberOfConnectedControllers()
{
	return g_NumberOfConnectedControllers;
}

eae6320::cResult eae6320::UserInput::ControllerInput::Initialize()
{
	eae6320::cResult result = Results::Success;
	if ((!g_UpdateThreadHandle) && !g_IsThreadRunning)
	{
		g_UpdateThreadHandle = CreateThread(NULL, 0, Update, NULL, 0, &g_UpdateThreadID);
		// eae6320::Logging::OutputMessage(reinterpret_cast<char*>(g_UpdateThreadID));
		if (g_UpdateThreadHandle)
		{
			g_IsThreadRunning = true;
		}
		else
		{
			result = Results::Failure;
		}
	}
	else
	{
		result = Results::Failure;
	}
	return result;
}

DWORD __stdcall eae6320::UserInput::ControllerInput::Update(LPVOID i_InParameter)
{
	eae6320::Logging::OutputMessage("Executing Thread");
	DWORD dwResult = 0;
	while (g_IsThreadRunning)
	{
		if (g_bIsCalledFirstTime || (g_elapsedTime >= 5))
		{
			if (g_bIsCalledFirstTime) { g_bIsCalledFirstTime = false; }
			CheckForNewControllers();
			g_PreviousTickCount = eae6320::Time::GetCurrentSystemTimeTickCount();
			g_elapsedTime = 0.f;
		}
		if (g_NumberOfConnectedControllers == 0) { return eae6320::Results::Failure; }
		g_elapsedTime += static_cast<float>(eae6320::Time::ConvertTicksToSeconds(eae6320::Time::GetCurrentSystemTimeTickCount() - g_PreviousTickCount));
		g_PreviousTickCount = eae6320::Time::GetCurrentSystemTimeTickCount();
		for (DWORD i = 0; i < g_NumberOfConnectedControllers; i++)
		{
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));

			dwResult = XInputGetState(i, &state);
			if (dwResult == ERROR_SUCCESS)
			{
				g_Controllers[i].state = state;
			}
		}

		for (uint8_t i = 0; i < 4; i++)
		{
			auto temp = g_FunctionLookupTable[i];
			for (auto& x : temp)
			{
				IsKeyPressed(x.first, i);
			}
		}
	}
	return dwResult;
}

eae6320::cResult eae6320::UserInput::ControllerInput::CleanUp()
{
	g_IsThreadRunning = false;
	/*DWORD exitCode;
	GetExitCodeThread(g_UpdateThreadHandle, &exitCode);
	while (exitCode == STILL_ACTIVE)
	{
		return Results::Failure;
	}*/
	return Results::Success;
}
