#include "cMesh.h"
namespace eae6320
{
	namespace Graphics
	{
		
		cResult cMesh::Initialize()
		{
#if EAE6320_PLATFORM_D3D
			auto result = eae6320::Results::Success;

			auto* const direct3dDevice = eae6320::Graphics::sContext::g_context.direct3dDevice;
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
					constexpr unsigned int vertexElementCount = 1;
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
					}

					const auto d3dResult = direct3dDevice->CreateInputLayout(layoutDescription, vertexElementCount,
						vertexShaderDataFromFile.data, vertexShaderDataFromFile.size, &s_vertexInputLayout);
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

				//Changing the triangle count to 2 to render a rectangle.
				constexpr unsigned int triangleCount = 2;
				constexpr unsigned int vertexCountPerTriangle = 3;
				constexpr auto vertexCount = triangleCount * vertexCountPerTriangle;
				eae6320::Graphics::VertexFormats::sMesh vertexData[vertexCount];
				{
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
				}
				D3D11_BUFFER_DESC bufferDescription{};
				{
					const auto bufferSize = vertexCount * sizeof(eae6320::Graphics::VertexFormats::sMesh);
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
					initialData.pSysMem = vertexData;
					// (The other data members are ignored for non-texture buffers)
				}

				const auto d3dResult = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &s_vertexBuffer);
				if (FAILED(d3dResult))
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, "Geometry vertex buffer creation failed (HRESULT %#010x)", d3dResult);
					eae6320::Logging::OutputError("Direct3D failed to create a geometry vertex buffer (HRESULT %#010x)", d3dResult);
					goto OnExit;
				}
			}

		OnExit:

			return result;
#elif EAE6320_PLATFORM_GL
auto result = eae6320::Results::Success;

// Create a vertex array object and make it active
{
	constexpr GLsizei arrayCount = 1;
	glGenVertexArrays(arrayCount, &s_vertexArrayId);
	const auto errorCode = glGetError();
	if (errorCode == GL_NO_ERROR)
	{
		glBindVertexArray(s_vertexArrayId);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to bind a new vertex array: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}
	else
	{
		result = eae6320::Results::Failure;
		EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
		eae6320::Logging::OutputError("OpenGL failed to get an unused vertex array ID: %s",
			reinterpret_cast<const char*>(gluErrorString(errorCode)));
		goto OnExit;
	}
}
// Create a vertex buffer object and make it active
{
	constexpr GLsizei bufferCount = 1;
	glGenBuffers(bufferCount, &s_vertexBufferId);
	const auto errorCode = glGetError();
	if (errorCode == GL_NO_ERROR)
	{
		glBindBuffer(GL_ARRAY_BUFFER, s_vertexBufferId);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to bind a new vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}
	else
	{
		result = eae6320::Results::Failure;
		EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
		eae6320::Logging::OutputError("OpenGL failed to get an unused vertex buffer ID: %s",
			reinterpret_cast<const char*>(gluErrorString(errorCode)));
		goto OnExit;
	}
}
// Assign the data to the buffer
{
	constexpr unsigned int triangleCount = 2;
	constexpr unsigned int vertexCountPerTriangle = 3;
	const auto vertexCount = triangleCount * vertexCountPerTriangle;
	eae6320::Graphics::VertexFormats::sMesh vertexData[vertexCount];
	{
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
	}
	const auto bufferSize = vertexCount * sizeof(*vertexData);
	EAE6320_ASSERT(bufferSize < (uint64_t(1u) << (sizeof(GLsizeiptr) * 8)));
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(bufferSize), reinterpret_cast<GLvoid*>(vertexData),
		// In our class we won't ever read from the buffer
		GL_STATIC_DRAW);
	const auto errorCode = glGetError();
	if (errorCode != GL_NO_ERROR)
	{
		result = eae6320::Results::Failure;
		EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
		eae6320::Logging::OutputError("OpenGL failed to allocate the vertex buffer: %s",
			reinterpret_cast<const char*>(gluErrorString(errorCode)));
		goto OnExit;
	}
}
// Initialize vertex format
{
	// The "stride" defines how large a single vertex is in the stream of data
	// (or, said another way, how far apart each position element is)
	const auto stride = static_cast<GLsizei>(sizeof(eae6320::Graphics::VertexFormats::sMesh));

	// Position (0)
	// 3 floats == 12 bytes
	// Offset = 0
	{
		constexpr GLuint vertexElementLocation = 0;
		constexpr GLint elementCount = 3;
		constexpr GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is
		glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride,
			reinterpret_cast<GLvoid*>(offsetof(eae6320::Graphics::VertexFormats::sMesh, x)));
		const auto errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glEnableVertexAttribArray(vertexElementLocation);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to enable the POSITION vertex attribute at location %u: %s",
					vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to set the POSITION vertex attribute at location %u: %s",
				vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}
}

