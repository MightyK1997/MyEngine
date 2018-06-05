#pragma once
#include <d3d11.h>

namespace Engine 
{
	namespace Renderer
	{
		inline bool Succeeded(HRESULT i_Result) { return i_Result >= 0; }
	} // namespace Renderer
}