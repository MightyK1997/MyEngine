// Includes
//=========
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 

#include "cFinalGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/ControllerInput/ControllerInput.h>
#include "Engine/Graphics/sRGB.h"

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
	(*m_PointLight).SetLightVelocity(Math::sVector(0, 0, 0));
	(*m_RenderingCamera).SetAngularSpeed(0.0f);
	(*m_PointLight).SetAngularSpeed(0.0f);
	m_Lambo->SetGameObjectAngularVelocity(0);
	(*m_DirectionalLight).SetAngularSpeed(0.0f);
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
		(*m_Lambo).SetGameObjectAngularVelocity(1.0f);
	}
	if (UserInput::IsKeyPressed('X'))
	{
		(*m_Lambo).SetGameObjectAngularVelocity(-1.0f);
	}
	if (UserInput::IsKeyPressed('U'))
	{
		(*m_DirectionalLight).SetAngularSpeed(1.0f);
	}
	if (UserInput::IsKeyPressed('O'))
	{
		(*m_DirectionalLight).SetAngularSpeed(-1.0f);
	}
	if (UserInput::IsKeyPressed('M'))
	{
		(*m_PointLight).SetLightVelocity(Math::sVector(0, 10.0f, 0));
	}
	if (UserInput::IsKeyPressed('N'))
	{
		(*m_PointLight).SetLightVelocity(Math::sVector(0, -10.0f, 0));
	}
	if (UserInput::IsKeyPressed('K'))
	{
		(*m_PointLight).SetLightVelocity(Math::sVector(0, 0, 10.0f));
	}
	if (UserInput::IsKeyPressed('I'))
	{
		(*m_PointLight).SetLightVelocity(Math::sVector(0, 0, -10.0f));
	}
	if (UserInput::IsKeyPressed('L'))
	{
		(*m_PointLight).SetLightVelocity(Math::sVector(10.0f, 0, 0));
	}
	if (UserInput::IsKeyPressed('J'))
	{
		(*m_PointLight).SetLightVelocity(Math::sVector(-10.0f, 0, 0));
	}
	if (UserInput::IsKeyPressed('G'))
	{
		m_MaterialSmoothness++;
	}
	if (UserInput::IsKeyPressed('H'))
	{
		m_MaterialSmoothness--;
	}
	if (UserInput::IsKeyPressed('B'))
	{
		m_RenderingEnvMapHandle = m_EnvMapHandle2;
	}
}

