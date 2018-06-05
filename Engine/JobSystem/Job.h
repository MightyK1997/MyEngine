#pragma once
#include <string>
#include <Windows.h>

namespace Engine
{
	namespace JobSystem
	{
		class IJob
		{
		public:
			IJob(std::string i_Name): m_Name(i_Name)
			{}

			virtual					~IJob() { }
			virtual void			Run() = 0;

			static void				Shutdown();
			static bool				ShutdownRequested();
		private:
			static CRITICAL_SECTION		m_Shutdown;
			static bool					m_bShutdown;
			std::string					m_Name;
		};

		inline bool IJob::ShutdownRequested()
		{
			return m_bShutdown;
		}

	}
}
