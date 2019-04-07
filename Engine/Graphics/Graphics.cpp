#include <cmath>
#include "Graphics.h"
#include "GraphicsHelper.h"
#include "cMesh.h"
#include "cEffect.h"
#include "cMaterial.h"
#include "RenderCommands.h"
#include "../Physics/cGameObject.h"
#include <algorithm>

eae6320::Graphics::cConstantBuffer eae6320::Graphics::s_constantBuffer_perFrame(eae6320::Graphics::ConstantBufferTypes::PerFrame);

eae6320::Graphics::cSamplerState::Handle eae6320::Graphics::m_SamplerStateHandle;
namespace
{
	struct sDataRequiredToRenderAFrame
	{
		eae6320::Graphics::ConstantBufferFormats::sPerFrame constantData_perFrame;
		eae6320::Graphics::ConstantBufferFormats::sPerDrawCall constantData_perDrawCall[eae6320::Graphics::m_maxNumberofMeshesAndEffects];
		eae6320::Graphics::ConstantBufferFormats::sPerMaterialCall constantData_perMaterial[eae6320::Graphics::m_maxNumberofMeshesAndEffects];
		eae6320::Graphics::sColor backBufferValue_perFrame;
		std::vector<uint64_t> m_RenderHandles;
		unsigned int m_NumberOfEffectsToRender;
	};
	eae6320::Graphics::cConstantBuffer s_constantBuffer_perDrawCall(eae6320::Graphics::ConstantBufferTypes::PerDrawCall);
	eae6320::Graphics::cConstantBuffer s_ConstantBuffer_perMaterial(eae6320::Graphics::ConstantBufferTypes::PerMaterial);


	//In our class there will be two copies of the data required to render a frame:
	   //* One of them will be getting populated by the data currently being submitted by the application loop thread
	   //* One of them will be fully populated, 
	sDataRequiredToRenderAFrame s_dataRequiredToRenderAFrame[2];
	auto* s_dataBeingSubmittedByApplicationThread = &s_dataRequiredToRenderAFrame[0];
	auto* s_dataBeingRenderedByRenderThread = &s_dataRequiredToRenderAFrame[1];

	// The following two events work together to make sure that
	// the main/render thread and the application loop thread can work in parallel but stay in sync:
	// This event is signaled by the application loop thread when it has finished submitting render data for a frame
	// (the main/render thread waits for the signal)
	eae6320::Concurrency::cEvent s_whenAllDataHasBeenSubmittedFromApplicationThread;
	// This event is signaled by the main/render thread when it has swapped render data pointers.
	// This means that the renderer is now working with all the submitted data it needs to render the next frame,
	// and the application loop thread can start submitting data for the following frame
	// (the application loop thread waits for the signal)
	eae6320::Concurrency::cEvent s_whenDataForANewFrameCanBeSubmittedFromApplicationThread;

	//Graphics Helper 
	eae6320::Graphics::GraphicsHelper* s_helper;
	uint64_t currentEffectIndex = (uint64_t)1 << 56;
	uint64_t currentMeshIndex = (uint64_t)1 << 56;
	uint64_t currentMaterialIndex = (uint64_t)1 << 56;
}

// Submission
//-----------

void eae6320::Graphics::SubmitElapsedTime(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);
	auto& constantData_perFrame = s_dataBeingSubmittedByApplicationThread->constantData_perFrame;
	constantData_perFrame.g_elapsedSecondCount_systemTime = i_elapsedSecondCount_systemTime;
	constantData_perFrame.g_elapsedSecondCount_simulationTime = i_elapsedSecondCount_simulationTime;
}

eae6320::cResult eae6320::Graphics::WaitUntilDataForANewFrameCanBeSubmitted(const unsigned int i_timeToWait_inMilliseconds)
{
	return Concurrency::WaitForEvent(s_whenDataForANewFrameCanBeSubmittedFromApplicationThread, i_timeToWait_inMilliseconds);
}

eae6320::cResult eae6320::Graphics::SignalThatAllDataForAFrameHasBeenSubmitted()
{
	return s_whenAllDataHasBeenSubmittedFromApplicationThread.Signal();
}

// Render
//-------
void eae6320::Graphics::SetBackBufferValue(eae6320::Graphics::sColor i_BackBuffer)
{
	auto& ColorValue = s_dataBeingSubmittedByApplicationThread->backBufferValue_perFrame;
	ColorValue = i_BackBuffer;
}

using namespace eae6320::Graphics::RenderCommands;

