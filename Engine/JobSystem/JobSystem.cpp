#include "JobSystem.h"


namespace Engine
{
	namespace JobSystem
	{
		std::map<std::string, JobQueueData*>* m_allQueues;
		void Initialize()
		{
#ifdef _DEBUG
			ConsolePrint_Windows("Creating Job System Queues", __LINE__, __FILE__, "JOb System");
#endif // _DEBUG
			m_allQueues = new std::map<std::string, JobQueueData *>;
			CreateQueue("FileProcessing", 1);
		}

		void AddRunner(JobQueueData & i_QueueData)
		{
			size_t runners = i_QueueData.m_Runners.size();

			JobRunnerData * pNewRunner = new JobRunnerData;

			pNewRunner->m_ThreadHandle = CreateThread(NULL, 0, JobRunner, &i_QueueData.m_SharedQueue, CREATE_SUSPENDED, &pNewRunner->m_ThreadID);
			assert(pNewRunner->m_ThreadHandle != NULL);

			i_QueueData.m_Runners.push_back(pNewRunner);
			ResumeThread(pNewRunner->m_ThreadHandle);
		}

		void AddRunner(std::string& i_QueueName)
		{
			std::map<std::string, JobQueueData *>::iterator existing = m_allQueues->find(i_QueueName);
			assert(existing != m_allQueues->end());
			assert(existing->second);

			AddRunner(*existing->second);
		}

		void CreateQueue(std::string i_Name, unsigned int i_numRunners)
		{
			assert(i_Name != "");

			assert(m_allQueues->find(i_Name) == m_allQueues->end());

#ifdef _DEBUG
			ConsolePrint_Windows("Creating Job Queue", __LINE__, __FILE__, "Job System");
#endif // _DEBUG
			JobQueueData * pNewJobQueueData = new JobQueueData;

			m_allQueues->operator[](i_Name) = pNewJobQueueData;

			for (unsigned int i = 0; i < i_numRunners; i++)
				AddRunner(*pNewJobQueueData);
		}

		void RunJob(IJob * i_pJob, const std::string& i_QueueName)
		{
			assert(i_pJob);
			std::map<std::string, JobQueueData *>::iterator existing = m_allQueues->find(i_QueueName);
			assert(existing != m_allQueues->end());
			assert(existing->second);
#ifdef _DEBUG
			ConsolePrint_Windows("Adding Job", __LINE__, __FILE__, "Job System");
#endif // _DEBUG
			existing->second->m_SharedQueue.Add(i_pJob);
		}

		void Shutdown()
		{
#ifdef _DEBUG
			ConsolePrint_Windows("Job System Shutdown", __LINE__, __FILE__, "Job System");
#endif // _DEBUG
			IJob::Shutdown();

			std::vector<HANDLE>	AllThreads;

			std::map<std::string, JobQueueData *>::iterator iter = m_allQueues->begin();
			while (iter != m_allQueues->end())
			{

				if (iter->second)
				{
					const size_t count = iter->second->m_Runners.size();
					for (size_t i = 0; i < count; i++)
					{
						JobRunnerData * pRunner = iter->second->m_Runners[i];
						if (pRunner  && pRunner->m_ThreadHandle != NULL)
							AllThreads.push_back(pRunner->m_ThreadHandle);
					}

					iter->second->m_SharedQueue.Shutdown();
				}
				++iter;
			}

			DWORD result = WaitForMultipleObjects(static_cast<DWORD>(AllThreads.size()), &AllThreads[0], TRUE, INFINITE);
			assert(result == WAIT_OBJECT_0);

			iter = m_allQueues->begin();
			while (iter != m_allQueues->end())
			{
				if (iter->second)
				{
					const size_t count = iter->second->m_Runners.size();
					for (size_t i = 0; i < count; i++)
					{
						JobRunnerData * pRunner = iter->second->m_Runners[i];
						if (pRunner)
							delete pRunner;
					}

					delete iter->second;
				}

				++iter;
			}
			m_allQueues->clear();
			delete m_allQueues;
		}

		DWORD WINAPI JobRunner(void * i_pThreadInput)
		{
			assert(i_pThreadInput);
			SharedJobQueue * pQueue = static_cast<SharedJobQueue *>(i_pThreadInput);

			bool bDone = false;

			do
			{
				IJob * pJob = pQueue->GetWhenAvailable();
				if (pJob)
				{
#ifdef _DEBUG
					ConsolePrint_Windows("Job Run", __LINE__, __FILE__, "Job System");
#endif // _DEBUG
					pJob->Run();
					delete pJob;

					bDone = pQueue->ShutdownRequested();
				}
				else
					bDone = pQueue->ShutdownRequested();

			} while (bDone == false);
			return 0;
		}
	}
}