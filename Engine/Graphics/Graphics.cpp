#include <cmath>
#include "Graphics.h"
#include "GraphicsHelper.h"
#include "cMesh.h"
#include "cEffect.h"
#include "../Physics/cGameObject.h"

eae6320::Graphics::cConstantBuffer eae6320::Graphics::s_constantBuffer_perFrame(eae6320::Graphics::ConstantBufferTypes::PerFrame);
namespace
{
	struct sDataRequiredToRenderAFrame
	{
		eae6320::Graphics::ConstantBufferFormats::sPerFrame constantData_perFrame;
		eae6320::Graphics::ConstantBufferFormats::sPerDrawCall constantData_perDrawCall[eae6320::Graphics::m_maxNumberofMeshesAndEffects];
		eae6320::Graphics::sColor backBufferValue_perFrame;
		eae6320::Graphics::sEffectsAndMeshesToRender m_MeshesAndEffects[eae6320::Graphics::m_maxNumberofMeshesAndEffects];
		unsigned int m_NumberOfEffectsToRender;
	};
	eae6320::Graphics::cConstantBuffer s_constantBuffer_perDrawCall(eae6320::Graphics::ConstantBufferTypes::PerDrawCall);
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

//This function gets called from the game to set the meshes and effects to render
void eae6320::Graphics::SetEffectsAndMeshesToRender(sEffectsAndMeshesToRender i_EffectsAndMeshes[eae6320::Graphics::m_maxNumberofMeshesAndEffects], unsigned int i_NumberOfEffectsAndMeshesToRender)
{
	EAE6320_ASSERT(i_NumberOfEffectsAndMeshesToRender < m_maxNumberofMeshesAndEffects);
	auto& meshesAndEffects = s_dataBeingSubmittedByApplicationThread->m_MeshesAndEffects;
	s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender = i_NumberOfEffectsAndMeshesToRender;
	auto m_allMeshes = s_dataBeingSubmittedByApplicationThread->m_MeshesAndEffects;

	for (unsigned int i = 0; i < (s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender > m_maxNumberofMeshesAndEffects ? m_maxNumberofMeshesAndEffects : s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender); i++)
	{
		meshesAndEffects[i] = i_EffectsAndMeshes[i];
		meshesAndEffects[i].m_RenderEffect->IncrementReferenceCount();
		meshesAndEffects[i].m_RenderMesh->IncrementReferenceCount();
	}
	//size_t somesize = sizeof(eae6320::Graphics::ConstantBufferFormats::sPerFrame);
	//somesize = sizeof(eae6320::Graphics::sColor);
	//somesize = sizeof(eae6320::Graphics::sEffectsAndMeshesToRender);
	//somesize = sizeof(unsigned int);

	//size_t meshSize = sizeof(cMesh);
	//size_t effectSize = sizeof(cEffect);
	//size_t structSize = sizeof(sDataRequiredToRenderAFrame);
}

void eae6320::Graphics::SetCameraDataToRender(eae6320::Math::cMatrix_transformation i_WorldToCameraMatrix, eae6320::Math::cMatrix_transformation i_CameraToProjectedMatrix)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);
	auto& constDataBuffer = s_dataBeingSubmittedByApplicationThread->constantData_perFrame;
	constDataBuffer.g_transform_worldToCamera = i_WorldToCameraMatrix;
	constDataBuffer.g_transform_cameraToProjected = i_CameraToProjectedMatrix;
}

void eae6320::Graphics::SetCameraToRender(eae6320::Graphics::cCamera* i_Camera, const float i_secondCountToExtrapolate)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);
	auto& constDataBuffer = s_dataBeingSubmittedByApplicationThread->constantData_perFrame;
	constDataBuffer.g_transform_worldToCamera = eae6320::Math::cMatrix_transformation::CreateWorldToCameraTransform(
		i_Camera->PredictFutureOrientation(i_secondCountToExtrapolate), i_Camera->PredictFuturePosition(i_secondCountToExtrapolate));
	constDataBuffer.g_transform_cameraToProjected = eae6320::Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(0.745f, 4 / 3, 0.1f, 100);
}

