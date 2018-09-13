
#include <cmath>
#include "Graphics.h"
#include "GraphicsHelper.h"
#include "cMesh.h"
#include "cEffect.h"

//#include <Engine/Asserts/Asserts.h>
eae6320::Graphics::cMesh* eae6320::Graphics::s_Mesh;
eae6320::Graphics::cMesh* eae6320::Graphics::s_Mesh2;
eae6320::Graphics::cEffect* eae6320::Graphics::s_Effect;
eae6320::Graphics::cEffect* eae6320::Graphics::s_Effect2;
eae6320::Graphics::sDataRequiredToRenderAFrame* eae6320::Graphics::m_dataRequiredToRenderAFrame;
eae6320::Graphics::cConstantBuffer eae6320::Graphics::s_constantBuffer_perFrame(eae6320::Graphics::ConstantBufferTypes::PerFrame);
namespace
{
	struct sDataRequiredToRenderAFrame
	{
		eae6320::Graphics::ConstantBufferFormats::sPerFrame constantData_perFrame;
	};
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

//namespace
//{
//	eae6320::cResult InitializeGeometry();
//	eae6320::cResult InitializeShadingData();
//}

// Interface
//==========

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

void eae6320::Graphics::RenderFrame()
{
	// Wait for the application loop to submit data to be rendered
	{
		sBackBuffer m_BackBuffer{ abs(sin(s_dataBeingSubmittedByApplicationThread->constantData_perFrame.g_elapsedSecondCount_simulationTime)), abs(cos(s_dataBeingSubmittedByApplicationThread->constantData_perFrame.g_elapsedSecondCount_simulationTime)) , abs(cos(s_dataBeingSubmittedByApplicationThread->constantData_perFrame.g_elapsedSecondCount_simulationTime)) , 1};
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
		s_helper->SetBackBuffer(m_BackBuffer);
		s_helper->RenderFrame();
	}
}
eae6320::cResult eae6320::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;
	s_helper = new eae6320::Graphics::GraphicsHelper();
	s_Mesh = new eae6320::Graphics::cMesh();
	s_Mesh2 = new eae6320::Graphics::cMesh();
	s_Effect = new eae6320::Graphics::cEffect();
	s_Effect2 = new eae6320::Graphics::cEffect();
	std::string m_vertShader1Location = "data/Shaders/Vertex/standard.shader";
	std::string m_fragShader1Location = "data/Shaders/Fragment/animatedshader.shader";
	std::string m_vertShader2Location = "data/Shaders/Vertex/standard.shader";
	std::string m_fragShader2Location = "data/Shaders/Fragment/standard.shader";
	size_t sizeOfMesh = sizeof(eae6320::Graphics::cMesh);
	size_t sizeOfEffect = sizeof(eae6320::Graphics::cEffect);

	eae6320::Graphics::VertexFormats::sMesh vertexData[5];
	{
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
		vertexData[4].y = -0.5f;
		vertexData[4].z = 0.0f;
	}
	eae6320::Graphics::cMesh::sIndex indexData[9];
	{
		indexData[0].indexValue = 0;
		indexData[1].indexValue = 1;
		indexData[2].indexValue = 2;
		indexData[3].indexValue = 3;
		indexData[4].indexValue = 1;
		indexData[5].indexValue = 0;
		indexData[6].indexValue = 3;
		indexData[7].indexValue = 4;
		indexData[8].indexValue = 1;
	}
	eae6320::Graphics::VertexFormats::sMesh vertexData2[10];
	{
		vertexData2[0].x = -0.5f;
		vertexData2[0].y = -0.5f;
		vertexData2[0].z = 0.0f;

		vertexData2[1].x = -0.25f;
		vertexData2[1].y = -0.5f;
		vertexData2[1].z = 0.0f;

		vertexData2[2].x = -0.25f;
		vertexData2[2].y = 0.25f;
		vertexData2[2].z = 0.0f;

		vertexData2[3].x = -0.5f;
		vertexData2[3].y = 0.25f;
		vertexData2[3].z = 0.0f;

		vertexData2[4].x = -0.5f;
		vertexData2[4].y = 0.75f;
		vertexData2[4].z = 0.0f;

		vertexData2[5].x = -0.75f;
		vertexData2[5].y = 0.75f;
		vertexData2[5].z = 0.0f;

		vertexData2[6].x = 0.0f;
		vertexData2[6].y = 0.75f;
		vertexData2[6].z = 0.0f;

		vertexData2[7].x = -0.25f;
		vertexData2[7].y = 0.75f;
		vertexData2[7].z = 0.0f;

	}
	eae6320::Graphics::cMesh::sIndex indexData2[12];
	{
		indexData2[0].indexValue = 0;
		indexData2[1].indexValue = 1;
		indexData2[2].indexValue = 2;
		indexData2[3].indexValue = 0;
		indexData2[4].indexValue = 2;
		indexData2[5].indexValue = 3;
		indexData2[6].indexValue = 3;
		indexData2[7].indexValue = 4;
		indexData2[8].indexValue = 5;
		indexData2[9].indexValue = 7;
		indexData2[10].indexValue = 2;
		indexData2[11].indexValue = 6;
	}
	m_dataRequiredToRenderAFrame = reinterpret_cast<eae6320::Graphics::sDataRequiredToRenderAFrame*>(s_dataRequiredToRenderAFrame);

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
	s_Effect->Initialize(m_vertShader1Location, m_fragShader1Location);
	s_Effect2->Initialize(m_vertShader2Location, m_fragShader2Location);
	s_Mesh->Initialize(vertexData, indexData, 3);
	s_Mesh2->Initialize(vertexData2, indexData2, 4);
	

OnExit:

return result;
}

eae6320::cResult eae6320::Graphics::CleanUp()
{
	auto result = s_helper->CleanUp();
	delete(s_helper);
	return result;
}