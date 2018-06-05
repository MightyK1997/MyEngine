#include <assert.h>

#include <Windows.h>

#include "..\Renderer.h"

#include "Device.h"
#include "DXHelpers.h"
#include "Input.h"
#include "Window.h"

#include "..\..\Logging\ConsolePrint.h"
#include "..\Buffers\DrawResourceBuffer.h"

namespace Engine
{
	namespace Renderer {
		bool Initialize(HINSTANCE i_hInstance, int i_CmdShow, const char * i_pWindowName, WORD i_IconID, unsigned int i_WindowWidth, unsigned int i_WindowHeight)
		{
			g_hInstance = i_hInstance;

#ifdef  UNICODE
			const size_t		lenWindowName = 128;
			wchar_t				WindowName[lenWindowName];

			MultiByteToWideChar(CP_ACP, 0, (i_pWindowName) ? i_pWindowName : "GLib Render", -1, WindowName, lenWindowName);

			LPCTSTR pWindowName = WindowName;
#else
			LPCTSTR pWindowName = (i_pWindowName) ? i_pWindowName : "GLib Render";
#endif // UNICODE

			g_hWnd = _CreateWindow(WndProc, g_hInstance, i_IconID, pWindowName, i_WindowWidth, i_WindowHeight);
			if (g_hWnd == nullptr)
			{
				Engine::ConsolePrint_Windows("_CreateWindow() failed.\n", __LINE__, __FILE__, "Engine");
				return NULL;
			}

			HRESULT hResult = CreateDevice(g_hWnd);
			if (!Succeeded(hResult))
			{
				Engine::ConsolePrint_Windows("_CreateDevice() failed. HRESULT = %d.\n", hResult , __FILE__, "Engine");
				return false;
			}

			ShowWindow(g_hWnd, i_CmdShow);

			Input::Initialize();
			Sprites::Initialize();
			return true;
		}

		void Shutdown()
		{
			Input::Shutdown();
			Sprites::Shutdown();
			ReleaseDevice();
		}
	}
}// namespace GLib