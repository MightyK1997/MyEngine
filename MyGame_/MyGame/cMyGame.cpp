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

void eae6320::cMyGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
	Application::cbApplication::SetSimulationRate(UserInput::IsKeyPressed(UserInput::KeyCodes::Shift) ? 0.5f : 1.0f);
}

void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
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
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{
	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	return Results::Success;
}
