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
	static float f = 0;
	static float startTimer = 0;
	f += i_elapsedSecondCount_sinceLastUpdate;
	m_TopDownCamera->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_InCarCamera->Update(i_elapsedSecondCount_sinceLastUpdate);
	m_Car->UpdateGameObject(i_elapsedSecondCount_sinceLastUpdate);
	if (m_IsCarMeshSwitched)
	{
		m_CarMeshChangeTimer += i_elapsedSecondCount_sinceLastUpdate;
		if (m_CarMeshChangeTimer >= 2.f)
		{
			m_CarMeshChangeTimer = 0.f;
			m_IsCarMeshSwitched = false;
		}
	}
	if (m_IsGameStarted)
	{
		startTimer += i_elapsedSecondCount_sinceLastUpdate;
		if (startTimer > 2.f)
		{
			m_CanTakeInput = true;
			m_IsGameStarted = false;
			startTimer = 0;
		}
	}
	if (IsKeyPressed(ControllerKeyCodes::RIGHT_SHOULDER) || UserInput::IsKeyPressed('K'))
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
	m_EffectsAndMeshes.clear();
	m_EffectsAndMeshes.push_back(m_Car->GetMeshEffectPair());
	m_EffectsAndMeshes.push_back(m_NPCCar->GetMeshEffectPair());
	m_EffectsAndMeshes.push_back(m_RaceTrackObj->GetMeshEffectPair());
	m_EffectsAndMeshes.push_back(m_TrafficLightObj->GetMeshEffectPair());
	for (auto&x:m_TreeObjs)
	{
		m_EffectsAndMeshes.push_back(x->GetMeshEffectPair());
	}


	m_GameObjectLocalToWorldTransforms.clear();
	m_GameObjectLocalToWorldTransforms.push_back(m_Car->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));
	m_GameObjectLocalToWorldTransforms.push_back(m_NPCCar->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));
	m_GameObjectLocalToWorldTransforms.push_back(m_RaceTrackObj->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));
	m_GameObjectLocalToWorldTransforms.push_back(m_TrafficLightObj->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));
	for (auto&x:m_TreeObjs)
	{
		m_GameObjectLocalToWorldTransforms.push_back(x->GetLocalToWorldTransformation(i_elapsedSecondCount_sinceLastSimulationUpdate));
	}


	eae6320::Graphics::SetCameraToRender(m_RenderingCamera, i_elapsedSecondCount_sinceLastSimulationUpdate);
	eae6320::Graphics::SetEffectsAndMeshesToRender(&(m_EffectsAndMeshes[0]), &(m_GameObjectLocalToWorldTransforms[0]), (uint8_t)(4 + (m_TreeObjs.size())));
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cFinalGame::Initialize()
{
	eae6320::Graphics::cCamera::CreateCamera(m_TopDownCamera);
	eae6320::Graphics::cCamera::CreateCamera(m_InCarCamera);

	m_RenderingCamera = m_TopDownCamera;

	eae6320::Physics::cGameObject::CreateGameObject(m_Car);
	eae6320::Physics::cGameObject::CreateGameObject(m_NPCCar);
	eae6320::Physics::cGameObject::CreateGameObject(m_RaceTrackObj);
	eae6320::Physics::cGameObject::CreateGameObject(m_TrafficLightObj);

	for (size_t i = 1; i < 32; i++)
	{
		eae6320::Physics::cGameObject* temp;
		eae6320::Physics::cGameObject::CreateGameObject(temp);
		if (i < 16)
		{
			temp->SetGameObjectPosition(Math::sVector(-9, -10, (i * -25.0f)));
		}
		else if (i > 16)
		{
			temp->SetGameObjectPosition(Math::sVector(9, -10, ((i - 16) * -25.0f)));
		}
		m_TreeObjs.push_back(temp);
	}

	ResetDetails();

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
	fName = "data/Meshes/Tree.meshbinary";
	eae6320::Graphics::cMesh::s_Manager.Load(fName.c_str(), m_TreeHandle);

	std::string effectPath = "data/Effects/Effect1.effectbinary";
	eae6320::Graphics::cEffect::s_Manager.Load(effectPath.c_str(), effect1Handle);

	m_PlayerCarHandle = m_CarHandles[1];
	m_NPCCarHandle = m_CarHandles[2];

	UpdateMeshAndEffect();
	GetDetailsFromFile();
	m_PlayerCarAccelerationValue = m_AccelerationDetails[0];
	m_NPCCarAccelerationValue = m_AccelerationDetails[1];
	m_IsGameStarted = true;

	return Results::Success;
}

