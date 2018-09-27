#include "cCamera.h"

eae6320::cResult eae6320::Graphics::cCamera::CreateCamera(cCamera *& o_Camera)
{
	cResult result = Results::Success;
	o_Camera = new cCamera();
	if (o_Camera == nullptr) return Results::Failure;
	return result;
}

eae6320::cResult eae6320::Graphics::cCamera::CreateCamera(Math::sVector i_InputPosition, Math::cQuaternion i_InputRotation, cCamera *& o_Camera)
{
	cResult result = Results::Success;
	o_Camera = new cCamera();
	if (o_Camera == nullptr) return Results::Failure;
	o_Camera->m_CameraRigidBody.position = i_InputPosition;
	o_Camera->m_CameraRigidBody.orientation = i_InputRotation;
	return result;
}

void eae6320::Graphics::cCamera::SetCameraPosition(Math::sVector i_InputPosition)
{
	m_CameraRigidBody.position = i_InputPosition;
}

void eae6320::Graphics::cCamera::SetCameraRotation(Math::cQuaternion i_InputRotation)
{
	m_CameraRigidBody.orientation = i_InputRotation;
}

void eae6320::Graphics::cCamera::SetCameraVelocity(Math::sVector i_InputVelocity)
{
	m_CameraRigidBody.velocity = i_InputVelocity;
}

void eae6320::Graphics::cCamera::SetCameraAcceleration(Math::sVector i_InputAcceleration)
{
	m_CameraRigidBody.acceleration = i_InputAcceleration;
}

eae6320::Math::sVector eae6320::Graphics::cCamera::GetCameraPosition()
{
	return m_CameraRigidBody.position;
}

eae6320::Math::cQuaternion eae6320::Graphics::cCamera::GetCameraRotation()
{
	return m_CameraRigidBody.orientation;
}

eae6320::Math::sVector eae6320::Graphics::cCamera::PredictFuturePosition(const float i_secondCountToExtrapolate) const
{
	return m_CameraRigidBody.PredictFuturePosition(i_secondCountToExtrapolate);
}

eae6320::Math::cQuaternion eae6320::Graphics::cCamera::PredictFutureOrientation(const float i_secondCountToExtrapolate) const
{
	return m_CameraRigidBody.PredictFutureOrientation(i_secondCountToExtrapolate);
}

void eae6320::Graphics::cCamera::Update(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_CameraRigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
}
