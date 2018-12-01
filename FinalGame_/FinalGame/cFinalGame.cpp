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
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cFinalGame::Initialize()
{
	eae6320::Graphics::cCamera::CreateCamera(m_TopDownCamera);
	eae6320::Graphics::cCamera::CreateCamera(m_InCarCamera);


	m_TopDownCamera->SetCameraPosition(Math::sVector(0, 0, 10));

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
	if (IsKeyPressed(ControllerKeyCodes::RIGHT_TRIGGER))
	{
		m_TopDownCamera->SetCameraVelocity(Math::sVector(0, GetNormalizedTriggerDeflection(ControllerKeyCodes::RIGHT_TRIGGER), 0));
		m_InCarCamera->SetCameraVelocity(Math::sVector(0, GetNormalizedTriggerDeflection(ControllerKeyCodes::RIGHT_TRIGGER), 0));
		m_Car->SetGameObjectVelocity(Math::sVector(0, GetNormalizedTriggerDeflection(ControllerKeyCodes::RIGHT_TRIGGER) * 10, 0));
	}
	if (IsKeyPressed(ControllerKeyCodes::LEFT_TRIGGER))
	{
		m_TopDownCamera->SetCameraVelocity(Math::sVector(0, -GetNormalizedTriggerDeflection(ControllerKeyCodes::LEFT_TRIGGER), 0));
		m_InCarCamera->SetCameraVelocity(Math::sVector(0, -GetNormalizedTriggerDeflection(ControllerKeyCodes::LEFT_TRIGGER), 0));
		m_Car->SetGameObjectVelocity(Math::sVector(0, GetNormalizedTriggerDeflection(ControllerKeyCodes::LEFT_TRIGGER) * -10, 0));
	}
}
