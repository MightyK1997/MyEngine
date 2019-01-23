// Includes
//=========
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 

#include "cFinalGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/ControllerInput/ControllerInput.h>

// Inherited Implementation
//=========================

// Run
//----

using namespace eae6320::UserInput::ControllerInput;

void eae6320::cFinalGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Escape))
	{
		// Exit the application
		const auto result = Exit(EXIT_SUCCESS);
		EAE6320_ASSERT(result);
	}
	(*m_RenderingCamera).SetCameraVelocity(Math::sVector(0, 0, 0));
	(*m_RenderingCamera).SetAngularSpeed(0.0f);
	if (UserInput::IsKeyPressed('S'))
	{
		(*m_RenderingCamera).SetCameraVelocity(Math::sVector(0, 0, 10.0f));
	}
	if (UserInput::IsKeyPressed('W'))
	{
		(*m_RenderingCamera).SetCameraVelocity(Math::sVector(0, 0, -10.0f));
	}
	if (UserInput::IsKeyPressed('D'))
	{
		(*m_RenderingCamera).SetCameraVelocity(Math::sVector(10.0f, 0, 0));
	}
	if (UserInput::IsKeyPressed('A'))
	{
		(*m_RenderingCamera).SetCameraVelocity(Math::sVector(-10.0f, 0, 0));
	}
	if (UserInput::IsKeyPressed('Q'))
	{
		(*m_RenderingCamera).SetCameraVelocity(Math::sVector(0, 10.0f, 0));
	}
	if (UserInput::IsKeyPressed('E'))
	{
		(*m_RenderingCamera).SetCameraVelocity(Math::sVector(0, -10.0f, 0));
	}
	if (UserInput::IsKeyPressed('Z'))
	{
		(*m_RenderingCamera).SetAngularSpeed(1.0f);
	}
	if (UserInput::IsKeyPressed('X'))
	{
		(*m_RenderingCamera).SetAngularSpeed(-1.0f);
	}
	m_TreeObjs[1]->SetGameObjectVelocity(Math::sVector(0, 0, 0));
	//(*m_RenderingCamera).SetAngularSpeed(0.0f);
	if (UserInput::IsKeyPressed('K'))
	{
		m_TreeObjs[1]->SetGameObjectVelocity(Math::sVector(0, 0, 10.0f));
	}
	if (UserInput::IsKeyPressed('I'))
	{
		m_TreeObjs[1]->SetGameObjectVelocity(Math::sVector(0, 0, -10.0f));
	}
	if (UserInput::IsKeyPressed('L'))
	{
		m_TreeObjs[1]->SetGameObjectVelocity(Math::sVector(10.0f, 0, 0));
	}
	if (UserInput::IsKeyPressed('J'))
	{
		m_TreeObjs[1]->SetGameObjectVelocity(Math::sVector(-10.0f, 0, 0));
	}
	if (UserInput::IsKeyPressed('U'))
	{
		m_TreeObjs[1]->SetGameObjectVelocity(Math::sVector(0, 10.0f, 0));
	}
	if (UserInput::IsKeyPressed('O'))
	{
		m_TreeObjs[1]->SetGameObjectVelocity(Math::sVector(0, -10.0f, 0));
	}
	//if (UserInput::IsKeyPressed('Z'))
	//{
	//	m_TreeObjs[1]->S(1.0f);
	//}
	//if (UserInput::IsKeyPressed('X'))
	//{
	//	(*m_RenderingCamera).SetAngularSpeed(-1.0f);
	//}
}

void eae6320::cFinalGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	static float f = 0;
	static float startTimer = 0;
	f += i_elapsedSecondCount_sinceLastUpdate;
	m_TopDownCamera->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_InCarCamera->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_TreeObjs[1]->UpdateGameObject(i_elapsedSecondCount_sinceLastUpdate);
	if (IsKeyPressed(ControllerKeyCodes::RIGHT_SHOULDER) || UserInput::IsKeyPressed('C'))
	{
		if (f > 0.25f)
		{
			if (m_IsCameraSwitched)
			{
				m_RenderingCamera = m_TopDownCamera;
			}
			else
			{
				m_RenderingCamera = m_InCarCamera;
			}
			m_IsCameraSwitched = !m_IsCameraSwitched;
			f = 0;
		}
	}
}

