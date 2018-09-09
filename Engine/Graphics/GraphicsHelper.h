#pragma once

#include <Engine/Concurrency/cEvent.h>
#include "Graphics.h"

#ifdef EAE6320_PLATFORM_D3D 
#include <d3d11.h>
#include <dxgi.h>
#endif
namespace eae6320
{
	namespace Graphics
	{
		class GraphicsHelper
		{
		public:
			cResult Initialize(const sInitializationParameters& i_initializationParameters);
			void RenderFrame();
			cResult CleanUp();
			void SetBackBuffer(sBackBuffer i_backBuffer) { m_BackBuffer = i_backBuffer; }
#ifdef EAE6320_PLATFORM_D3D
			cResult InitializeViews(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight);
			void SetRenderTargetView(ID3D11RenderTargetView* i_inputRenderTargetView) { m_renderTargetView = i_inputRenderTargetView; }
			ID3D11RenderTargetView* GetRenderTargetView() { return m_renderTargetView; }
			void SetDepthStencilView(ID3D11DepthStencilView* i_inputDepthStencilView) { m_depthStencilView = i_inputDepthStencilView; }
			ID3D11DepthStencilView* GetDepthStencilView() { return m_depthStencilView; }
#endif // EAE6320_PLat			
		private:
#ifdef EAE6320_PLATFORM_D3D
			// Constant buffer object
			ID3D11RenderTargetView* m_renderTargetView = nullptr;
			// A depth/stencil view allows a texture to have depth rendered to it
			ID3D11DepthStencilView* m_depthStencilView = nullptr;
#endif
			sBackBuffer m_BackBuffer;
		};
	}
}