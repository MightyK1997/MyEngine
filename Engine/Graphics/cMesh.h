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
#include <utility>


#include "sContext.h"

namespace eae6320
{
	namespace Graphics
	{
		class cMesh
		{
		public:
			struct sIndex
			{
				uint16_t indexValue;
			};
			cMesh(eae6320::Graphics::VertexFormats::sMesh* i_inputMesh, sIndex* i_inputIndex, unsigned int i_triangleCount)
			{
				indexData = i_inputIndex;
				triangleCount = i_triangleCount;
				{
#if EAE6320_PLATFORM_GL
					vertexData[0].x = 0.0f;
					vertexData[0].y = 0.5f;
					vertexData[0].z = 0.0f;

					vertexData[1].x = 1.0f;
					vertexData[1].y = 0.5f;
					vertexData[1].z = 0.0f;

					vertexData[2].x = 0.5f;
					vertexData[2].y = 1.0f;
					vertexData[2].z = 0.0f;

					vertexData[3].x = 0.0f;
					vertexData[3].y = -0.5f;
					vertexData[3].z = 0.0f;

					vertexData[4].x = 1.0f;
					vertexData[4].y = 0.5f;
					vertexData[4].z = 0.0f;

					vertexData[5].x = 0.0f;
					vertexData[5].y = 0.5f;
					vertexData[5].z = 0.0f;

					vertexData[6].x = 0.0f;
					vertexData[6].y = -0.5f;
					vertexData[6].z = 0.0f;

					vertexData[7].x = 1.0f;
					vertexData[7].y = -0.5f;
					vertexData[7].z = 0.0f;

					vertexData[8].x = 1.0f;
					vertexData[8].y = 0.5f;
					vertexData[8].z = 0.0f;

					vertexData[9].x = -0.5f;
					vertexData[9].y = -0.5f;
					vertexData[9].z = 0.0f;

					vertexData[10].x = -0.25f;
					vertexData[10].y = -0.5f;
					vertexData[10].z = 0.0f;

					vertexData[11].x = -0.25f;
					vertexData[11].y = 0.25f;
					vertexData[11].z = 0.0f;

					vertexData[12].x = -0.5f;
					vertexData[12].y = -0.5f;
					vertexData[12].z = 0.0f;

					vertexData[13].x = -0.25f;
					vertexData[13].y = 0.25f;
					vertexData[13].z = 0.0f;

					vertexData[14].x = -0.5f;
					vertexData[14].y = 0.25f;
					vertexData[14].z = 0.0f;

					vertexData[15].x = -0.5f;
					vertexData[15].y = 0.25f;
					vertexData[15].z = 0.0f;

					vertexData[16].x = -0.5f;
					vertexData[16].y = 0.75f;
					vertexData[16].z = 0.0f;

					vertexData[17].x = -0.75f;
					vertexData[17].y = 0.75f;
					vertexData[17].z = 0.0f;

					vertexData[18].x = -0.25f;
					vertexData[18].y = 0.25f;
					vertexData[18].z = 0.0f;

					vertexData[19].x = 0.0f;
					vertexData[19].y = 0.75f;
					vertexData[19].z = 0.0f;

					vertexData[20].x = -0.25f;
					vertexData[20].y = 0.75f;
					vertexData[20].z = 0.0f;
#endif

					/*#elif EAE6320_PLATFORM_D3D
										vertexData[0].x = 0.0f;
										vertexData[0].y = 0.5f;
										vertexData[0].z = 0.0f;

										vertexData[1].x = 0.5f;
										vertexData[1].y = 1.0f;
										vertexData[1].z = 0.0f;

										vertexData[2].x = 1.0f;
										vertexData[2].y = 0.5f;
										vertexData[2].z = 0.0f;

										vertexData[3].x = 0.0f;
										vertexData[3].y = -0.5f;
										vertexData[3].z = 0.0f;

										vertexData[4].x = 0.0f;
										vertexData[4].y = 0.5f;
										vertexData[4].z = 0.0f;

										vertexData[5].x = 1.0f;
										vertexData[5].y = 0.5f;
										vertexData[5].z = 0.0f;

										vertexData[6].x = 0.0f;
										vertexData[6].y = -0.5f;
										vertexData[6].z = 0.0f;

										vertexData[7].x = 1.0f;
										vertexData[7].y = 0.5f;
										vertexData[7].z = 0.0f;

										vertexData[8].x = 1.0f;
										vertexData[8].y = -0.5f;
										vertexData[8].z = 0.0f;

										vertexData[9].x = -0.5f;
										vertexData[9].y = -0.5f;
										vertexData[9].z = 0.0f;

										vertexData[10].x = -0.25f;
										vertexData[10].y = 0.25f;
										vertexData[10].z = 0.0f;

										vertexData[11].x = -0.25f;
										vertexData[11].y = -0.5f;
										vertexData[11].z = 0.0f;

										vertexData[12].x = -0.5f;
										vertexData[12].y = -0.5f;
										vertexData[12].z = 0.0f;

										vertexData[13].x = -0.5f;
										vertexData[13].y = 0.25f;
										vertexData[13].z = 0.0f;

										vertexData[14].x = -0.25f;
										vertexData[14].y = 0.25f;
										vertexData[14].z = 0.0f;

										vertexData[15].x = -0.5f;
										vertexData[15].y = 0.25f;
										vertexData[15].z = 0.0f;

										vertexData[16].x = -0.75f;
										vertexData[16].y = 0.75f;
										vertexData[16].z = 0.0f;

										vertexData[17].x = -0.5f;
										vertexData[17].y = 0.75f;
										vertexData[17].z = 0.0f;

										vertexData[18].x = -0.25f;
										vertexData[18].y = 0.25f;
										vertexData[18].z = 0.0f;

										vertexData[19].x = -0.25f;
										vertexData[19].y = 0.75f;
										vertexData[19].z = 0.0f;

										vertexData[20].x = 0.0f;
										vertexData[20].y = 0.75f;
										vertexData[20].z = 0.0f;
					#endif
									}*/
				}
			}
			cResult Initialize();
			cResult Initialize(eae6320::Graphics::VertexFormats::sMesh* i_InputVertexMesh);
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
			static unsigned int triangleCount;
			eae6320::Graphics::VertexFormats::sMesh* vertexData;
			sIndex* indexData;
		};
	}
}
