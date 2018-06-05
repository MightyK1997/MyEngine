#pragma once

#include <Windows.h>

#include "..\BasicTypes.h"

namespace Engine 
{
	namespace Renderer
	{
		extern KeyTypes returnVal;
		namespace Input
		{
			bool Initialize();
			void Service(HRAWINPUT i_Input);
			void Shutdown();
			void SetKeyCode(RAWINPUT*);
		} // namespace Input
	} // namespace Renderer
}

