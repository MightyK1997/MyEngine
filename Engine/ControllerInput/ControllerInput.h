#pragma once

#include <cstdint>
//#include "Engine/Math/sVector.h"
//Need to include windows.h before including XInput.h
#include <Windows.h>
#include "ExternalLibraries.h"
#include <Xinput.h>

#include "Engine/Results/Results.h"

namespace eae6320
{
	namespace UserInput
	{
		namespace ControllerInput
		{
			enum ControllerKeyCodes
			{
				DPAD_UP = 0x0001,
				DPAD_DOWN = 0x0002,
				DPAD_LEFT = 0x0004,
				DPAD_RIGHT = 0x0008,
				START = 0x0010,
				BACK = 0x0020,
				LEFT_THUMB = 0x0040,
				RIGHT_THUMB = 0x0080,
				LEFT_SHOULDER = 0x0100,
				RIGHT_SHOULDER = 0x0200,
				A = 0x1000,
				B = 0x2000,
				X = 0x4000,
				Y = 0x8000,
				LEFT_TRIGGER,
				RIGHT_TRIGGER,
				LEFT_STICK,
				RIGHT_STICK
			};

			bool IsKeyPressed(ControllerKeyCodes i_KeyCode);
			float GetDeflection(ControllerKeyCodes i_KeyCode);
			
			void SetVibrationEffects(uint16_t i_LowFrequencySpeed, uint16_t i_HighFrequencySpeed);
			void SetVibrationEffects(uint16_t i_Speed);

			uint8_t GetNumberOfConnectedControllers();

			eae6320::cResult Initialize();
			void Update();
		}
	}
}
