// Includes
//=========

#include "cMyGame.h"
#include <Engine/Graphics/Graphics.h>
#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cMyGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
	Application::cbApplication::SetSimulationRate(UserInput::IsKeyPressed(UserInput::KeyCodes::Shift) ? 0.5f : 1.0f);
	m_NumberOfMeshesToRender = UserInput::IsKeyPressed(UserInput::KeyCodes::F1) ? 1 : 2;
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::F2))
	{
		std::swap(s_Effect, s_Effect2);
	}
}

void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
}

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	//Submit The value of Backbuffer
	eae6320::Graphics::SetBackBufferValue(eae6320::Graphics::sColor
		{
			abs(sin(i_elapsedSecondCount_systemTime)),
			abs(cos(i_elapsedSecondCount_systemTime)) ,
			abs(cos(i_elapsedSecondCount_systemTime)) ,
			1
		});
	m_EffectsAndMeshes[0].m_RenderEffect = s_Effect;
	m_EffectsAndMeshes[0].m_RenderMesh = s_Mesh;
	m_EffectsAndMeshes[1].m_RenderEffect = s_Effect2;
	m_EffectsAndMeshes[1].m_RenderMesh = s_Mesh2;

	eae6320::Graphics::SetEffectsAndMeshesToRender(m_EffectsAndMeshes, m_NumberOfMeshesToRender);
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{
	std::string m_vertShader1Location = "data/Shaders/Vertex/standard.shader";
	std::string m_fragShader1Location = "data/Shaders/Fragment/animatedshader.shader";
	std::string m_vertShader2Location = "data/Shaders/Vertex/standard.shader";
	std::string m_fragShader2Location = "data/Shaders/Fragment/standard.shader";
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
		indexData[3].indexValue = 1;
		indexData[4].indexValue = 0;
		indexData[5].indexValue = 3;
		indexData[6].indexValue = 4;
		indexData[7].indexValue = 1;
		indexData[8].indexValue = 3;
	}
	eae6320::Graphics::VertexFormats::sMesh vertexData2[8];
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
	if (!(eae6320::Graphics::cEffect::CreateEffect(m_vertShader1Location, m_fragShader1Location, s_Effect)))
	{
		EAE6320_ASSERT(false);
		//goto OnExit;
	}
	if (!(eae6320::Graphics::cEffect::CreateEffect(m_vertShader2Location, m_fragShader2Location, s_Effect2)))
	{
		EAE6320_ASSERT(false);
		//goto OnExit;
	}
	if (!(eae6320::Graphics::cMesh::CreateMesh(vertexData, indexData, 9, s_Mesh)))
	{
		EAE6320_ASSERT(false);
		//goto OnExit;
	}
	if (!(eae6320::Graphics::cMesh::CreateMesh(vertexData2, indexData2, 12, s_Mesh2)))
	{
		EAE6320_ASSERT(false);
		//goto OnExit;
	}
	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	s_Effect->DecrementReferenceCount();
	s_Effect2->DecrementReferenceCount();
	s_Mesh->DecrementReferenceCount();
	s_Mesh2->DecrementReferenceCount();
	return Results::Success;
}
