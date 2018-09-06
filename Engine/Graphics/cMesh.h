#pragma once
#include "Graphics.h"
//#include <Engine/Windows/Includes.h>

#ifdef EAE6320_PLATFORM_D3D 
#include <d3d11.h>
#include <dxgi.h>
#endif

#include "cConstantBuffer.h"
#include "ConstantBufferFormats.h"
#include "cRenderState.h"
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
		class cMesh
		{
		public:
			cMesh()
			{
				{
#if EAE6320_PLATFORM_GL
					vertexData[0].x = 0.0f;
					vertexData[0].y = 0.0f;
					vertexData[0].z = 0.0f;

					vertexData[1].x = 1.0f;
					vertexData[1].y = 0.0f;
					vertexData[1].z = 0.0f;

					vertexData[2].x = 1.0f;
					vertexData[2].y = 1.0f;
					vertexData[2].z = 0.0f;

					vertexData[3].x = 0.0f;
					vertexData[3].y = 0.0f;
					vertexData[3].z = 0.0f;

					vertexData[4].x = 1.0f;
					vertexData[4].y = 1.0f;
					vertexData[4].z = 0.0f;

					vertexData[5].x = 0.0f;
					vertexData[5].y = 1.0f;
					vertexData[5].z = 0.0f;

					vertexData[6].x = 0.0f;
					vertexData[6].y = 0.0f;
					vertexData[6].z = 0.0f;

					vertexData[7].x = 0.5f;
					vertexData[7].y = -1.0f;
					vertexData[7].z = 0.0f;

					vertexData[8].x = 1.0f;
					vertexData[8].y = 0.0f;
					vertexData[8].z = 0.0f;
#elif EAE6320_PLATFORM_D3D
					vertexData[0].x = 0.0f;
					vertexData[0].y = 0.0f;
					vertexData[0].z = 0.0f;

					vertexData[1].x = 1.0f;
					vertexData[1].y = 1.0f;
					vertexData[1].z = 0.0f;

					vertexData[2].x = 1.0f;
					vertexData[2].y = 0.0f;
					vertexData[2].z = 0.0f;

					vertexData[3].x = 0.0f;
					vertexData[3].y = 0.0f;
					vertexData[3].z = 0.0f;

					vertexData[4].x = 0.0f;
					vertexData[4].y = 1.0f;
					vertexData[4].z = 0.0f;

					vertexData[5].x = 1.0f;
					vertexData[5].y = 1.0f;
					vertexData[5].z = 0.0f;

					vertexData[6].x = 0.0f;
					vertexData[6].y = 0.0f;
					vertexData[6].z = 0.0f;

					vertexData[7].x = 1.0f;
					vertexData[7].y = 0.0f;
					vertexData[7].z = 0.0f;

					vertexData[8].x = 0.5f;
					vertexData[8].y = -1.0f;
					vertexData[8].z = 0.0f;
#endif
				}
			}
			cResult Initialize();
			void Draw();
			cResult Shutdown();
		private:
#ifdef EAE6320_PLATFORM_D3D
			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* m_vertexBuffer = nullptr;
			// D3D has an "input layout" object that associates the layout of the vertex format struct
			// with the input from a vertex shader
			ID3D11InputLayout* m_vertexInputLayout = nullptr;
#elif EAE6320_PLATFORM_GL
			// A vertex buffer holds the data for each vertex
			GLuint m_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint m_vertexArrayId = 0;
#endif
			static const unsigned int triangleCount = 3;
			static const unsigned int vertexCountPerTriangle = 3;
			static const auto vertexCount = triangleCount * vertexCountPerTriangle;
			eae6320::Graphics::VertexFormats::sMesh vertexData[vertexCount];
		};
	}
}