void eae6320::cFinalGame::UpdateCarPosition()
{
	if (m_CanTakeInput)
	{

		if (GetNormalizedStickDeflection(ControllerKeyCodes::RIGHT_STICK, 0).x != 0)
		{
			m_TopDownCamera->SetAngularSpeed(GetNormalizedStickDeflection(ControllerKeyCodes::RIGHT_STICK, 0).x);
			m_InCarCamera->SetAngularSpeed(GetNormalizedStickDeflection(ControllerKeyCodes::RIGHT_STICK, 0).x);
		}
		if (IsKeyPressed(ControllerKeyCodes::RIGHT_TRIGGER) || (UserInput::IsKeyPressed('W')))
		{
			if (m_Car->GetGameObjectVelocity().z >= -20)
			{
				float val = m_PlayerCarAccelerationValue;
				if (!(GetNumberOfConnectedControllers() == 0))
				{
					val = GetNormalizedTriggerDeflection(ControllerKeyCodes::RIGHT_TRIGGER) * m_PlayerCarAccelerationValue;
				}
				m_TopDownCamera->SetCameraAcceleration(Math::sVector(0, 0, -val));
				m_InCarCamera->SetCameraAcceleration(Math::sVector(0, 0, -val));
				m_Car->SetGameObjectAcceleration(Math::sVector(0, 0, -val));
			}
		}
		if (IsKeyPressed(ControllerKeyCodes::LEFT_TRIGGER) || (UserInput::IsKeyPressed('S')))
		{
			if (m_Car->GetGameObjectVelocity().z <= 10)
			{
				float val = m_PlayerCarAccelerationValue;
				if (!(GetNumberOfConnectedControllers() == 0))
				{
					val = GetNormalizedTriggerDeflection(ControllerKeyCodes::LEFT_TRIGGER) * m_PlayerCarAccelerationValue;
				}
				m_TopDownCamera->SetCameraAcceleration(Math::sVector(0, 0, val));
				m_InCarCamera->SetCameraAcceleration(Math::sVector(0, 0, val));
				m_Car->SetGameObjectAcceleration(Math::sVector(0, 0, val));
			}
		}
		if (UserInput::IsKeyPressed('N') || IsKeyPressed(ControllerKeyCodes::DPAD_UP))
		{
			if (!m_IsCarMeshSwitched)
			{
				m_CarCount = (m_CarCount + 1) % m_CarHandles.size();
				m_PlayerCarHandle = m_CarHandles[m_CarCount];
				UpdateMeshAndEffect();
			}
		}
		if (UserInput::IsKeyPressed('M') || IsKeyPressed(ControllerKeyCodes::DPAD_DOWN))
		{
			if (!m_IsCarMeshSwitched)
			{
				auto t = (m_CarCount + 1);
				m_CarCount = (m_CarCount + 1) % m_CarHandles.size();
				m_PlayerCarHandle = m_CarHandles[m_CarHandles.size() - t];
				UpdateMeshAndEffect();
			}
		}
	}
	if (m_IsGameFinished && (IsKeyPressed(ControllerKeyCodes::START)))
	{
		m_IsGameFinished = false;
		m_IsGameStarted = true;
		ResetDetails();
	}
	if (m_Car->GetGameObjectPosition().z < -300)
	{
		m_Car->SetGameObjectVelocity(Math::sVector(0, 0, 0));
		m_TopDownCamera->SetCameraVelocity(Math::sVector(0, 0, 0));
		m_InCarCamera->SetCameraVelocity(Math::sVector(0, 0, 0));
		m_Car->SetGameObjectAcceleration(Math::sVector(0, 0, 0));
		m_TopDownCamera->SetCameraAcceleration(Math::sVector(0, 0, 0));
		m_InCarCamera->SetCameraAcceleration(Math::sVector(0, 0, 0));
	}
}

