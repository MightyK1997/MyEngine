#include "JobRunner.h"

#include <assert.h>

//#include "../Console/ConsolePrint.h"

//namespace Engine
//{
//	namespace JobSystem
//	{
//
//		DWORD WINAPI JobRunner(void * i_pThreadInput)
//		{
//			assert(i_pThreadInput);
//			assert(((JobRunnerInput *)i_pThreadInput)->i_pQueue);
//
//			JobRunnerInput * pInput = reinterpret_cast<JobRunnerInput *>(i_pThreadInput);
//			SharedJobQueue * pQueue = reinterpret_cast<SharedJobQueue *>(pInput->i_pQueue);
//
//			bool bDone = false;
//
//			do
//			{
//				IJob * pJob = pQueue->GetWhenAvailable();
//				if (pJob)
//				{
//					pJob->Run();
//					delete pJob;
//
//					bDone = pQueue->ShutdownRequested();
//				}
//				else
//					bDone = pQueue->ShutdownRequested();
//
//			} while (bDone == false);
//
//#ifdef _DEBUG
//			//DEBUG_PRINT("JobRunner \"%s\": Shutting down.\n", pInput->m_ThreadName.c_str());
//#endif
//			return 0;
//		}
//
//	} // namespace JobSystem
//} // namespace Engine
