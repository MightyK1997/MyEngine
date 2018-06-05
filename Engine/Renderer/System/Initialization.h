#pragma once
#include <Windows.h>

namespace Engine
{
	namespace Renderer
	{
		bool Initialize(HINSTANCE i_hInstance, int i_CmdShow, const char * i_pWindowName, WORD i_IconID, unsigned int i_WindowWidth, unsigned int i_WindowHeight);
		void Shutdown();

	}// Namespace Renderer
} // namespace GLib

