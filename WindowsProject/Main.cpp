#include <Windows.h>
#include <sstream>

#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

#include "..\Engine\EngineInit.h"
//#include "..\Engine\FeatureCheck.h"
#include "Game\Game.h"

using namespace Engine;
using namespace Renderer;
using namespace std;

extern bool MatrixUnitTest();

void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown)
{
#ifdef _DEBUG
	const size_t	lenBuffer = 65;
	char			Buffer[lenBuffer];

	sprintf_s(Buffer, lenBuffer, "VKey 0x%04x went %s\n", i_VKeyID, bWentDown ? "down" : "up");
	OutputDebugStringA(Buffer);
#endif // __DEBUG
}

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	//_crtBreakAlloc = 257;
#ifdef _DEBUG
	ConsolePrint_Windows("Initializing Engine",__LINE__, __FILE__, "MatrixUnitTest");
#endif // _DEBUG
	bool bSuccess = Engine::Initialize(i_hInstance, i_hPrevInstance, i_lpCmdLine, i_nCmdShow);
#ifdef _DEBUG
	ConsolePrint_Windows("MatrixUnitTestValue", MatrixUnitTest(), __FILE__, "MatrixUnitTest");
#endif // _DEBUG
	//ConsolePrint_Windows("MatrixUnitTestValue", InstructionSet::AVX512CD(), __FILE__, "MatrixUnitTest");
	if (bSuccess)
	{
#ifdef _DEBUG
		ConsolePrint_Windows("Initializing Game Values", __LINE__, __FILE__, "MatrixUnitTest");
#endif // _DEBUG
		Game::Initialize();
#ifdef _DEBUG
		ConsolePrint_Windows("Engine Run", __LINE__, __FILE__, "MatrixUnitTest");
#endif // _DEBUG
		Engine::Run();
#ifdef _DEBUG
		ConsolePrint_Windows("Game Shutdown", __LINE__, __FILE__, "MatrixUnitTest");
#endif // _DEBUG
		Game::Shutdown();
#ifdef _DEBUG
		ConsolePrint_Windows("Engine Shutdown", __LINE__, __FILE__, "MatrixUnitTest");
#endif // _DEBUG
		Engine::Shutdown();
	}
#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
}