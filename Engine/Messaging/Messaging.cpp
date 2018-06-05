#include "Messaging.h"

namespace Engine
{
	namespace Messaging
	{
		void Engine::Messaging::Initialize()
		{
#ifdef _DEBUG
			ConsolePrint_Windows("Creating Message Handler", __LINE__, __FILE__, "Messaging System");
#endif // _DEBUG
			m_messageRecieverMap = new std::map<std::string, std::vector<MessageHandler*>>;
		}
		void Engine::Messaging::AddMessageReciever(std::string i_MessageRecieverName, MessageHandler* i_pReciver)
		{
			auto& someval = m_messageRecieverMap->operator[](i_MessageRecieverName);
			someval.push_back(i_pReciver);
		}

		void Engine::Messaging::RemoveMessageReciever(std::string i_MessageRecieverName, MessageHandler* i_pReciever)
		{
			auto MessageRecievers = m_messageRecieverMap->find(i_MessageRecieverName);
			if (MessageRecievers != m_messageRecieverMap->end())
			{
				auto Reciever = MessageRecievers->second;
			}
		}

		void Engine::Messaging::SendMessageToRecievers(std::string i_Message, void* i_pPayload)
		{
			auto MessageRecievers = m_messageRecieverMap->find(i_Message);

			if (MessageRecievers != m_messageRecieverMap->end())
			{
				auto Recievers = MessageRecievers->second;
				for (auto Reciver : Recievers)
				{
					Reciver->ProcessMessage(i_Message, i_pPayload);
				}
			}
		}

		void Engine::Messaging::Shutdown()
		{
#ifdef _DEBUG
			ConsolePrint_Windows("Message System Shutdown", __LINE__, __FILE__, "Message System");
#endif // _DEBUG
			delete(m_messageRecieverMap);
		}

		MessageHandler::~MessageHandler()
		{

		}

	}
}


