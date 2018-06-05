#pragma once
#include <Windows.h>
#include "..\Logging\ConsolePrint.h"

namespace Engine
{
	namespace Timer
	{

		//Engine Function Calls
		void Initialize();
		void Update();
		void Shutdown();

		//Abstraction from Engine
		void CalculateTimeSinceLastCall();

		//Extra Function Calls
		long GetCurrentCPUTickCounter();
		long GetCPUFrequency_ms();
		float GetTimeDifference_ms(long i_InputTime);

		//Variables

		extern LARGE_INTEGER g_currentCPUTime, g_previousCPUTime, g_defaultCPUFrequency;
		extern float g_elapsedTime;
	}
}