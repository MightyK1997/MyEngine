#include "../cEffect.h"

namespace eae6320
{
	namespace Graphics
	{
		void cEffect::Bind()
		{
			auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
			{
				constexpr ID3D11ClassInstance* const* noInterfaces = nullptr;
				constexpr unsigned int interfaceCount = 0;
				// Vertex shader
				{
					EAE6320_ASSERT(m_vertexShader);
					auto* const shader = cShader::s_manager.Get(m_vertexShader);
					EAE6320_ASSERT(shader && shader->m_shaderObject.vertex);
					direct3dImmediateContext->VSSetShader(shader->m_shaderObject.vertex, noInterfaces, interfaceCount);
				}
				// Fragment shader
				{
					EAE6320_ASSERT(m_fragmentShader);
					auto* const shader = cShader::s_manager.Get(m_fragmentShader);
					EAE6320_ASSERT(shader && shader->m_shaderObject.fragment);
					direct3dImmediateContext->PSSetShader(shader->m_shaderObject.fragment, noInterfaces, interfaceCount);
				}
			}
			s_renderState.Bind();
		}
	}
}