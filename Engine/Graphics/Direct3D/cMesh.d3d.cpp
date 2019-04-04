#include "../cMesh.h"

#include "Includes.h"

namespace eae6320
{
	namespace Graphics
	{
		cResult cMesh::Initialize(eae6320::Graphics::VertexFormats::sMesh* i_inputMesh, eae6320::Graphics::VertexFormats::sIndex* i_inputIndex, unsigned int i_IndexCount, unsigned int i_VertexCount)
		{
			indexCount = i_IndexCount;
			auto result = eae6320::Results::Success;
			auto* const direct3dDevice = eae6320::Graphics::sContext::g_context.direct3dDevice;
			auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
			EAE6320_ASSERT(direct3dDevice);

			// Initialize vertex format
			{
				// Load the compiled binary vertex shader for the input layout
				eae6320::Platform::sDataFromFile vertexShaderDataFromFile;
				std::string errorMessage;
				if (result = eae6320::Platform::LoadBinaryFile("data/Shaders/Vertex/vertexInputLayout.shader", vertexShaderDataFromFile, &errorMessage))
				{
					// Create the vertex layout

					// These elements must match the VertexFormats::sMesh layout struct exactly.
					// They instruct Direct3D how to match the binary data in the vertex buffer
					// to the input elements in a vertex shader
					// (by using so-called "semantic" names so that, for example,
					// "POSITION" here matches with "POSITION" in shader code).
					// Note that OpenGL uses arbitrarily assignable number IDs to do the same thing.
					// Adding "COLOR"
					constexpr unsigned int vertexElementCount = 4;
					D3D11_INPUT_ELEMENT_DESC layoutDescription[vertexElementCount] = {};
					{
						// Slot 0

						// POSITION
						// 3 floats == 12 bytes
						// Offset = 0
						{
							auto& positionElement = layoutDescription[0];

							positionElement.SemanticName = "POSITION";
							positionElement.SemanticIndex = 0;	// (Semantics without modifying indices at the end can always use zero)
							positionElement.Format = DXGI_FORMAT_R32G32B32_FLOAT;
							positionElement.InputSlot = 0;
							positionElement.AlignedByteOffset = offsetof(eae6320::Graphics::VertexFormats::sMesh, x);
							positionElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
							positionElement.InstanceDataStepRate = 0;	// (Must be zero for per-vertex data)
						}
						//For Color
						{
							auto& positionElement = layoutDescription[1];

							positionElement.SemanticName = "COLOR";
							positionElement.SemanticIndex = 0;	// (Semantics without modifying indices at the end can always use zero)
							positionElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
							positionElement.InputSlot = 0;
							positionElement.AlignedByteOffset = offsetof(eae6320::Graphics::VertexFormats::sMesh, r);
							positionElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
							positionElement.InstanceDataStepRate = 0;	// (Must be zero for per-vertex data)
						}
						//For UV
						{
							auto& positionElement = layoutDescription[2];

							positionElement.SemanticName = "TEXCOORD";
							positionElement.SemanticIndex = 0;	// (Semantics without modifying indices at the end can always use zero)
							positionElement.Format = DXGI_FORMAT_R32G32_FLOAT;
							positionElement.InputSlot = 0;
							positionElement.AlignedByteOffset = offsetof(eae6320::Graphics::VertexFormats::sMesh, u);
							positionElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
							positionElement.InstanceDataStepRate = 0;	// (Must be zero for per-vertex data)
						}
						//For Normals
						{
							auto& positionElement = layoutDescription[3];

							positionElement.SemanticName = "NORMAL";
							positionElement.SemanticIndex = 0;	// (Semantics without modifying indices at the end can always use zero)
							positionElement.Format = DXGI_FORMAT_R32G32B32_FLOAT;
							positionElement.InputSlot = 0;
							positionElement.AlignedByteOffset = offsetof(eae6320::Graphics::VertexFormats::sMesh, nx);
							positionElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
							positionElement.InstanceDataStepRate = 0;	// (Must be zero for per-vertex data)
						}

					}

					const auto d3dResult = direct3dDevice->CreateInputLayout(layoutDescription, vertexElementCount,
						vertexShaderDataFromFile.data, vertexShaderDataFromFile.size, &m_vertexInputLayout);
					if (FAILED(result))
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF(false, "Geometry vertex input layout creation failed (HRESULT %#010x)", d3dResult);
						eae6320::Logging::OutputError("Direct3D failed to create the geometry vertex input layout (HRESULT %#010x)", d3dResult);
					}

					vertexShaderDataFromFile.Free();
				}
				else
				{
					EAE6320_ASSERTF(false, errorMessage.c_str());
					eae6320::Logging::OutputError("The geometry vertex input layout shader couldn't be loaded: %s", errorMessage.c_str());
					goto OnExit;
				}
			}
			// Vertex Buffer
			{
				D3D11_BUFFER_DESC bufferDescription{};
				{
					const auto bufferSize = i_VertexCount * sizeof(eae6320::Graphics::VertexFormats::sMesh);
					EAE6320_ASSERT(bufferSize < (uint64_t(1u) << (sizeof(bufferDescription.ByteWidth) * 8)));
					bufferDescription.ByteWidth = static_cast<unsigned int>(bufferSize);
					bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
					bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
					bufferDescription.MiscFlags = 0;
					bufferDescription.StructureByteStride = 0;	// Not used
				}
				D3D11_SUBRESOURCE_DATA initialData{};
				{
					initialData.pSysMem = i_inputMesh;
					// (The other data members are ignored for non-texture buffers)
				}

				const auto d3dResult = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &m_vertexBuffer);
				if (FAILED(d3dResult))
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, "Geometry vertex buffer creation failed (HRESULT %#010x)", d3dResult);
					eae6320::Logging::OutputError("Direct3D failed to create a geometry vertex buffer (HRESULT %#010x)", d3dResult);
					goto OnExit;
				}
			}
			// Index Buffer
			{
				D3D11_BUFFER_DESC indexDescription{};
				{
					const auto bufferSize = indexCount * sizeof(*i_inputIndex);
					EAE6320_ASSERT(bufferSize < (uint64_t(1u) << (sizeof(indexDescription.ByteWidth) * 8)));
					indexDescription.ByteWidth = static_cast<unsigned int>(bufferSize);
					indexDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
					indexDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
					indexDescription.CPUAccessFlags = 0;	// No CPU access is necessary
					indexDescription.MiscFlags = 0;
					indexDescription.StructureByteStride = 0;	// Not used
				}
				D3D11_SUBRESOURCE_DATA indexInitialData{};
				{
					indexInitialData.pSysMem = i_inputIndex;
					// (The other data members are ignored for non-texture buffers)
				}

				const auto d3dResult = direct3dDevice->CreateBuffer(&indexDescription, &indexInitialData, &m_indexBuffer);
				if (FAILED(d3dResult))
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, "Geometry vertex buffer creation failed (HRESULT %#010x)", d3dResult);
					eae6320::Logging::OutputError("Direct3D failed to create a geometry vertex buffer (HRESULT %#010x)", d3dResult);
					goto OnExit;
				}
			}

			// Specify what kind of data the vertex buffer holds
			{
				// Set the layout (which defines how to interpret a single vertex)
				{
					EAE6320_ASSERT(m_vertexInputLayout);
					direct3dImmediateContext->IASetInputLayout(m_vertexInputLayout);
				}
				// Set the topology (which defines how to interpret multiple vertices as a single "primitive";
				// the vertex buffer was defined as a triangle list
				// (meaning that every primitive is a triangle and will be defined by three vertices)
				direct3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			}

		OnExit:

			return result;
		}
		void cMesh::Draw(bool bindVertex)
		{
			auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
			{
				if (bindVertex)
				{
					// Bind a specific vertex buffer to the device as a data source
					{
						EAE6320_ASSERT(m_vertexBuffer);
						constexpr unsigned int startingSlot = 0;
						constexpr unsigned int vertexBufferCount = 1;
						// The "stride" defines how large a single vertex is in the stream of data
						constexpr unsigned int bufferStride = sizeof(VertexFormats::sMesh);
						// It's possible to start streaming data in the middle of a vertex buffer
						constexpr unsigned int bufferOffset = 0;
						direct3dImmediateContext->IASetVertexBuffers(startingSlot, vertexBufferCount, &m_vertexBuffer, &bufferStride, &bufferOffset);
					}
					// Bind a specific Index buffer to the device as a data source
					{
						EAE6320_ASSERT(m_indexBuffer);
						constexpr DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT;
						// The indices start at the beginning of the buffer
						constexpr unsigned int offset = 0;
						direct3dImmediateContext->IASetIndexBuffer(m_indexBuffer, indexFormat, offset);
					}
				}
				// Render triangles from the currently-bound index buffer
				{
					// As of this comment only a single triangle is drawn
					// (you will have to update this code in future assignments!)
					// It's possible to start rendering primitives in the middle of the stream
					constexpr unsigned int indexOfFirstIndexToUse = 0;
					constexpr unsigned int offsetToAddToEachIndex = 0;
					direct3dImmediateContext->DrawIndexed(static_cast<unsigned int>(indexCount), indexOfFirstIndexToUse, offsetToAddToEachIndex);
				}
			}

		}
		cResult cMesh::Shutdown()
		{
			auto result = Results::Success;
			if (m_vertexBuffer)
			{
				m_vertexBuffer->Release();
				m_vertexBuffer = nullptr;
			}
			if (m_indexBuffer)
			{
				m_indexBuffer->Release();
				m_indexBuffer = nullptr;
			}
			if (m_vertexInputLayout)
			{
				m_vertexInputLayout->Release();
				m_vertexInputLayout = nullptr;
			}
			return result;
		}
	}
}