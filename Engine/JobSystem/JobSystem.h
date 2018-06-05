#pragma once

#include <Windows.h>
#include <cassert>
#include "JobQueue.h"
#include "..\Logging\ConsolePrint.h"

namespace Engine
{
	namespace JobSystem
	{
		struct JobRunnerData
		{
			HANDLE				m_ThreadHandle;
			DWORD				m_ThreadID;
		};

		struct JobQueueData
		{
			SharedJobQueue		m_SharedQueue;
			std::vector<JobRunnerData *>	m_Runners;
		};

		extern std::map<std::string, JobQueueData*>* m_allQueues;
		class IJob;
		void Initialize();
		void CreateQueue(std::string i_Name, unsigned int i_numRunners);
		void AddRunner(std::string & i_QueueName);
		void RunJob(IJob * i_pJob, const std::string& i_QueueName);
		void Shutdown();
		DWORD WINAPI JobRunner(void * i_pJobRunnerInput);
	}
}