//This function gets called from the game to set the meshes and effects to render
void eae6320::Graphics::SetEffectsAndMeshesToRender(eae6320::Physics::cGameObject* i_GameObject[100],
	eae6320::Math::cMatrix_transformation i_LocaltoWorldTransforms[100], unsigned i_NumberOfGameObjectsToRender, Graphics::cLight*  i_DirectionalLight, Graphics::cLight* i_PointLight,
	eae6320::Graphics::cCamera* i_Camera, const float i_secondCountToExtrapolate)
{
	EAE6320_ASSERT(i_NumberOfGameObjectsToRender < m_maxNumberofMeshesAndEffects);
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);
	auto& constDataBuffer = s_dataBeingSubmittedByApplicationThread->constantData_perFrame;
	constDataBuffer.g_transform_worldToCamera = eae6320::Math::cMatrix_transformation::CreateWorldToCameraTransform(
		i_Camera->PredictFutureOrientation(i_secondCountToExtrapolate), i_Camera->PredictFuturePosition(i_secondCountToExtrapolate));
	constDataBuffer.g_transform_cameraToProjected = eae6320::Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(0.745f, 1, 0.1f, 100);
	constDataBuffer.g_CameraPositionInWorld = i_Camera->GetCameraPosition();


	constDataBuffer.g_LightRotation = i_DirectionalLight->GetLightRotation().CalculateForwardDirection();
	constDataBuffer.g_LightColor = eae6320::Graphics::Color::ConvertNormalizedsRGBToLinear(i_DirectionalLight->GetLightColor());
	constDataBuffer.g_LightPositionInWorld = i_DirectionalLight->GetLightPosition();

	constDataBuffer.g_PointLightColor = eae6320::Graphics::Color::ConvertNormalizedsRGBToLinear(i_PointLight->GetLightColor());
	constDataBuffer.g_PointLightPositionInWorld = i_PointLight->GetLightPosition();
	constDataBuffer.g_PointLightRotation = i_PointLight->GetLightRotation().CalculateForwardDirection();


	auto& renderCommand = s_dataBeingSubmittedByApplicationThread->m_RenderHandles;
	s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender = i_NumberOfGameObjectsToRender;
	auto m_allDrawCallConstants = s_dataBeingSubmittedByApplicationThread->constantData_perDrawCall;
	auto constantMaterialData = s_dataBeingSubmittedByApplicationThread->constantData_perMaterial;
	renderCommand.clear();
	for (unsigned int i = 0; i < (s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender > m_maxNumberofMeshesAndEffects ? m_maxNumberofMeshesAndEffects : s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender); i++)
	{
		auto materialHandle = i_GameObject[i]->GetGameObjectMaterialHandle();
		auto material = cMaterial::s_Manager.Get(materialHandle);
		auto materialHandleIndex = materialHandle.GetIndex();
		auto effectHandle = material->GetEffectHandle();
		auto effect = cEffect::s_Manager.Get(effectHandle);
		auto effectHandleIndex = effectHandle.GetIndex();
		auto meshHandleIndex = i_GameObject[i]->GetGameObjectMeshHandle().GetIndex();
		constantMaterialData[i].g_color = material->GetMaterialColor();
		m_allDrawCallConstants[i].g_transform_localToWorld = (constDataBuffer.g_transform_worldToCamera * i_LocaltoWorldTransforms[i]);
		m_allDrawCallConstants[i].g_transform_localToProjected = constDataBuffer.g_transform_cameraToProjected *(constDataBuffer.g_transform_worldToCamera * i_LocaltoWorldTransforms[i]);
		bool isEffectDependent = effect->IsEffectDependent();
		auto zValue = (constDataBuffer.g_transform_worldToCamera * i_LocaltoWorldTransforms[i]).GetTranslation().z;
		zValue = -((zValue - 0.1f) / (100-0.1f));
		if (zValue > 1) zValue = 1;
		if (zValue < 0) zValue = 0;
		zValue = isEffectDependent ? 1 - zValue : zValue;
		uint64_t a = 0;
		if (isEffectDependent)
		{
			a |= ((a | static_cast<uint64_t>(effectHandleIndex) << static_cast<uint64_t>(BitShiftsForDependentRenderCommands::E_EFFECTSHIFT)) |
				(static_cast<uint64_t>(materialHandleIndex) << static_cast<uint64_t>(BitShiftsForDependentRenderCommands::E_MATERIALSHIFT)) |
				(static_cast<uint64_t>(zValue * 255) << static_cast<uint64_t>(BitShiftsForDependentRenderCommands::E_DEPTHSHIFT)) |
				(static_cast<uint64_t>((isEffectDependent) ? (RenderCommandTypes::E_DRAWDEPENDENT) : (RenderCommandTypes::E_DRAWINDEPENDENT)) << static_cast<uint64_t>(BitShiftsForCommonRenderCommands::E_TYPESHIFT)) |
				(static_cast<uint64_t>(meshHandleIndex) << static_cast<uint64_t>(BitShiftsForCommonRenderCommands::E_MESHSHIFT)) |
				i);
		}
		else
		{
			a |= ((a | static_cast<uint64_t>(effectHandleIndex) << static_cast<uint64_t>(BitShiftsForIndependentRenderCommands::E_EFFECTSHIFT)) |
				(static_cast<uint64_t>(materialHandleIndex) << static_cast<uint64_t>(BitShiftsForIndependentRenderCommands::E_MATERIALSHIFT)) |
				(static_cast<uint64_t>(zValue * 255) << static_cast<uint64_t>(BitShiftsForIndependentRenderCommands::E_DEPTHSHIFT)) |
				(static_cast<uint64_t>((isEffectDependent) ? (RenderCommandTypes::E_DRAWDEPENDENT) : (RenderCommandTypes::E_DRAWINDEPENDENT)) << static_cast<uint64_t>(BitShiftsForCommonRenderCommands::E_TYPESHIFT)) |
				(static_cast<uint64_t>(meshHandleIndex) << static_cast<uint64_t>(BitShiftsForCommonRenderCommands::E_MESHSHIFT)) |
				i);
		}
		renderCommand.push_back(a);
	}
	std::sort(renderCommand.begin(), renderCommand.end());
}

