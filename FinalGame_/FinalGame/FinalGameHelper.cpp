#include "cFinalGame.h"

void eae6320::cFinalGame::ResetDetails()
{
	m_TopDownCamera->SetCameraPosition(Math::sVector(-3.5f, 0.4f, -2));
	m_TopDownCamera->SetCameraRotation(Math::cQuaternion(0.4f, Math::sVector(-1, 0, 0)));
	m_InCarCamera->SetCameraPosition(Math::sVector(-4.6f, -8.1f, -25.5f));
	m_TopDownCamera->SetCameraVelocity(Math::sVector(0, 0, 0));
	m_InCarCamera->SetCameraVelocity(Math::sVector(0, 0, 0));
	m_TopDownCamera->SetCameraAcceleration(Math::sVector(0, 0, 0));
	m_InCarCamera->SetCameraAcceleration(Math::sVector(0, 0, 0));
}

void eae6320::cFinalGame::UpdateMeshAndEffect()
{

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
}