void eae6320::cFinalGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	static float f = 0;
	static float startTimer = 0;
	f += i_elapsedSecondCount_sinceLastUpdate;
	m_TopDownCamera->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_DirectionalLight->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_PointLight->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_PointLightGameObject->SetGameObjectPosition(m_PointLight->GetLightPosition());
	m_InCarCamera->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_PointLightGameObject->UpdateGameObject(i_elapsedSecondCount_sinceLastUpdate);
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
	SetBackBufferValue(Graphics::Color::ConvertNormalizedsRGBToLinear(Graphics::sColor{ 0,0,0, 1 }));

	m_ListOfGameObjects.clear();
	m_ListOfGameObjects.push_back(m_Lambo);
	m_ListOfGameObjects.push_back(m_PointLightGameObject);
	//m_ListOfGameObjects.push_back(m_Skybox);
	for (auto&x : m_TreeObjs)
	{
		m_ListOfGameObjects.push_back(x);
	}
	m_GameObjectLocalToWorldTransforms.clear();
	m_GameObjectLocalToWorldTransforms.push_back(m_Lambo->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));
	m_GameObjectLocalToWorldTransforms.push_back(m_PointLightGameObject->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));
	//m_GameObjectLocalToWorldTransforms.push_back(m_Skybox->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));
	for (auto&x : m_TreeObjs)
	{
		m_GameObjectLocalToWorldTransforms.push_back(x->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));
	}

	eae6320::Graphics::SetEffectsAndMeshesToRender(&(m_ListOfGameObjects[0]), &(m_GameObjectLocalToWorldTransforms[0]), (uint8_t)(m_ListOfGameObjects.size()),
		m_DirectionalLight,m_PointLight, m_RenderingCamera, i_elapsedSecondCount_sinceLastSimulationUpdate);
	eae6320::Graphics::SumbitMaterialData(m_MaterialSmoothness);
	eae6320::Graphics::SubmitEnvMap(m_RenderingEnvMapHandle);
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cFinalGame::Initialize()
{
	eae6320::Graphics::cCamera::CreateCamera(m_TopDownCamera);
	eae6320::Graphics::cCamera::CreateCamera(m_InCarCamera);
	eae6320::Physics::cGameObject::CreateGameObject(m_Lambo);
	eae6320::Physics::cGameObject::CreateGameObject(m_PointLightGameObject);
	eae6320::Physics::cGameObject::CreateGameObject(m_Skybox);
	eae6320::Graphics::cLight::CreateDirectionalLight(m_DirectionalLight); 
	eae6320::Graphics::cLight::CreateDirectionalLight(m_PointLight);

	m_DirectionalLight->SetLightPosition(Math::sVector(0, 0, 0));
	m_DirectionalLight->SetLightRotation(Math::cQuaternion(-0.02f, 0.0f, -0.9f,0.0f));
	m_DirectionalLight->SetLightColor({ 1,1,1,1 });



	m_PointLight->SetLightPosition(Math::sVector(0, 0, -10));
	m_PointLight->SetLightColor({ 1,1,1,1 });

	m_Lambo->SetGameObjectPosition(Math::sVector(0, 0, -10));
	m_PointLightGameObject->SetGameObjectPosition(Math::sVector(0, 0, -10));
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

	std::string fName = "data/Meshes/Gecko.meshbinary";
	eae6320::Graphics::cMesh::s_Manager.Load(fName, m_TreeHandle);

	fName = "data/Meshes/Plane.meshbinary";
	eae6320::Graphics::cMesh::s_Manager.Load(fName, m_LamboHandle);

	fName = "data/Meshes/PointLight.meshbinary";
	eae6320::Graphics::cMesh::s_Manager.Load(fName, m_PointLightMeshHandle);

	fName = "data/Meshes/skybox.meshbinary";
	eae6320::Graphics::cMesh::s_Manager.Load(fName, m_SkyboxMeshHandle);

	fName = "data/Materials/GeckoMaterial.materialbinary";
	eae6320::Graphics::cMaterial::s_Manager.Load(fName, m_Material1Handle);

	fName = "data/Materials/material1.materialbinary";
	eae6320::Graphics::cMaterial::s_Manager.Load(fName, m_Material2Handle);

	fName = "data/Materials/gold.materialbinary";
	eae6320::Graphics::cMaterial::s_Manager.Load(fName, m_LamboMaterialHandle);

	fName = "data/Materials/skybox.materialbinary";
	eae6320::Graphics::cMaterial::s_Manager.Load(fName, m_SkyboxMaterialHandle);

	fName = "data/envmaps/yokohoma.dds";
	eae6320::Graphics::cTexture::s_manager.Load(fName, m_EnvMapHandle);

	fName = "data/envmaps/yokohoma2.dds";
	eae6320::Graphics::cTexture::s_manager.Load(fName, m_EnvMapHandle2);

	m_RenderingEnvMapHandle = m_EnvMapHandle2;

	UpdateMeshAndEffect();

	return Results::Success;
}

eae6320::cResult eae6320::cFinalGame::CleanUp()
{
	eae6320::Graphics::cMesh::s_Manager.Release(m_TreeHandle);
	eae6320::Graphics::cMesh::s_Manager.Release(m_LamboHandle);
	eae6320::Graphics::cMesh::s_Manager.Release(m_PointLightMeshHandle);
	eae6320::Graphics::cMaterial::s_Manager.Release(m_Material1Handle);
	eae6320::Graphics::cMaterial::s_Manager.Release(m_Material2Handle);
	eae6320::Graphics::cMaterial::s_Manager.Release(m_LamboMaterialHandle);
	eae6320::Graphics::cTexture::s_manager.Release(m_EnvMapHandle);

	for (auto&x : m_TreeObjs)
	{
		x->DecrementReferenceCount();
	}

	m_Lambo->DecrementReferenceCount();
	m_PointLightGameObject->DecrementReferenceCount();
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
	m_Lambo->SetGameObjectHandles(m_LamboHandle, m_LamboMaterialHandle);
	m_PointLightGameObject->SetGameObjectHandles(m_PointLightMeshHandle, m_Material2Handle);
	m_Skybox->SetGameObjectHandles(m_SkyboxMeshHandle, m_SkyboxMaterialHandle);
	for (auto&treeObj : m_TreeObjs)
	{
		treeObj->SetGameObjectHandles(m_TreeHandle, m_Material1Handle);
	}
}