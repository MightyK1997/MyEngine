#include "Profiler.h"

namespace Engine
{
	namespace Profiler
	{
		Profiler* g_Profiler;
		void Initialize()
		{
#ifdef _DEBUG
			ConsolePrint_Windows("Profile init", __LINE__, __FILE__, "Profiler");
#endif // _DEBUG
			g_Profiler = new Profiler();
		}

		void Shutdown()
		{
#ifdef _DEBUG
			ConsolePrint_Windows("Profiler Shutdown", __LINE__, __FILE__, "Profiler");
#endif // _DEBUG
			delete(g_Profiler);
		}
	}
}