void eae6320::cFinalGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	eae6320::Graphics::SetBackBufferValue(eae6320::Graphics::sColor{ 1,1,1, 1 });

	m_ListOfGameObjects.clear();
	for (auto&x : m_TreeObjs)
	{
		m_ListOfGameObjects.push_back(x);
	}
	m_GameObjectLocalToWorldTransforms.clear();
	for (auto&x : m_TreeObjs)
	{
		m_GameObjectLocalToWorldTransforms.push_back(x->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));
	}

	eae6320::Graphics::SetEffectsAndMeshesToRender(&(m_ListOfGameObjects[0]), &(m_GameObjectLocalToWorldTransforms[0]), (uint8_t)(m_ListOfGameObjects.size()),
		m_RenderingCamera, i_elapsedSecondCount_sinceLastSimulationUpdate);
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cFinalGame::Initialize()
{
	eae6320::Graphics::cCamera::CreateCamera(m_TopDownCamera);
	eae6320::Graphics::cCamera::CreateCamera(m_InCarCamera);

	m_RenderingCamera = m_TopDownCamera;

	for (size_t i = 1; i < 7; i++)
	{
		eae6320::Physics::cGameObject* temp;
		eae6320::Physics::cGameObject::CreateGameObject(temp);
		if (i <= 3)
		{
			temp->SetGameObjectPosition(Math::sVector(-9.0f, -10, i* -25.0f));
			m_TreeObjs.push_back(temp);
		}
		else
		{
			temp->SetGameObjectPosition(Math::sVector(-9.0f, -10, (i - 2)* -10.0f));
			m_TreeObjs.push_back(temp);
		}		
	}

	ResetDetails();

	std::string fName = "data/Meshes/Tree.meshbinary";
	eae6320::Graphics::cMesh::s_Manager.Load(fName, m_TreeHandle);

	std::string effectPath = "data/Effects/Effect1.effectbinary";
	eae6320::Graphics::cEffect::s_Manager.Load(effectPath, effect1Handle);
	effectPath = "data/Effects/Effect1.effectbinary";
	eae6320::Graphics::cEffect::s_Manager.Load(effectPath, effect2Handle);

	UpdateMeshAndEffect();

	return Results::Success;
}

eae6320::cResult eae6320::cFinalGame::CleanUp()
{
	eae6320::Graphics::cEffect::s_Manager.Release(effect1Handle);
	eae6320::Graphics::cEffect::s_Manager.Release(effect2Handle);
	eae6320::Graphics::cMesh::s_Manager.Release(m_TreeHandle);

	for (auto&x : m_TreeObjs)
	{
		x->DecrementReferenceCount();
	}

	m_TopDownCamera->DecrementReferenceCount();
	m_InCarCamera->DecrementReferenceCount();

	return Results::Success;
}

void eae6320::cFinalGame::ResetDetails()
{
	m_TopDownCamera->SetCameraPosition(Math::sVector(0, 0, 0));
	m_InCarCamera->SetCameraPosition(Math::sVector(-4.6f, -8.1f, -25.5f));
	m_TopDownCamera->SetCameraVelocity(Math::sVector(0, 0, 0));
	m_InCarCamera->SetCameraVelocity(Math::sVector(0, 0, 0));
	m_TopDownCamera->SetCameraAcceleration(Math::sVector(0, 0, 0));
	m_InCarCamera->SetCameraAcceleration(Math::sVector(0, 0, 0));
}

void eae6320::cFinalGame::UpdateMeshAndEffect()
{

	int count = 1;
	for (auto&treeObj : m_TreeObjs)
	{
		if (count <= 3)
		{
			treeObj->SetGameObjectHandles(m_TreeHandle, effect2Handle);
		}
		else
		{
			treeObj->SetGameObjectHandles(m_TreeHandle, effect1Handle);
		}
		count++;
	}
}