#pragma once
#include "Graphics.h"
#ifdef EAE6320_PLATFORM_D3D 
#include <d3d11.h>
#include <dxgi.h>
#endif
#include "sContext.h"
#include "cConstantBuffer.h"
#include "ConstantBufferFormats.h"
#include "cRenderState.h"
#include "cShader.h"
#include "VertexFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <Engine/Assets/ReferenceCountedAssets.h>
#include <utility>

namespace eae6320
{
	namespace Graphics
	{
		class cEffect
		{
		public:
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()
			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cEffect)
			void Bind();
			static cResult CreateEffect(std::string i_vertexShaderLocation, std::string i_fragmentShaderLocation, cEffect*& o_Effect)
			{
				cResult result = Results::Success;
				o_Effect = new cEffect();
				result = o_Effect->Initialize(i_vertexShaderLocation, i_fragmentShaderLocation);
				return result;
			}
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()
		private:
			cEffect() {}
			~cEffect() 
			{
				Shutdown();
			}
			cResult Initialize(std::string i_vertexShaderLocation, std::string i_fragmentShaderLocation);
			cResult Shutdown();
#ifdef EAE6320_PLATFORM_D3D
			eae6320::Graphics::cShader::Handle m_vertexShader;
			eae6320::Graphics::cShader::Handle m_fragmentShader;
			eae6320::Graphics::cRenderState s_renderState;
#elif EAE6320_PLATFORM_GL
			eae6320::Graphics::cShader::Handle m_vertexShader;
			eae6320::Graphics::cShader::Handle m_fragmentShader;
			GLuint s_programId = 0;
			eae6320::Graphics::cRenderState s_renderState;
			cResult InitGL();
			cResult ShutdownGL();
#endif
		};
	}
}