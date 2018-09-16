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
		class cMesh
		{
		public:
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()
			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cMesh)
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()
			struct sIndex
			{
				uint16_t indexValue;
			};
			void Draw();
			static cResult CreateMesh(eae6320::Graphics::VertexFormats::sMesh* i_inputMesh, sIndex* i_inputIndex, unsigned int i_triangleCount, cMesh*& o_Mesh)
			{
				cResult result = Results::Success;
				o_Mesh = new cMesh();
				result = o_Mesh->Initialize(i_inputMesh, i_inputIndex, i_triangleCount);
				return result;
			}
		private:
			cMesh() {}
			~cMesh()
			{
				Shutdown();
			}
			cResult Initialize();
			cResult Initialize(eae6320::Graphics::VertexFormats::sMesh* i_inputMesh, sIndex* i_inputIndex, unsigned int i_triangleCount)
			{
				auto result = Results::Success;
				indexData = i_inputIndex;
				triangleCount = i_triangleCount;
				vertexData = i_inputMesh;
				result = Initialize();
				return result;
			}
			cResult Shutdown();
#ifdef EAE6320_PLATFORM_D3D
			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* m_vertexBuffer = nullptr;
			// D3D has an "input layout" object that associates the layout of the vertex format struct
			// with the input from a vertex shader
			ID3D11InputLayout* m_vertexInputLayout = nullptr;

			// Index Buffer corresponding to the vertex buffer
			ID3D11Buffer* m_indexBuffer = nullptr;
#elif EAE6320_PLATFORM_GL
			// A vertex buffer holds the data for each vertex
			GLuint m_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint m_vertexArrayId = 0;
			//Creating Index Buffer
			GLuint m_indexBufferId = 0;

#endif
			unsigned int triangleCount;
			eae6320::Graphics::VertexFormats::sMesh* vertexData;
			sIndex* indexData;
		};
	}
}
