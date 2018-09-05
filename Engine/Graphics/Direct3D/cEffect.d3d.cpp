#include "../cEffect.h"

namespace eae6320
{
	namespace Graphics
	{
		cResult cEffect::Initialize()
		{
			auto result = eae6320::Results::Success;

			if (!(result = eae6320::Graphics::cShader::s_manager.Load("data/Shaders/Vertex/standard.shader",
				s_vertexShader, eae6320::Graphics::ShaderTypes::Vertex)))
			{
				EAE6320_ASSERT(false);
				goto OnExit;
			}
			if (!(result = eae6320::Graphics::cShader::s_manager.Load("data/Shaders/Fragment/animatedshader.shader",
				s_fragmentShader, eae6320::Graphics::ShaderTypes::Fragment)))
			{
				EAE6320_ASSERT(false);
				goto OnExit;
			}
			{
				constexpr uint8_t defaultRenderState = 0;
				if (!(result = s_renderState.Initialize(defaultRenderState)))
				{
					EAE6320_ASSERT(false);
					goto OnExit;
				}
			}

		OnExit:

			return result;
		}
		void cEffect::Bind()
		{
			auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
			{
				constexpr ID3D11ClassInstance* const* noInterfaces = nullptr;
				constexpr unsigned int interfaceCount = 0;
				// Vertex shader
				{
					EAE6320_ASSERT(s_vertexShader);
					auto* const shader = cShader::s_manager.Get(s_vertexShader);
					EAE6320_ASSERT(shader && shader->m_shaderObject.vertex);
					direct3dImmediateContext->VSSetShader(shader->m_shaderObject.vertex, noInterfaces, interfaceCount);
				}
				// Fragment shader
				{
					EAE6320_ASSERT(s_fragmentShader);
					auto* const shader = cShader::s_manager.Get(s_fragmentShader);
					EAE6320_ASSERT(shader && shader->m_shaderObject.fragment);
					direct3dImmediateContext->PSSetShader(shader->m_shaderObject.fragment, noInterfaces, interfaceCount);
				}
			}
			s_renderState.Bind();
		}
		cResult cEffect::Shutdown()
		{
			auto result = Results::Success;
			if (s_vertexShader)
			{
				const auto localResult = cShader::s_manager.Release(s_vertexShader);
				if (!localResult)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = localResult;
					}
				}
			}
			if (s_fragmentShader)
			{
				const auto localResult = cShader::s_manager.Release(s_fragmentShader);
				if (!localResult)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = localResult;
					}
				}
			}
			{
				const auto localResult = s_renderState.CleanUp();
				if (!localResult)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = localResult;
					}
				}
			}
			return result;
		}
	}
}