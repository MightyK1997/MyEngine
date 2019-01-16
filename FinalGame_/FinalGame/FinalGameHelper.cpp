#include "cFinalGame.h"

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

void eae6320::cFinalGame::ResetDetails()
{
	m_Car->SetGameObjectPosition(Math::sVector(-4, -9.5f, -25));
	m_NPCCar->SetGameObjectPosition(Math::sVector(4, -9.5f, -25));
	m_RaceTrackObj->SetGameObjectPosition(Math::sVector(0, -10, -210));
	m_TrafficLightObj->SetGameObjectPosition(Math::sVector(0, -10, -25));

	m_PlayerScoreObj->SetGameObjectPosition(Math::sVector(-10, -3, -25));
	m_NPCScoreObj->SetGameObjectPosition(Math::sVector(4, -3, -25));

	m_RestartObj->SetGameObjectPosition(Math::sVector(100, 100, 100));

	m_TopDownCamera->SetCameraPosition(Math::sVector(-3.5f, 0.4f, -2));
	m_TopDownCamera->SetCameraRotation(Math::cQuaternion(0.4f, Math::sVector(-1, 0, 0)));
	m_InCarCamera->SetCameraPosition(Math::sVector(-4.6f, -8.1f, -25.5f));
	m_Car->SetGameObjectVelocity(Math::sVector(0, 0, 0));
	m_TopDownCamera->SetCameraVelocity(Math::sVector(0, 0, 0));
	m_InCarCamera->SetCameraVelocity(Math::sVector(0, 0, 0));
	m_Car->SetGameObjectAcceleration(Math::sVector(0, 0, 0));
	m_TopDownCamera->SetCameraAcceleration(Math::sVector(0, 0, 0));
	m_InCarCamera->SetCameraAcceleration(Math::sVector(0, 0, 0));
	m_NPCCar->SetGameObjectVelocity(Math::sVector(0, 0, 0));
	m_NPCCar->SetGameObjectAcceleration(Math::sVector(0, 0, 0));
	m_AccelerationSound->Stop();
	startSound->Play();
}

void eae6320::cFinalGame::UpdateMeshAndEffect()
{
	m_IsCarMeshSwitched = true;
	m_Car->SetGameObjectHandles(m_PlayerCarHandle, effect1Handle);
	m_Car->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_PlayerCarHandle));
	m_Car->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));

	m_NPCCar->SetGameObjectHandles(m_NPCCarHandle, effect1Handle);
	m_NPCCar->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_NPCCarHandle));
	m_NPCCar->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));


	m_RaceTrackObj->SetGameObjectHandles(m_RaceTrack, effect1Handle);
	m_RaceTrackObj->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_RaceTrack));
	m_RaceTrackObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));


	m_TrafficLightObj->SetGameObjectHandles(m_TrafficLight, effect1Handle);
	m_TrafficLightObj->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_TrafficLight));
	m_TrafficLightObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));


	m_RestartObj->SetGameObjectHandles(m_RestartHandle, effect1Handle);
	m_RestartObj->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_RestartHandle));
	m_RestartObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));

	m_CountdownHandle = m_ScoreHandles[3];
	m_CountdownObj->SetGameObjectHandles(m_CountdownHandle, effect1Handle);
	m_CountdownObj->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_CountdownHandle));
	m_CountdownObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));

	int count = 0;

	for (auto&treeObj : m_TreeObjs)
	{
		if (count < 16)
		{
			treeObj->SetGameObjectHandles(m_TreeHandle, effect2Handle);
			treeObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect2Handle));
		}
		else
		{
			treeObj->SetGameObjectHandles(m_TreeHandle, effect1Handle);
			treeObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));
		}
		count++;
		treeObj->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_TreeHandle));
	}
	if (m_PlayerScore < 10)
	{
		m_PlayerScoreHandle = m_ScoreHandles[m_PlayerScore];
		m_PlayerScoreObj->SetGameObjectHandles(m_PlayerScoreHandle, effect1Handle);
		m_PlayerScoreObj->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_PlayerScoreHandle));
		m_PlayerScoreObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));
	}
	if (m_NPCScore < 10)
	{
		m_NPCScoreHandle = m_ScoreHandles[m_NPCScore];
		m_NPCScoreObj->SetGameObjectHandles(m_NPCScoreHandle, effect1Handle);
		m_NPCScoreObj->SetGameObjectMesh(eae6320::Graphics::cMesh::s_Manager.Get(m_NPCScoreHandle));
		m_NPCScoreObj->SetGameObjectEffect(eae6320::Graphics::cEffect::s_Manager.Get(effect1Handle));
	}

}