OnExit:

return result;
#endif
		}

		void cMesh::Draw()
		{
#if EAE6320_PLATFORM_D3D
			auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
			{
				// Bind a specific vertex buffer to the device as a data source
				{
					EAE6320_ASSERT(s_vertexBuffer);
					constexpr unsigned int startingSlot = 0;
					constexpr unsigned int vertexBufferCount = 1;
					// The "stride" defines how large a single vertex is in the stream of data
					constexpr unsigned int bufferStride = sizeof(VertexFormats::sMesh);
					// It's possible to start streaming data in the middle of a vertex buffer
					constexpr unsigned int bufferOffset = 0;
					direct3dImmediateContext->IASetVertexBuffers(startingSlot, vertexBufferCount, &s_vertexBuffer, &bufferStride, &bufferOffset);
				}
				// Specify what kind of data the vertex buffer holds
				{
					// Set the layout (which defines how to interpret a single vertex)
					{
						EAE6320_ASSERT(s_vertexInputLayout);
						direct3dImmediateContext->IASetInputLayout(s_vertexInputLayout);
					}
					// Set the topology (which defines how to interpret multiple vertices as a single "primitive";
					// the vertex buffer was defined as a triangle list
					// (meaning that every primitive is a triangle and will be defined by three vertices)
					direct3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				}
				// Render triangles from the currently-bound vertex buffer
				{
					// As of this comment only a single triangle is drawn
					// (you will have to update this code in future assignments!)

					//Changing the number of triangles to 2 to render a rectangle
					constexpr unsigned int triangleCount = 2;
					constexpr unsigned int vertexCountPerTriangle = 3;
					constexpr auto vertexCountToRender = triangleCount * vertexCountPerTriangle;
					// It's possible to start rendering primitives in the middle of the stream
					constexpr unsigned int indexOfFirstVertexToRender = 0;
					direct3dImmediateContext->Draw(vertexCountToRender, indexOfFirstVertexToRender);
				}
		}
#elif EAE6320_PLATFORM_GL
			// Bind a specific vertex buffer to the device as a data source
			{
				glBindVertexArray(s_vertexArrayId);
				EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			}
			// Render triangles from the currently-bound vertex buffer
			{
				// The mode defines how to interpret multiple vertices as a single "primitive";
				// a triangle list is defined
				// (meaning that every primitive is a triangle and will be defined by three vertices)
				constexpr GLenum mode = GL_TRIANGLES;
				// It's possible to start rendering primitives in the middle of the stream
				constexpr GLint indexOfFirstVertexToRender = 0;
				// As of this comment we are only drawing a single triangle
				// (you will have to update this code in future assignments!)
				constexpr unsigned int triangleCount = 2;
				constexpr unsigned int vertexCountPerTriangle = 3;
				constexpr auto vertexCountToRender = triangleCount * vertexCountPerTriangle;
				glDrawArrays(mode, indexOfFirstVertexToRender, vertexCountToRender);
				EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			}
#endif
		}

		void cMesh::Shutdown()
		{
#if EAE6320_PLATFORM_D3D
			if (s_vertexBuffer)
			{
				s_vertexBuffer->Release();
				s_vertexBuffer = nullptr;
			}
			if (s_vertexInputLayout)
			{
				s_vertexInputLayout->Release();
				s_vertexInputLayout = nullptr;
			}
#elif EAE6320_PLATFORM_GL
			if (s_vertexArrayId != 0)
			{
				// Make sure that the vertex array isn't bound
				{
					// Unbind the vertex array
					glBindVertexArray(0);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						//if (result)
						//{
						//	result = Results::Failure;
						//}
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Logging::OutputError("OpenGL failed to unbind all vertex arrays before cleaning up geometry: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
					}
				}
				constexpr GLsizei arrayCount = 1;
				glDeleteVertexArrays(arrayCount, &s_vertexArrayId);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					/*if (result)
					{
						result = Results::Failure;
					}*/
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to delete the vertex array: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
				s_vertexArrayId = 0;
			}
			if (s_vertexBufferId != 0)
			{
				constexpr GLsizei bufferCount = 1;
				glDeleteBuffers(bufferCount, &s_vertexBufferId);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					/*if (result)
					{
						result = Results::Failure;
					}*/
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
				s_vertexBufferId = 0;
			}
#endif
		}
	}
}