#pragma once
#include <cassert>
#include <vector>
#include <queue>
#include <map>
#include "Job.h"

namespace Engine
{
	namespace JobSystem
	{
		class SharedJobQueue
		{
		public:
			SharedJobQueue();

			void				Shutdown();
			bool				ShutdownRequested() const;

			bool					Add(IJob * i_pJob);
			IJob *					GetWhenAvailable();
		private:
			std::queue<IJob *>		m_Jobs;

			CONDITION_VARIABLE		m_WakeAndCheck;
			CRITICAL_SECTION		m_QueueAccess;

			bool					m_ShutdownRequested;
		};
	}
}