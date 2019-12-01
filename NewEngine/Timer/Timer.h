#pragma once
#include  <Windows.h>
#include <cstdint>

#include "../BaseEngineClass.h"

namespace Engine
{
	namespace Timer
	{
		class Timer : Engine::BaseClass
		{
		public:
			//Base Class functions
			virtual void Initialize() override;
			virtual void Update() override;
			virtual void Shutdown() override;

			uint64_t DeltaTime() { return m_DeltaTime; }

		private:
			LARGE_INTEGER m_CurrentCPUTicks, m_PreviousCPUTime, m_CPUFrequency;
			uint64_t m_ElapsedTime;
			uint64_t m_DeltaTime;
		};
	}
}