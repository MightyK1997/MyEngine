#include "JobQueue.h"

namespace Engine
{
	namespace JobSystem
	{
			SharedJobQueue::SharedJobQueue() : m_ShutdownRequested(false)
			{
				InitializeConditionVariable(&m_WakeAndCheck);
				InitializeCriticalSection(&m_QueueAccess);
			}

			bool SharedJobQueue::Add(IJob * i_pJob)
			{
				assert(i_pJob);
				bool bAdded = false;

				EnterCriticalSection(&m_QueueAccess);
				if (m_ShutdownRequested == false)
				{
					m_Jobs.push(i_pJob);
					bAdded = true;
				}
				LeaveCriticalSection(&m_QueueAccess);

				if (bAdded)
					WakeConditionVariable(&m_WakeAndCheck);

				return bAdded;
			}

			IJob * SharedJobQueue::GetWhenAvailable()
			{
				EnterCriticalSection(&m_QueueAccess);

				if (m_Jobs.empty() && (m_ShutdownRequested == false))
				{
					BOOL result = SleepConditionVariableCS(&m_WakeAndCheck, &m_QueueAccess, INFINITE);
					assert(result != 0);

					if (m_ShutdownRequested == true)
					{
						LeaveCriticalSection(&m_QueueAccess);
						return nullptr;
					}
				}

				IJob * pJob = nullptr;

				if (!m_Jobs.empty())
				{
					pJob = m_Jobs.front();
					m_Jobs.pop();
				}

				LeaveCriticalSection(&m_QueueAccess);
				return pJob;
			}

			void SharedJobQueue::Shutdown()
			{
				EnterCriticalSection(&m_QueueAccess);
				m_ShutdownRequested = true;
				LeaveCriticalSection(&m_QueueAccess);

				WakeAllConditionVariable(&m_WakeAndCheck);
			}

			bool SharedJobQueue::ShutdownRequested() const
			{
				return m_ShutdownRequested;
			}
		//Job* JobQueue::GetJobFromQueue()
		//{
		//	//EnterCriticalSection(&m_criticalSection);
		//	Job* pJobPointer;

		//	if (m_Jobs.empty())
		//	{
		//		SleepConditionVariableCS(&m_accessVariable, &m_criticalSection, INFINITE);
		//	}

		//	if (!m_Jobs.empty())
		//	{
		//		pJobPointer = m_Jobs.front();
		//		m_Jobs.pop();
		//		//LeaveCriticalSection(&m_criticalSection);
		//		return pJobPointer;
		//	}
		//	else
		//		//LeaveCriticalSection(&m_criticalSection);
		//		return nullptr;
		//}
	}
}
