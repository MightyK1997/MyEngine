#pragma once
#include  <Windows.h>

namespace Engine
{
	namespace Timer
	{
		class Timer
		{
		public:
			void CalculateTimeSinceLastCall();
			float GetTimeDifference(long i_InputTime);

			void Initialize();
			void Update();
			void ShutDown();

		private:
			LARGE_INTEGER m_CurrentCPUTime, m_PreviousCPUTime, m_CPUFrequency;
			__int64 m_ElapsedTime;
		};
	}
}