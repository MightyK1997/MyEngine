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

void eae6320::cMyGame::UpdateCameraPosition()
{
	m_Camera->SetCameraVelocity(Math::sVector(0, 0, 0));

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
	{
		m_Camera->SetCameraVelocity(Math::sVector(0, 0, 10.0f));
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
	{
		m_Camera->SetCameraVelocity(Math::sVector(0, 0, -10.0f));
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		m_Camera->SetCameraVelocity(Math::sVector(10.0f, 0, 0));
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
	{
		m_Camera->SetCameraVelocity(Math::sVector(-10.0f, 0, 0));
	}
}

void eae6320::cMyGame::UpdateGameobjectPosition()
{
	for (int i = 0; i < m_NumberOfGameObjects; i++)
	{
		m_GameObjects[i]->SetGameObjectVelocity(Math::sVector(0, 0, 0));
	}
//GO1
	if (UserInput::IsKeyPressed('W'))
	{
		m_GameObjects[0]->SetGameObjectVelocity(Math::sVector(0, 10.0f, 0.0f));
	}
	if (UserInput::IsKeyPressed('A'))
	{
		m_GameObjects[0]->SetGameObjectVelocity(Math::sVector(-10.0f, 0, 0));
	}
	if (UserInput::IsKeyPressed('S'))
	{
		m_GameObjects[0]->SetGameObjectVelocity(Math::sVector(0,-10.0f,0));
	}
	if (UserInput::IsKeyPressed('D'))
	{
		m_GameObjects[0]->SetGameObjectVelocity(Math::sVector(10.0f,0,0));
	}

//GO2
	if (UserInput::IsKeyPressed('I'))
	{
		m_GameObjects[1]->SetGameObjectVelocity(Math::sVector(0, 10.0f, 0.0f));
	}
	if (UserInput::IsKeyPressed('J'))
	{
		m_GameObjects[1]->SetGameObjectVelocity(Math::sVector(-10.0f, 0, 0));
	}
	if (UserInput::IsKeyPressed('K'))
	{
		m_GameObjects[1]->SetGameObjectVelocity(Math::sVector(0, -10.0f, 0));
	}
	if (UserInput::IsKeyPressed('L'))
	{
		m_GameObjects[1]->SetGameObjectVelocity(Math::sVector(10.0f, 0, 0));
	}
}


void eae6320::cMyGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Escape))
	{
		// Exit the application
		const auto result = Exit(EXIT_SUCCESS);
		EAE6320_ASSERT(result);
	}
	Application::cbApplication::SetSimulationRate(UserInput::IsKeyPressed(UserInput::KeyCodes::Shift) ? 0.5f : 1.0f);
	m_NumberOfMeshesToRender = UserInput::IsKeyPressed(UserInput::KeyCodes::F1) ? 1 : 2;
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::F2))
	{
		if (!isEffectSwapped)
		{
			std::swap(s_Effect, s_Effect2);
			isEffectSwapped = true;
		}
	}
	else
	{
		if (isEffectSwapped)
		{
			std::swap(s_Effect, s_Effect2);
			isEffectSwapped = false;
		}
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::F3))
	{
		if (!isMeshSwapped)
		{
			std::swap(s_Mesh, s_Mesh3);
			isMeshSwapped = true;
		}
	}
	else
	{
		if (isMeshSwapped)
		{
			std::swap(s_Mesh, s_Mesh3);
			isMeshSwapped = false;
		}
	}
	UpdateCameraPosition();
	UpdateGameobjectPosition();
}

void eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_Camera->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_GameObjects[0]->UpdateGameObject(i_elapsedSecondCount_sinceLastUpdate);
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


	m_GameObjects[0]->SetGameObjectEffect(s_Effect);
	m_GameObjects[0]->SetGameObjectMesh(s_Mesh);
	m_GameObjects[1]->SetGameObjectEffect(s_Effect2);
	m_GameObjects[1]->SetGameObjectMesh(s_Mesh2);

	m_EffectsAndMeshes[0] = m_GameObjects[0]->GetMeshEffectPair();
	m_EffectsAndMeshes[1] = m_GameObjects[1]->GetMeshEffectPair();

	for (size_t i = 0; i < m_NumberOfGameObjects; i++)
	{
		m_GameObjectLocalToWorldTransforms[i] = 
			eae6320::Math::cMatrix_transformation(m_GameObjects[i]->PredictFutureOrientation(i_elapsedSecondCount_sinceLastSimulationUpdate), m_GameObjects[i]->PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate));
	}

	eae6320::Graphics::SetCameraToRender(m_Camera, i_elapsedSecondCount_sinceLastSimulationUpdate);

	eae6320::Graphics::SetEffectsAndMeshesToRender(m_EffectsAndMeshes, m_GameObjectLocalToWorldTransforms, m_NumberOfMeshesToRender);
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{
	std::string m_vertShader1Location = "data/Shaders/Vertex/standard.shader";
	std::string m_fragShader1Location = "data/Shaders/Fragment/animatedshader.shader";
	std::string m_vertShader2Location = "data/Shaders/Vertex/standard.shader";
	std::string m_fragShader2Location = "data/Shaders/Fragment/standard.shader";

	eae6320::Graphics::cCamera::CreateCamera(m_Camera);

	m_Camera->SetCameraPosition(Math::sVector(0, 0, 10));

	eae6320::Physics::cGameObject::CreateGameObject(m_GameObjects[0]);
	eae6320::Physics::cGameObject::CreateGameObject(m_GameObjects[1]);

	m_GameObjects[0]->SetGameObjectPosition(Math::sVector(0, 0, 0));

	m_GameObjects[1]->SetGameObjectPosition(Math::sVector(0, 0, 0));

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
	eae6320::Graphics::VertexFormats::sIndex indexData[6];
	{
		indexData[0].indexValue = 1;
		indexData[1].indexValue = 0;
		indexData[2].indexValue = 3;
		indexData[3].indexValue = 4;
		indexData[4].indexValue = 1;
		indexData[5].indexValue = 3;
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
	eae6320::Graphics::VertexFormats::sIndex indexData2[3];
	{
		indexData2[0].indexValue = 0;
		indexData2[1].indexValue = 1;
		indexData2[2].indexValue = 2;
		//indexData2[3].indexValue = 0;
		//indexData2[4].indexValue = 2;
		//indexData2[5].indexValue = 3;
		//indexData2[6].indexValue = 3;
		//indexData2[7].indexValue = 4;
		//indexData2[8].indexValue = 5;
		//indexData2[9].indexValue = 7;
		//indexData2[10].indexValue = 2;
		//indexData2[11].indexValue = 6;
	}

	eae6320::Graphics::VertexFormats::sIndex indexData3[12];
	{
		indexData3[0].indexValue = 0;
		indexData3[1].indexValue = 1;
		indexData3[2].indexValue = 2;
		indexData3[3].indexValue = 0;
		indexData3[4].indexValue = 2;
		indexData3[5].indexValue = 3;
		indexData3[6].indexValue = 3;
		indexData3[7].indexValue = 4;
		indexData3[8].indexValue = 5;
		indexData3[9].indexValue = 7;
		indexData3[10].indexValue = 2;
		indexData3[11].indexValue = 6;
	}
	if (!(eae6320::Graphics::cEffect::CreateEffect(m_vertShader1Location, m_fragShader1Location, eae6320::Graphics::RenderStates::DepthBuffering, s_Effect)))
	{
		EAE6320_ASSERT(false);
		//goto OnExit;
	}
	if (!(eae6320::Graphics::cEffect::CreateEffect(m_vertShader2Location, m_fragShader2Location, eae6320::Graphics::RenderStates::DepthBuffering, s_Effect2)))
	{
		EAE6320_ASSERT(false);
		//goto OnExit;
	}
	if (!(eae6320::Graphics::cEffect::CreateEffect(m_vertShader2Location, m_fragShader2Location, eae6320::Graphics::RenderStates::DepthBuffering, s_Effect3)))
	{
		EAE6320_ASSERT(false);
		//goto OnExit;
	}
	if (!(eae6320::Graphics::cMesh::CreateMesh(vertexData, indexData, 6, s_Mesh)))
	{
		EAE6320_ASSERT(false);
		//goto OnExit;
	}
	if (!(eae6320::Graphics::cMesh::CreateMesh(vertexData2, indexData2, 3, s_Mesh2)))
	{
		EAE6320_ASSERT(false);
		//goto OnExit;
	}
	if (!(eae6320::Graphics::cMesh::CreateMesh(vertexData2, indexData3, 12, s_Mesh3)))
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
	s_Effect3->DecrementReferenceCount();
	s_Mesh3->DecrementReferenceCount();
	for (int i = 0; i < m_NumberOfGameObjects; i++)
	{
		m_GameObjects[i]->DecrementReferenceCount();
	}
	m_Camera->DecrementReferenceCount();
	return Results::Success;
}
