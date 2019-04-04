#include "cDirectionalLight.h"

eae6320::cResult eae6320::Graphics::cDirectionalLight::CreateDirectionalLight(cDirectionalLight *& o_Light)
{
	cResult result = Results::Success;
	o_Light = new cDirectionalLight();
	if (o_Light == nullptr) return Results::Failure;
	return result;
}

eae6320::cResult eae6320::Graphics::cDirectionalLight::CreateDirectionalLight(Math::sVector i_InputPosition, Math::cQuaternion i_InputRotation, sColor i_Color, cDirectionalLight *& o_Light)
{
	cResult result = Results::Success;
	o_Light = new cDirectionalLight();
	if (o_Light == nullptr) return Results::Failure;
	o_Light->m_RigidBody.position = i_InputPosition;
	o_Light->m_RigidBody.orientation = i_InputRotation;
	o_Light->m_LightColor = i_Color;
	return result;
}

eae6320::Math::sVector eae6320::Graphics::cDirectionalLight::PredictFuturePosition(const float i_secondCountToExtrapolate) const
{
	return m_RigidBody.PredictFuturePosition(i_secondCountToExtrapolate);
}

eae6320::Math::cQuaternion eae6320::Graphics::cDirectionalLight::PredictFutureOrientation(const float i_secondCountToExtrapolate) const
{
	return m_RigidBody.PredictFutureOrientation(i_secondCountToExtrapolate);
}

void eae6320::Graphics::cDirectionalLight::Update(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_RigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
}

eae6320::Math::cMatrix_transformation eae6320::Graphics::cDirectionalLight::GetLocalToWorldTransformation(float i_secondCountToExtrapolate) const
{
	return eae6320::Math::cMatrix_transformation(m_RigidBody.PredictFutureOrientation(i_secondCountToExtrapolate),
		m_RigidBody.PredictFuturePosition(i_secondCountToExtrapolate));
}
