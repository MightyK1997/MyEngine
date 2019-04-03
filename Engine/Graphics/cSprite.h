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
#include "VertexFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <Engine/Assets/ReferenceCountedAssets.h>
#include <utility>


#include "sContext.h"

namespace eae6320
{
	namespace Graphics
	{
		class cSprite
		{
		public:

			using Handle = Assets::cHandle<cSprite>;
			static Assets::cManager<cSprite> s_Manager;

			static cResult Load(const std::string& i_FilePath, cSprite*& o_Sprite);

			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()
				EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cSprite)
				void Draw(bool bindVertex);
			static cResult CreateSprite(eae6320::Graphics::VertexFormats::sSprite* i_inputMesh, unsigned int i_VertexCount, cSprite*& o_Sprite)
			{
				cResult result = Results::Success;
				o_Sprite = new cSprite();
				result = o_Sprite->Initialize(i_inputMesh, i_VertexCount);
				return result;
			}
		private:
			cSprite() = default;
			~cSprite()
			{
				Shutdown();
			}
			cResult Initialize(eae6320::Graphics::VertexFormats::sSprite* i_inputSprite, unsigned int i_VertexCount);
			cResult Shutdown();
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()
#ifdef EAE6320_PLATFORM_D3D
				// A vertex buffer holds the data for each vertex
				ID3D11Buffer* m_vertexBuffer = nullptr;
			// D3D has an "input layout" object that associate s the layout of the vertex format struct
			// with the input from a vertex shader
			ID3D11InputLayout* m_vertexInputLayout = nullptr;
#endif
		};
	}
}