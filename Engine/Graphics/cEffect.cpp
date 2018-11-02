#include "cEffect.h"

#include "../Platform/Platform.h"

eae6320::Assets::cManager<eae6320::Graphics::cEffect> eae6320::Graphics::cEffect::s_Manager;

namespace eae6320
{
	namespace Graphics
	{
		cResult cEffect::Initialize(std::string i_vertexShaderLocation, std::string i_fragmentShaderLocation, uint8_t i_TypeOfRender)
		{
			auto result = eae6320::Results::Success;

			if (!(result = eae6320::Graphics::cShader::s_manager.Load(i_vertexShaderLocation.c_str(),
				m_vertexShader, eae6320::Graphics::ShaderTypes::Vertex)))
			{
				EAE6320_ASSERT(false);
				goto OnExit;
			}
			if (!(result = eae6320::Graphics::cShader::s_manager.Load(i_fragmentShaderLocation.c_str(),
				m_fragmentShader, eae6320::Graphics::ShaderTypes::Fragment)))
			{
				EAE6320_ASSERT(false);
				goto OnExit;
			}
			//Changing this to enable depth buffering and other state from the game, defaults to 0 if nothing is passed
			{
				if (!(result = s_renderState.Initialize(i_TypeOfRender)))
				{
					EAE6320_ASSERT(false);
					goto OnExit;
				}
			}
#ifdef EAE6320_PLATFORM_GL
			InitGL();
#endif
		OnExit:

			return result;
		}
		cResult cEffect::Shutdown()
		{
			auto result = Results::Success;
#if EAE6320_PLATFORM_GL
			result = ShutdownGL();
#endif
			if (m_vertexShader)
			{
				const auto localResult = cShader::s_manager.Release(m_vertexShader);
				if (!localResult)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = localResult;
					}
				}
			}
			if (m_fragmentShader)
			{
				const auto localResult = cShader::s_manager.Release(m_fragmentShader);
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

		//Load data from file
		cResult cEffect::Load(const char* const i_FileName, cEffect*& o_Effect)
		{
			cResult result = Results::Success;
			cEffect* outputEffect = nullptr;

			eae6320::Platform::sDataFromFile dataFromFile;
			std::string errorMessage;
			result = eae6320::Platform::LoadBinaryFile(i_FileName, dataFromFile, &errorMessage);
			uintptr_t offset = reinterpret_cast<uintptr_t>(dataFromFile.data);
			const uintptr_t finalOffset = offset + dataFromFile.size;

			uint8_t renderState = *reinterpret_cast<uint8_t*>(offset);
			offset += sizeof(uint8_t);
			uint8_t lengthOfVertexPath = *reinterpret_cast<uint8_t*>(offset);
			offset += sizeof(uint8_t);
			char* pathToVertexShader = reinterpret_cast<char*>(offset);
			offset += lengthOfVertexPath + 1;
			char* pathToFragmentShader = reinterpret_cast<char*>(offset);

			result = CreateEffect(std::string(pathToVertexShader), std::string(pathToFragmentShader), renderState, outputEffect);

			o_Effect = outputEffect;

			return result;
		}
	}
}