#pragma once
#include "Windows.h"
#include "d3d11.h"
#include <cstdint>

namespace Graphics
{
	void Initialize(HWND i_Window, uint32_t width, uint32_t height);
}

struct DirectXValues
{
	HWND windowBeingRenderedTo = NULL;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	IDXGISwapChain* pSwapChain;


	static DirectXValues g_Context;

	HRESULT Initialize(HWND window, uint32_t width, uint32_t height);
	HRESULT CleanUp();


private:
	DirectXValues() = default;
};