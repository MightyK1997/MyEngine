#pragma once

//#define ENABLE_PROFILING
//#define CONCAT_HELPER(left, right) left##right
//#define CONCAT(left, right) CONCAT_HELPER(left, right)
//
//#if defined( ENABLE_PROFILING )
//
//#define PROFILE_UNSCOPED(name)
//static Engine::Profiler::Accumulator CONCAT(__Accumulator, __LINE__)(); Engine::Profiler::ScopedTimer CONCAT(__Timer, __LINE__)(CONCAT(__Accumulator, __LINE__));
//
//#define PROFILE_SCOPE_BEGIN(name)   {
//static Engine::Profiler::Accumulator CONCAT(__Accumulator, __LINE__)(); Engine::Profiler::ScopedTimer CONCAT(__Timer, __LINE__)(CONCAT(__Accumulator, __LINE__));
//
//#define PROFILE_SCOPE_END()	   }
//
//#else // ENABLE_PROFILING not defined
//
//#define PROFILE_SCOPE_BEGIN(str)		__noop
//#define PROFILE_SCOPE_END		__noop
//#define PROFILE_UNSCOPED(str)		__noop
//
//#endif // ENABLE_PROFILING

#include "..\Engine\MemoryAllocator\AllocatingAllocators.h"
#include "..\Engine\Renderer\Renderer.h"
#include "..\Engine\Renderer\System\Input.h"
#include "..\Engine\Physics\Physics.h"
#include "..\Engine\Timer\Timer.h"
#include "..\Engine\Logging\ConsolePrint.h"
#include "..\Engine\GameData\Classes\ReadGameData.h"
#include "..\WindowsProject\Game\Game.h"
#include "..\Engine\Profiling\Profiler.h"

namespace Engine
{
	bool Initialize(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow);
	void Run();
	void Shutdown();
	MemoryAllocators::AllocatingAllocators* GetAllocator();
	void SetAllocator();
	void CleanUp();
};