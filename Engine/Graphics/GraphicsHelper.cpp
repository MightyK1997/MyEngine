#include "GraphicsHelper.h"

// Includes
//=========
//
#include "cMesh.h"
#include "cEffect.h"

namespace
{
#ifdef EAE6320_PLATFORM_D3D
	// In Direct3D "views" are objects that allow a texture to be used a particular way:
// A render target view allows a texture to have color rendered to it
	ID3D11RenderTargetView* s_renderTargetView[2] = { nullptr, nullptr };
	// A depth/stencil view allows a texture to have depth rendered to its
	ID3D11DepthStencilView* s_depthStencilView = nullptr;
#endif
}

namespace eae6320
{
	namespace Graphics
	{

		cResult GraphicsHelper::CleanUp()
		{
			auto result = Results::Success;
#ifdef EAE6320_PLATFORM_D3D
			if (s_renderTargetView)
			{
				s_renderTargetView[0]->Release();
				s_renderTargetView[0] = nullptr;
			}
			if (s_depthStencilView)
			{
				s_depthStencilView->Release();
				s_depthStencilView = nullptr;
			}
#endif
			//delete s_Mesh;
			//delete s_Mesh2;
			//delete s_Effect;
			//delete s_Effect2;
			return result;
		}
		cResult GraphicsHelper::Initialize(const sInitializationParameters& i_initializationParameters)
		{
			auto result = Results::Success;
			// Initialize the views
#ifdef EAE6320_PLATFORM_D3D
			{
				if (!(result = InitializeViews(i_initializationParameters.resolutionWidth, i_initializationParameters.resolutionHeight)))
				{
					EAE6320_ASSERT(false);
					goto OnExit;
				}
			}
#endif
			goto OnExit;
		OnExit:

			return result;
		}
		void GraphicsHelper::SetRenderTargetView(sColor i_BackBufferColor)
		{
#ifdef EAE6320_PLATFORM_D3D
			auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
			EAE6320_ASSERT(direct3dImmediateContext);

			// Every frame an entirely new image will be created.
			// Before drawing anything, then, the previous image will be erased
			// by "clearing" the image buffer (filling it with a solid color)
			{
				EAE6320_ASSERT(m_renderTargetView);
				// Black is usually used
				/*constexpr*/ float clearColor[4] = { i_BackBufferColor.r, i_BackBufferColor.g, i_BackBufferColor.b, i_BackBufferColor.alpha };
				//float clearColor[4] = {1,1,1,1 };
				direct3dImmediateContext->ClearRenderTargetView(m_renderTargetView[0], clearColor);
			}
#elif EAE6320_PLATFORM_GL
			{
				// Black is usually used
				{
					glClearColor(i_BackBufferColor.r, i_BackBufferColor.g, i_BackBufferColor.b, i_BackBufferColor.alpha);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
				{
					constexpr GLbitfield clearColor = GL_COLOR_BUFFER_BIT;
					glClear(clearColor);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
			}
#endif
		}
		void GraphicsHelper::ClearDepthStencilView()
		{
#ifdef EAE6320_PLATFORM_D3D
			{
				EAE6320_ASSERT(m_depthStencilView);
				auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;

				constexpr float clearToFarDepth = 1.0f;
				constexpr uint8_t stencilValue = 0;	// Arbitrary if stencil isn't used
				direct3dImmediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, clearToFarDepth, stencilValue);
			}
#elif EAE6320_PLATFORM_GL
			{
				{
					glDepthMask(GL_TRUE);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
					constexpr GLclampd clearToFarDepth = 1.0;
					glClearDepth(clearToFarDepth);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
				{
					constexpr GLbitfield clearDepth = GL_DEPTH_BUFFER_BIT;
					glClear(clearDepth);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
			}
#endif
		}
		void GraphicsHelper::UpdateConstantBuffer(eae6320::Graphics::ConstantBufferFormats::sPerFrame i_ConstantBuffer)
		{
			// Update the per-frame constant buffer
#ifdef EAE6320_PLATFORM_D3D
			{
				// Copy the data from the system memory that the application owns to GPU memory
				s_constantBuffer_perFrame.Update(&i_ConstantBuffer);
			}
#elif EAE6320_PLATFORM_GL
			{
				// Copy the data from the system memory that the application owns to GPU memory
				s_constantBuffer_perFrame.Update(&i_ConstantBuffer);
			}
#endif
		}
		void GraphicsHelper::SwapChain()
		{
#ifdef EAE6320_PLATFORM_D3D
			{
				auto* const swapChain = sContext::g_context.swapChain;
				EAE6320_ASSERT(swapChain);
				constexpr unsigned int swapImmediately = 0;
				constexpr unsigned int presentNextFrame = 0;
				const auto result = swapChain->Present(swapImmediately, presentNextFrame);
				EAE6320_ASSERT(SUCCEEDED(result));
			}
#elif EAE6320_PLATFORM_GL
			{
				const auto deviceContext = sContext::g_context.deviceContext;
				EAE6320_ASSERT(deviceContext != NULL);

				const auto glResult = SwapBuffers(deviceContext);
				EAE6320_ASSERT(glResult != FALSE);
			}
#endif
		}
	}
#ifdef EAE6320_PLATFORM_D3D
	eae6320::cResult Graphics::GraphicsHelper::InitializeViews(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight)
	{
		auto result = eae6320::Results::Success;

		ID3D11Texture2D* backBuffer = nullptr;
		ID3D11Texture2D* depthBuffer = nullptr;
		ID3D11Texture2D* bufferLeft = nullptr;
		ID3D11Texture2D* bufferRight = nullptr;

		auto& g_context = eae6320::Graphics::sContext::g_context;
		auto* const direct3dDevice = g_context.direct3dDevice;
		EAE6320_ASSERT(direct3dDevice);
		auto* const direct3dImmediateContext = g_context.direct3dImmediateContext;
		EAE6320_ASSERT(direct3dImmediateContext);

		// Create a "render target view" of the back buffer
		// (the back buffer was already created by the call to D3D11CreateDeviceAndSwapChain(),
		// but a "view" of it is required to use as a "render target",
		// meaning a texture that the GPU can render to)
		{
			// Get the back buffer from the swap chain
			{
				constexpr unsigned int bufferIndex = 0;	// This must be 0 since the swap chain is discarded
				const auto d3dResult = g_context.swapChain->GetBuffer(bufferIndex, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
				if (FAILED(d3dResult))
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, "Couldn't get the back buffer from the swap chain (HRESULT %#010x)", d3dResult);
					eae6320::Logging::OutputError("Direct3D failed to get the back buffer from the swap chain (HRESULT %#010x)", d3dResult);
					goto OnExit;
				}
			}
			// Create the view
			{
				constexpr D3D11_RENDER_TARGET_VIEW_DESC* const accessAllSubResources = nullptr;
				const auto d3dResult = direct3dDevice->CreateRenderTargetView(backBuffer, accessAllSubResources, &s_renderTargetView[0]);
				if (FAILED(d3dResult))
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, "Couldn't create render target view (HRESULT %#010x)", d3dResult);
					eae6320::Logging::OutputError("Direct3D failed to create the render target view (HRESULT %#010x)", d3dResult);
					goto OnExit;
				}
			}
		}
		// Create a depth/stencil buffer and a view of it
		{
			// Unlike the back buffer no depth/stencil buffer exists until and unless it is explicitly created
			{
				D3D11_TEXTURE2D_DESC textureDescription{};
				{
					textureDescription.Width = i_resolutionWidth;
					textureDescription.Height = i_resolutionHeight;
					textureDescription.MipLevels = 1;	// A depth buffer has no MIP maps
					textureDescription.ArraySize = 1;
					textureDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 24 bits for depth and 8 bits for stencil
					{
						DXGI_SAMPLE_DESC& sampleDescription = textureDescription.SampleDesc;

						sampleDescription.Count = 1;	// No multisampling
						sampleDescription.Quality = 0;	// Doesn't matter when Count is 1
					}
					textureDescription.Usage = D3D11_USAGE_DEFAULT;	// Allows the GPU to write to it
					textureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
					textureDescription.CPUAccessFlags = 0;	// CPU doesn't need access
					textureDescription.MiscFlags = 0;
				}
				// The GPU renders to the depth/stencil buffer and so there is no initial data
				// (like there would be with a traditional texture loaded from disk)
				constexpr D3D11_SUBRESOURCE_DATA* const noInitialData = nullptr;
				const auto d3dResult = direct3dDevice->CreateTexture2D(&textureDescription, noInitialData, &depthBuffer);
				if (FAILED(d3dResult))
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, "Couldn't create depth buffer (HRESULT %#010x)", d3dResult);
					eae6320::Logging::OutputError("Direct3D failed to create the depth buffer resource (HRESULT %#010x)", d3dResult);
					goto OnExit;
				}
			}
			// Create the view
			{
				constexpr D3D11_DEPTH_STENCIL_VIEW_DESC* const noSubResources = nullptr;
				const auto d3dResult = direct3dDevice->CreateDepthStencilView(depthBuffer, noSubResources, &s_depthStencilView);
				if (FAILED(d3dResult))
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, "Couldn't create depth stencil view (HRESULT %#010x)", d3dResult);
					eae6320::Logging::OutputError("Direct3D failed to create the depth stencil view (HRESULT %#010x)", d3dResult);
					goto OnExit;
				}
			}
		}

		// Bind the views
		{
			constexpr unsigned int renderTargetCount = 1;
			direct3dImmediateContext->OMSetRenderTargets(renderTargetCount, &s_renderTargetView[0], s_depthStencilView);
		}
		// Specify that the entire render target should be visible
		{
			D3D11_VIEWPORT viewPort{};
			{
				viewPort.TopLeftX = viewPort.TopLeftY = 0.0f;
				viewPort.Width = static_cast<float>(i_resolutionWidth);
				viewPort.Height = static_cast<float>(i_resolutionHeight);
				viewPort.MinDepth = 0.0f;
				viewPort.MaxDepth = 1.0f;
			}
			constexpr unsigned int viewPortCount = 1;
			direct3dImmediateContext->RSSetViewports(viewPortCount, &viewPort);
		}
		m_depthStencilView = s_depthStencilView;
		m_renderTargetView[0] = s_renderTargetView[0];
	OnExit:

		// Regardless of success or failure the two texture resources should be released
		// (if successful the views will hold internal references to the resources)
		if (backBuffer)
		{
			backBuffer->Release();
			backBuffer = nullptr;
		}
		if (depthBuffer)
		{
			depthBuffer->Release();
			depthBuffer = nullptr;
		}

		return result;
	}
#endif
}