#include "ConsolePrint.h"
#include <cassert>
#include <stdarg.h>		// for va_<xxx>
#include <stdio.h>		// for vsprintf_s()
#include <Windows.h>	// for OutputDebugStringA(). Uggh.. this pulls in a lot of Windows specific stuff

namespace Engine
{
	void ConsolePrint_Windows(const char * i_fmt, ...)
	{
		assert(i_fmt);

		const size_t lenTemp = 2048;
		char strTemp[lenTemp] = "DEBUG: ";

		strcat_s(strTemp, i_fmt);
		//strcat_s(strTemp, "\n");

		const size_t lenOutput = lenTemp + 32;

		char strOutput[lenOutput];

		// define a variable argument list variable 
		va_list args;

		// initialize it. second parameter is variable immediately
		// preceding variable arguments
		va_start(args, i_fmt);

		int lineNumber = va_arg(args, int);
		char lineNumChar[sizeof(int)];
		sprintf_s(lineNumChar, "%d", lineNumber);
		const char * fileName = va_arg(args, const char*);
		const char * system = va_arg(args, const char*);

		strcat_s(strTemp, " ");
		strcat_s(strTemp, lineNumChar);
		strcat_s(strTemp, " ");
		strcat_s(strTemp, fileName);
		strcat_s(strTemp, " ");
		strcat_s(strTemp, system);
		strcat_s(strTemp, "\n");

		// (safely) print our formatted string to a char buffer
		vsprintf_s(strOutput, lenOutput, strTemp, args);

		va_end(args);

		OutputDebugStringA(strOutput);
	}

	void c_printf(const char* formatString, const char* debugLog, ...)
	{
		assert(formatString);
		assert(debugLog);

		char strTemp[5000] = "";
		strcat_s(strTemp, debugLog);
		strcat_s(strTemp, formatString);
		strcat_s(strTemp, "\n");

		char strOutput[5000];

		va_list args;
		va_start(args, debugLog);
		vsprintf_s(strOutput, 5000, strTemp, args);
		va_end(args);

		OutputDebugStringA(strOutput);
	}
}