void eae6320::Graphics::RenderFrame()
{
	// Wait for the application loop to submit data to be rendered
	{
		const auto result = Concurrency::WaitForEvent(s_whenAllDataHasBeenSubmittedFromApplicationThread);
		if (result)
		{
			// Switch the render data pointers so that
			// the data that the application just submitted becomes the data that will now be rendered
			std::swap(s_dataBeingSubmittedByApplicationThread, s_dataBeingRenderedByRenderThread);
			// Once the pointers have been swapped the application loop can submit new data
			const auto result = s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Signal();
			if (!result)
			{
				EAE6320_ASSERTF(false, "Couldn't signal that new graphics data can be submitted");
				Logging::OutputError("Failed to signal that new render data can be submitted");
				UserOutput::Print("The renderer failed to signal to the application that new graphics data can be submitted."
					" The application is probably in a bad state and should be exited");
				return;
			}
		}
		else
		{
			EAE6320_ASSERTF(false, "Waiting for the graphics data to be submitted failed");
			Logging::OutputError("Waiting for the application loop to submit data to be rendered failed");
			UserOutput::Print("The renderer failed to wait for the application to submit data to be rendered."
				" The application is probably in a bad state and should be exited");
			return;
		}
		s_helper->SetRenderTargetView(s_dataBeingRenderedByRenderThread->backBufferValue_perFrame);
		s_helper->ClearDepthStencilView();
		s_helper->UpdateConstantBuffer(s_dataBeingRenderedByRenderThread->constantData_perFrame);

		auto allRenderCommands = s_dataBeingRenderedByRenderThread->m_RenderHandles;
		for (unsigned int i = 0; i < (s_dataBeingRenderedByRenderThread->m_NumberOfEffectsToRender > m_maxNumberofMeshesAndEffects ? m_maxNumberofMeshesAndEffects : s_dataBeingRenderedByRenderThread->m_NumberOfEffectsToRender); i++)
		{
			auto typeOfRenderCommand = static_cast<uint64_t>(BitMasksForRenderCommands::E_COMMONBITMASK) & (allRenderCommands[i] >> static_cast<uint64_t>(BitShiftsForCommonRenderCommands::E_TYPESHIFT));
			bool isDependent = (typeOfRenderCommand == static_cast<uint64_t>(RenderCommandTypes::E_DRAWDEPENDENT));

			uint64_t effectIndex, materialIndex;

			if (isDependent)
			{
				effectIndex = static_cast<uint64_t>(BitMasksForRenderCommands::E_COMMONBITMASK) & (allRenderCommands[i] >> static_cast<uint64_t>(BitShiftsForDependentRenderCommands::E_EFFECTSHIFT));
				materialIndex = static_cast<uint64_t>(BitMasksForRenderCommands::E_COMMONBITMASK) & (allRenderCommands[i] >> static_cast<uint64_t>(BitShiftsForDependentRenderCommands::E_MATERIALSHIFT));
			}
			else
			{
				effectIndex = (allRenderCommands[i] >> static_cast<uint64_t>(BitShiftsForIndependentRenderCommands::E_EFFECTSHIFT));
				materialIndex = static_cast<uint64_t>(BitMasksForRenderCommands::E_COMMONBITMASK) & (allRenderCommands[i] >> static_cast<uint64_t>(BitShiftsForIndependentRenderCommands::E_MATERIALSHIFT));
			}
			uint64_t meshIndex = static_cast<uint64_t>(BitMasksForRenderCommands::E_COMMONBITMASK) & (allRenderCommands[i] >>
				static_cast<uint64_t>(BitShiftsForCommonRenderCommands::E_MESHSHIFT));
			uint64_t index = static_cast<uint64_t>(BitMasksForRenderCommands::E_COMMONBITMASK) & (allRenderCommands[i]);
			

			auto material = cMaterial::s_Manager.UnsafeGet(static_cast<uint32_t>(materialIndex));

			auto texture = cTexture::s_manager.Get(material->GetTextureHandle());

			auto tempEffect = cEffect::s_Manager.UnsafeGet(static_cast<uint32_t>(effectIndex));
 			auto tempMesh = cMesh::s_Manager.UnsafeGet(static_cast<uint32_t>(meshIndex));

			if ((currentEffectIndex ^ effectIndex))
			{
				tempEffect->Bind();
				currentEffectIndex = effectIndex;
			}
			texture->Bind(0);
			if (currentMaterialIndex ^ materialIndex)
			{
				s_ConstantBuffer_perMaterial.Update(&s_dataBeingRenderedByRenderThread->constantData_perMaterial[index]);
			}
			s_constantBuffer_perDrawCall.Update(&s_dataBeingRenderedByRenderThread->constantData_perDrawCall[index]);
			if ((currentMeshIndex ^ meshIndex))
			{
				tempMesh->Draw(true);
				currentMeshIndex = meshIndex;
			}
			else
			{
				tempMesh->Draw(false);
			}
		}
		s_helper->SwapChain();

		//CleanUp
		s_dataBeingRenderedByRenderThread->m_NumberOfEffectsToRender = 0;
	}
}
eae6320::cResult eae6320::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;
	s_helper = new eae6320::Graphics::GraphicsHelper();

	uint8_t samplerStateBits = SamplerStates::eSamplerState::Tile;

	// Initialize the platform-specific context
	if (!(result = sContext::g_context.Initialize(i_initializationParameters)))
	{
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	// Initialize the asset managers
	{
		if (!(result = cShader::s_manager.Initialize()))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}

	// Initialize the platform-independent graphics objects
	{
		if (result = s_constantBuffer_perFrame.Initialize())
		{
			// There is only a single per-frame constant buffer that is re-used
			// and so it can be bound at initialization time and never unbound
			s_constantBuffer_perFrame.Bind(
				// In our class both vertex and fragment shaders use per-frame constant data
				ShaderTypes::Vertex | ShaderTypes::Fragment);
		}
		else
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}

	//Initialize Platform dependent per draw buffer
	{
		if (result = s_constantBuffer_perDrawCall.Initialize())
		{
			s_constantBuffer_perDrawCall.Bind(ShaderTypes::Vertex | ShaderTypes::Fragment);
		}
		else
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}

	{
		if (result = s_ConstantBuffer_perMaterial.Initialize())
		{
			s_ConstantBuffer_perMaterial.Bind(ShaderTypes::Vertex | ShaderTypes::Fragment);
		}
	}
	// Initialize the events
	{
		if (!(result = s_whenAllDataHasBeenSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled)))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
		if (!(result = s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled,
			Concurrency::EventState::Signaled)))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}

	eae6320::Graphics::cSamplerState::s_manager.Load(samplerStateBits, m_SamplerStateHandle);
	eae6320::Graphics::cSamplerState::s_manager.Get(m_SamplerStateHandle)->Bind(0);
	// Initialize the views, Shading  data and Geometry
	result = s_helper->Initialize(i_initializationParameters);

OnExit:

	return result;
}

eae6320::cResult eae6320::Graphics::CleanUp()
{
	auto result = s_helper->CleanUp();
	eae6320::Graphics::cSamplerState::s_manager.Release(m_SamplerStateHandle);
	//CleanUp
	s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender = 0;
	{
		const auto localResult = s_constantBuffer_perFrame.CleanUp();
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
		const auto localResult = s_constantBuffer_perDrawCall.CleanUp();
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
		const auto localResult = s_ConstantBuffer_perMaterial.CleanUp();
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
		const auto localResult = cShader::s_manager.CleanUp();
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
		const auto localResult = sContext::g_context.CleanUp();
		if (!localResult)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = localResult;
			}
		}
	}
	delete(s_helper);
	return result;
}