#pragma once
#include "Timer.h"

namespace Engine
{
	namespace Timer
	{
		float g_elapsedTime;
		LARGE_INTEGER g_defaultCPUFrequency, g_currentCPUTime, g_previousCPUTime;
		void Initialize()
		{
#ifdef _DEBUG
			ConsolePrint_Windows("Timer Init", __LINE__, __FILE__, "Timer");
#endif // _DEBUG
			QueryPerformanceFrequency(&g_defaultCPUFrequency);
			QueryPerformanceCounter(&g_previousCPUTime);
		}

		long GetCurrentCPUTickCounter()
		{
			LARGE_INTEGER i_Li;
			QueryPerformanceCounter(&i_Li);
			return (long)i_Li.QuadPart;
		}

		long GetCPUFrequency_ms()
		{
			LARGE_INTEGER i_Lf;
			QueryPerformanceFrequency(&i_Lf);
			return (long)i_Lf.QuadPart;
		}

		float GetTimeDifference_ms(long i_InputTime)
		{
			LARGE_INTEGER currentTick;
			QueryPerformanceCounter(&currentTick);
			auto ticksElapsed = currentTick.QuadPart - g_previousCPUTime.QuadPart;
			g_previousCPUTime = currentTick;
			return ((float)ticksElapsed / (float)(GetCPUFrequency_ms()) * (float)1000);

		}

		void CalculateTimeSinceLastCall()
		{
			QueryPerformanceCounter(&g_currentCPUTime);
			g_elapsedTime = ((float)(g_currentCPUTime.QuadPart - g_previousCPUTime.QuadPart) / (float)(GetCPUFrequency_ms()) * (float)1000);;
			g_previousCPUTime = g_currentCPUTime;
		}

		void Update()
		{
			CalculateTimeSinceLastCall();
		}

		void Shutdown() {}
	}
}