void eae6320::Graphics::SetGameObjectsToRender(eae6320::Physics::cGameObject i_GameObjects[eae6320::Graphics::m_maxNumberofMeshesAndEffects], unsigned int i_NumberOfEffectsAndMeshesToRender)
{
	EAE6320_ASSERT(i_NumberOfEffectsAndMeshesToRender < m_maxNumberofMeshesAndEffects);
	auto& meshesAndEffects = s_dataBeingSubmittedByApplicationThread->m_MeshesAndEffects;
	s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender = i_NumberOfEffectsAndMeshesToRender;
	auto m_allMeshes = s_dataBeingSubmittedByApplicationThread->m_MeshesAndEffects;
	auto m_allDrawCallConstants = s_dataBeingSubmittedByApplicationThread->constantData_perDrawCall;

	for (unsigned int i = 0; i < (s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender > m_maxNumberofMeshesAndEffects ? m_maxNumberofMeshesAndEffects : s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender); i++)
	{
		meshesAndEffects[i] = i_GameObjects[i].GetMeshEffectPair();
		m_allDrawCallConstants[i].g_transform_localToWorld = eae6320::Math::cMatrix_transformation(i_GameObjects[i].GetGameObjectRotation(), i_GameObjects[i].GetGameObjectPosition());
		meshesAndEffects[i].m_RenderEffect->IncrementReferenceCount();
		meshesAndEffects[i].m_RenderMesh->IncrementReferenceCount();
	}
}

void eae6320::Graphics::SetGameObjectsToRender(eae6320::Physics::cGameObject* i_GameObjects[eae6320::Graphics::m_maxNumberofMeshesAndEffects], unsigned int i_NumberOfEffectsAndMeshesToRender, const float i_secondCountToExtrapolate)
{
	EAE6320_ASSERT(i_NumberOfEffectsAndMeshesToRender < m_maxNumberofMeshesAndEffects);
	auto& meshesAndEffects = s_dataBeingSubmittedByApplicationThread->m_MeshesAndEffects;
	s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender = i_NumberOfEffectsAndMeshesToRender;
	auto m_allMeshes = s_dataBeingSubmittedByApplicationThread->m_MeshesAndEffects;
	auto m_allDrawCallConstants = s_dataBeingSubmittedByApplicationThread->constantData_perDrawCall;

	for (unsigned int i = 0; i < (s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender > m_maxNumberofMeshesAndEffects ? m_maxNumberofMeshesAndEffects : s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender); i++)
	{
		meshesAndEffects[i] = i_GameObjects[i]->GetMeshEffectPair();
		m_allDrawCallConstants[i].g_transform_localToWorld = eae6320::Math::cMatrix_transformation(i_GameObjects[i]->PredictFutureOrientation(i_secondCountToExtrapolate), i_GameObjects[i]->PredictFuturePosition(i_secondCountToExtrapolate));
		meshesAndEffects[i].m_RenderEffect->IncrementReferenceCount();
		meshesAndEffects[i].m_RenderMesh->IncrementReferenceCount();
	}
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


		auto& m_allMeshes = s_dataBeingRenderedByRenderThread->m_MeshesAndEffects;

		if (m_allMeshes != nullptr)
		{
			for (unsigned int i = 0; i < (s_dataBeingRenderedByRenderThread->m_NumberOfEffectsToRender > m_maxNumberofMeshesAndEffects ? m_maxNumberofMeshesAndEffects : s_dataBeingRenderedByRenderThread->m_NumberOfEffectsToRender); i++)
			{
				m_allMeshes[i].m_RenderEffect->Bind();
				s_constantBuffer_perDrawCall.Update(&s_dataBeingRenderedByRenderThread->constantData_perDrawCall[i]);
				m_allMeshes[i].m_RenderMesh->Draw();
			}
		}
		s_helper->SwapChain();

		//CleanUp
		if (m_allMeshes != nullptr)
		{
			for (unsigned int i = 0; i < (s_dataBeingRenderedByRenderThread->m_NumberOfEffectsToRender > m_maxNumberofMeshesAndEffects ? m_maxNumberofMeshesAndEffects : s_dataBeingRenderedByRenderThread->m_NumberOfEffectsToRender); i++)
			{
				m_allMeshes[i].m_RenderEffect->DecrementReferenceCount();
				m_allMeshes[i].m_RenderMesh->DecrementReferenceCount();
			}

		}
		//s_dataBeingRenderedByRenderThread->m_MeshesAndEffects = nullptr;
		s_dataBeingRenderedByRenderThread->m_NumberOfEffectsToRender = 0;
	}
}
eae6320::cResult eae6320::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;
	s_helper = new eae6320::Graphics::GraphicsHelper();

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
	// Initialize the views, Shading  data and Geometry
	result = s_helper->Initialize(i_initializationParameters);

OnExit:

	return result;
}

eae6320::cResult eae6320::Graphics::CleanUp()
{
	auto result = s_helper->CleanUp();

	auto m_allMeshes = s_dataBeingSubmittedByApplicationThread->m_MeshesAndEffects;

	if (m_allMeshes != nullptr)
	{
		for (unsigned int i = 0; i < (s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender > m_maxNumberofMeshesAndEffects ? m_maxNumberofMeshesAndEffects : s_dataBeingSubmittedByApplicationThread->m_NumberOfEffectsToRender); i++)
		{
			m_allMeshes[i].m_RenderEffect->DecrementReferenceCount();
			m_allMeshes[i].m_RenderMesh->DecrementReferenceCount();
		}
	}

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