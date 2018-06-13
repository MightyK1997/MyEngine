#include "Log.h"
#include <string>
#include <stdarg.h>
#include <iostream>
#ifdef _WIN32 || _WIN64
#include <Windows.h>
#endif
#ifdef _gcc

#endif

namespace Engine
{
	namespace Debug
	{
		void Log(void* i_pString, const char* i_formatString, ...)
		{
			char strTemp[5000] = "";
			strcat_s(strTemp, static_cast<char*>(i_pString));
			if (i_formatString) strcat_s(strTemp, i_formatString);
			strcat_s(strTemp, "\n");

			char strOutput[5000];

			va_list args;
			va_start(args, i_formatString);
			vsprintf_s(strOutput, 5000, strTemp, args);
			va_end(args);

			OutputDebugStringA(strOutput);
			std::clog << strOutput;
		}
	}
}|