#include "Timer.h"

namespace Engine
{
	namespace Timer
	{
		void Timer::Update()
		{
			QueryPerformanceCounter(&m_CurrentCPUTicks);
			m_ElapsedTime = (m_CurrentCPUTicks.QuadPart - m_PreviousCPUTime.QuadPart) / m_CPUFrequency.QuadPart;
			m_DeltaTime = m_PreviousCPUTime.QuadPart - m_CurrentCPUTicks.QuadPart;
			m_PreviousCPUTime = m_CurrentCPUTicks;
		}

		void Timer::Shutdown()
		{
		}

		void Timer::Initialize()
		{
			QueryPerformanceCounter(&m_PreviousCPUTime);
			QueryPerformanceFrequency(&m_CPUFrequency);
		}
	}
}