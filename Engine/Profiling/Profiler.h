#pragma once

#include <map>
#include <string>
#include "..\Timer\Timer.h"

namespace Engine
{
	namespace Profiler
	{
		struct Accumulator
		{
			double m_Sum;
			uint64_t m_Count;
			//float m_Min, m_Max;

			Accumulator() :
				m_Sum(0.0), m_Count(0) {	}

			void operator+= (float i_Time)
			{
				m_Sum += i_Time;
				m_Count++;
			}

			float Average()
			{
				return static_cast<float>(m_Sum) / m_Count;
			}
		};

		class Profiler
		{
		public:
			Accumulator& RegisterAccumulator(std::string i_Name)
			{
				auto m_Accumulator = m_AllAccumulators.insert(make_pair(i_Name, Accumulator()));
				return m_Accumulator.first->second;
			}

			Accumulator& GetAccumulator(std::string i_Name)
			{
				return m_AllAccumulators.find(i_Name)->second;
			}
		private:
			std::map<std::string, Accumulator> m_AllAccumulators;
		};
		extern Profiler* g_Profiler;
		void Initialize();
		void Shutdown();

		class ScopedTimer
		{
		public:
			ScopedTimer(Accumulator& i_myAccumulator) : m_Start(Engine::Timer::GetCurrentCPUTickCounter()), m_pAccumulator(i_myAccumulator) {}
			~ScopedTimer()
			{
				m_pAccumulator += Engine::Timer::GetTimeDifference_ms(Engine::Timer::GetCurrentCPUTickCounter());
			}
		private:
			long m_Start;
			Accumulator& m_pAccumulator;
			std::string m_pScopeName;
		};
	}
}