void eae6320::cFinalGame::UpdateMeshAndEffect()
{
	m_IsCarMeshSwitched = true;
	m_Car->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_PlayerCarHandle));
	m_Car->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));

	m_NPCCar->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_NPCCarHandle));
	m_NPCCar->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));

	m_RaceTrackObj->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_RaceTrack));
	m_RaceTrackObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));

	m_TrafficLightObj->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_TrafficLight));
	m_TrafficLightObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));

	for (auto&treeObj : m_TreeObjs)
	{
		treeObj->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_TreeHandle));
		treeObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));
	}
}

void eae6320::cFinalGame::ResetDetails()
{
	m_Car->SetGameObjectPosition(Math::sVector(-4, -9.5f, -25));
	m_NPCCar->SetGameObjectPosition(Math::sVector(4, -9.5f, -25));
	m_RaceTrackObj->SetGameObjectPosition(Math::sVector(0, -10, -210));
	m_TrafficLightObj->SetGameObjectPosition(Math::sVector(0, -10, -25));
	m_TopDownCamera->SetCameraPosition(Math::sVector(-3.5f, 0.4f, -2));
	m_TopDownCamera->SetCameraRotation(Math::cQuaternion(0.4f, Math::sVector(-1, 0, 0)));
	m_InCarCamera->SetCameraPosition(Math::sVector(-4.6f, -8.1f, -25.5f));
	m_Car->SetGameObjectVelocity(Math::sVector(0, 0, 0));
	m_TopDownCamera->SetCameraVelocity(Math::sVector(0, 0, 0));
	m_InCarCamera->SetCameraVelocity(Math::sVector(0, 0, 0));
	m_Car->SetGameObjectAcceleration(Math::sVector(0, 0, 0));
	m_TopDownCamera->SetCameraAcceleration(Math::sVector(0, 0, 0));
	m_InCarCamera->SetCameraAcceleration(Math::sVector(0, 0, 0));
}

void eae6320::cFinalGame::GetDetailsFromFile()
{
	eae6320::Platform::sDataFromFile dataFromFile;
	std::string errorMessage;
	eae6320::Platform::LoadBinaryFile("Details.bin", dataFromFile, &errorMessage);
	uintptr_t offset = reinterpret_cast<uintptr_t>(dataFromFile.data);
	const uintptr_t finalOffset = offset + dataFromFile.size;
	m_AccelerationDetails.push_back(*reinterpret_cast<uint8_t*>(offset));
	offset += sizeof(uint8_t);
	m_AccelerationDetails.push_back(*reinterpret_cast<uint8_t*>(offset));
	offset += sizeof(uint8_t);
	m_AccelerationDetails.push_back(*reinterpret_cast<uint8_t*>(offset));
}

eae6320::cResult eae6320::cFinalGame::CleanUp()
{
	eae6320::Graphics::cEffect::s_Manager.Release(effect1Handle);
	for (auto&x : m_CarHandles)
	{
		eae6320::Graphics::cMesh::s_Manager.Release(x);
	}
	eae6320::Graphics::cMesh::s_Manager.Release(m_RaceTrack);
	eae6320::Graphics::cMesh::s_Manager.Release(m_TrafficLight);
	eae6320::Graphics::cMesh::s_Manager.Release(m_TreeHandle);

	m_Car->DecrementReferenceCount();
	m_NPCCar->DecrementReferenceCount();
	m_RaceTrackObj->DecrementReferenceCount();
	m_TrafficLightObj->DecrementReferenceCount();

	for (auto&x:m_TreeObjs)
	{
		x->DecrementReferenceCount();
	}

	m_TopDownCamera->DecrementReferenceCount();
	m_InCarCamera->DecrementReferenceCount();

	return Results::Success;
}