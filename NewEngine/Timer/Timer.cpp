#include "Timer.h"

namespace Engine
{
	namespace Timer
	{
		void Timer::Update()
		{
			QueryPerformanceCounter(&m_CurrentCPUTime);
			m_ElapsedTime = (m_CurrentCPUTime.QuadPart - m_PreviousCPUTime.QuadPart) / m_CPUFrequency.QuadPart;
			m_PreviousCPUTime = m_CurrentCPUTime;
		}

		void Timer::ShutDown()
		{
		}

		void Timer::Initialize()
		{
			QueryPerformanceCounter(&m_PreviousCPUTime);
			QueryPerformanceFrequency(&m_CPUFrequency);
		}

		float Timer::GetTimeDifference(long i_InputTime)
		{
			QueryPerformanceCounter(&m_CurrentCPUTime);
			auto numberOfTicks = m_CurrentCPUTime.QuadPart - m_PreviousCPUTime.QuadPart;
			m_PreviousCPUTime = m_CurrentCPUTime;
			return static_cast<float>(numberOfTicks / (m_CPUFrequency.QuadPart * 1000));
		}
	}
}