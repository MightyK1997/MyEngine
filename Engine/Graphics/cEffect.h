#pragma once
#include "Graphics.h"

#ifdef EAE6320_PLATFORM_D3D 
#include <d3d11.h>
#include <dxgi.h>
#endif

#include "cConstantBuffer.h"
#include "ConstantBufferFormats.h"
#include "cRenderState.h"
#include "cShader.h"
#include "sContext.h"
#include "VertexFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cEvent.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <utility>

namespace eae6320
{
	namespace Graphics
	{
		class cEffect
		{
		public:
			cResult Initialize();
			void Bind();
			cResult Shutdown();
		private:
#ifdef EAE6320_PLATFORM_D3D
			eae6320::Graphics::cShader::Handle m_vertexShader;
			eae6320::Graphics::cShader::Handle m_fragmentShader;

			eae6320::Graphics::cRenderState s_renderState;
#elif EAE6320_PLATFORM_GL
			eae6320::Graphics::cShader::Handle m_vertexShader;
			eae6320::Graphics::cShader::Handle m_fragmentShader;
			GLuint s_programId = 0;
			eae6320::Graphics::cRenderState s_renderState;
#endif
		};
	}
}