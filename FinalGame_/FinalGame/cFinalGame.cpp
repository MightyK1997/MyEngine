// Includes
//=========

#include "cFinalGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/ControllerInput/ControllerInput.h>

// Inherited Implementation
//=========================

// Run
//----

using namespace eae6320::UserInput::ControllerInput;
void eae6320::cFinalGame::UpdateCameraPosition()
{
	//m_TopDownCamera->SetCameraVelocity(Math::sVector(0, 0, 0));
	//m_TopDownCamera->SetAngularSpeed(0.0f);

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
	{
		m_TopDownCamera->SetCameraVelocity(Math::sVector(0, 0, 10.0f));
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
	{
		m_TopDownCamera->SetCameraVelocity(Math::sVector(0, 0, -10.0f));
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		m_TopDownCamera->SetCameraVelocity(Math::sVector(10.0f, 0, 0));
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
	{
		m_TopDownCamera->SetCameraVelocity(Math::sVector(-10.0f, 0, 0));
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Control))
	{
		m_TopDownCamera->SetCameraVelocity(Math::sVector(0, 10.0f, 0));
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Alt))
	{
		m_TopDownCamera->SetCameraVelocity(Math::sVector(0, -10.0f, 0));
	}
	if (UserInput::IsKeyPressed('Z'))
	{
		m_TopDownCamera->SetAngularSpeed(1.0f);
	}
	if (UserInput::IsKeyPressed('X'))
	{
		m_TopDownCamera->SetAngularSpeed(-1.0f);
	}
}

void eae6320::cFinalGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}

	UpdateCarPosition();
	UpdateCameraPosition();
}

void eae6320::cFinalGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_TopDownCamera->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_InCarCamera->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_Car->UpdateGameObject(i_elapsedSecondCount_sinceLastUpdate);
}

void eae6320::cFinalGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	eae6320::Graphics::SetBackBufferValue(eae6320::Graphics::sColor{ 1,1,1, 1 });
	m_EffectsAndMeshes.clear();
	m_EffectsAndMeshes.push_back(m_Car->GetMeshEffectPair());
	m_EffectsAndMeshes.push_back(m_RaceTrackObj->GetMeshEffectPair());
	m_EffectsAndMeshes.push_back(m_TrafficLightObj->GetMeshEffectPair());


	m_GameObjectLocalToWorldTransforms.clear();
	m_GameObjectLocalToWorldTransforms.push_back(m_Car->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));
	m_GameObjectLocalToWorldTransforms.push_back(m_RaceTrackObj->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));
	m_GameObjectLocalToWorldTransforms.push_back(m_TrafficLightObj->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));


	eae6320::Graphics::SetCameraToRender(m_TopDownCamera, i_elapsedSecondCount_sinceLastSimulationUpdate);
	eae6320::Graphics::SetEffectsAndMeshesToRender(&(m_EffectsAndMeshes[0]), &(m_GameObjectLocalToWorldTransforms[0]), 2);
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cFinalGame::Initialize()
{
	eae6320::Graphics::cCamera::CreateCamera(m_TopDownCamera);
	eae6320::Graphics::cCamera::CreateCamera(m_InCarCamera);

	eae6320::Physics::cGameObject::CreateGameObject(m_Car);
	eae6320::Physics::cGameObject::CreateGameObject(m_RaceTrackObj);
	eae6320::Physics::cGameObject::CreateGameObject(m_TrafficLightObj);

	m_Car->SetGameObjectPosition(Math::sVector(-4, -9.5f, -15));
	m_RaceTrackObj->SetGameObjectPosition(Math::sVector(0, -10, -210));
	m_TrafficLightObj->SetGameObjectPosition(Math::sVector(0, -10, -25));

	std::string fName = "data/Meshes/Lambo.meshbinary";
	eae6320::Graphics::cMesh::s_Manager.Load(fName.c_str(), m_PlayerCarHandle);
	m_CarHandles.push_back(m_PlayerCarHandle);
	fName = "data/Meshes/LP_Car1.meshbinary";
	eae6320::Graphics::cMesh::s_Manager.Load(fName.c_str(), m_PlayerCarHandle);
	m_CarHandles.push_back(m_PlayerCarHandle);
	fName = "data/Meshes/SmartCar.meshbinary";
	eae6320::Graphics::cMesh::s_Manager.Load(fName.c_str(), m_PlayerCarHandle);
	m_CarHandles.push_back(m_PlayerCarHandle);
	fName = "data/Meshes/RaceTrack.meshbinary";
	eae6320::Graphics::cMesh::s_Manager.Load(fName.c_str(), m_RaceTrack);
	fName = "data/Meshes/TrafficLight.meshbinary";
	eae6320::Graphics::cMesh::s_Manager.Load(fName.c_str(), m_TrafficLight);


	std::string effectPath = "data/Effects/Effect1.effectbinary";
	eae6320::Graphics::cEffect::s_Manager.Load(effectPath.c_str(), effect1Handle);

	m_PlayerCarHandle = m_CarHandles[1];

	m_Car->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_PlayerCarHandle));
	m_Car->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));

	m_RaceTrackObj->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_RaceTrack));
	m_RaceTrackObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));

	m_TrafficLightObj->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_TrafficLight));
	m_TrafficLightObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));

	m_TopDownCamera->SetCameraPosition(Math::sVector(-3.4f, 1.4f, -2));
	m_TopDownCamera->SetCameraRotation(Math::cQuaternion(0.4f, Math::sVector(-1,0,0)));

	return Results::Success;
}

eae6320::cResult eae6320::cFinalGame::CleanUp()
{
	return Results::Success;
}

void eae6320::cFinalGame::UpdateCarPosition()
{
	if (GetNormalizedStickDeflection(ControllerKeyCodes::RIGHT_STICK, 0).x != 0)
	{
		m_TopDownCamera->SetAngularSpeed(GetNormalizedStickDeflection(ControllerKeyCodes::RIGHT_STICK, 0).x);
		m_InCarCamera->SetAngularSpeed(GetNormalizedStickDeflection(ControllerKeyCodes::RIGHT_STICK, 0).x);
	}
	if (IsKeyPressed(ControllerKeyCodes::RIGHT_TRIGGER) || (UserInput::IsKeyPressed('W')))
	{
		float val = 10;
		if (!(GetNumberOfConnectedControllers() == 0))
		{
			val = GetNormalizedTriggerDeflection(ControllerKeyCodes::RIGHT_TRIGGER);
		}
		m_TopDownCamera->SetCameraVelocity(Math::sVector(0, 0, -val));
		m_InCarCamera->SetCameraVelocity(Math::sVector(0, 0, -val));
		m_Car->SetGameObjectVelocity(Math::sVector(0,0, -val));
	}
	if (IsKeyPressed(ControllerKeyCodes::LEFT_TRIGGER) || (UserInput::IsKeyPressed('S')))
	{
		m_TopDownCamera->SetCameraVelocity(Math::sVector(0, -GetNormalizedTriggerDeflection(ControllerKeyCodes::LEFT_TRIGGER), 0));
		m_InCarCamera->SetCameraVelocity(Math::sVector(0, -GetNormalizedTriggerDeflection(ControllerKeyCodes::LEFT_TRIGGER), 0));
		m_Car->SetGameObjectVelocity(Math::sVector(0, GetNormalizedTriggerDeflection(ControllerKeyCodes::LEFT_TRIGGER) * -10, 0));
	}
}
