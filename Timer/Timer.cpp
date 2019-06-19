#include "Timer.h"

namespace
{
	LARGE_INTEGER m_CurrentCPUTime, m_PreviousCPUTime, m_CPUFrequency;
}

namespace Engine
{
	namespace Timer
	{
		void Update()
		{
			QueryPerformanceCounter(&m_CurrentCPUTime);
			deltaTime = static_cast<float>((m_CurrentCPUTime.QuadPart - m_PreviousCPUTime.QuadPart) / m_CPUFrequency.QuadPart);
			m_PreviousCPUTime = m_CurrentCPUTime;
		}

		void Initialize()
		{
			QueryPerformanceCounter(&m_PreviousCPUTime);
			QueryPerformanceFrequency(&m_CPUFrequency);
		}

		float GetTimeDifference(long i_InputTime)
		{
			QueryPerformanceCounter(&m_CurrentCPUTime);
			auto numberOfTicks = m_CurrentCPUTime.QuadPart - m_PreviousCPUTime.QuadPart;
			m_PreviousCPUTime = m_CurrentCPUTime;
			return static_cast<float>(numberOfTicks / (m_CPUFrequency.QuadPart * 1000));
		}
	}
}