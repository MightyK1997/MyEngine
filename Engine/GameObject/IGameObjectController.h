#pragma once

#include "..\Messaging\Messaging.h"



class IGameObjectController
{

};


class PlayerController : public IGameObjectController, Engine::Messaging::MessageHandler
{
public:
	PlayerController()
	{
		Engine::Messaging::AddMessageReciever("ButtonPressed", Engine::Messaging::MessageDelegateStruct(this, &PlayerController::ProcessKeyDownMessage));
		Engine::Messaging::AddMessageReciever("ButtonReleased", Engine::Messaging::MessageDelegateStruct(this, &PlayerController::ProcessKeyUpMessage));
	}

	void Update()
	{

	}

	void ProcessKeyDownMessage(const std::string& i_Message, void* i_Payload)
	{

	}

	void ProcessKeyUpMessage(const std::string& i_message, void* i_Payload)
	{

	}
};
