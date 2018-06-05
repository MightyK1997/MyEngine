#pragma once
#include <string>
#include <map>
#include <vector>
#include <cassert>

#include "..\Logging\ConsolePrint.h"

namespace Engine
{
	namespace Messaging
	{
		class MessageHandler
		{
		public:
			virtual ~MessageHandler() = 0;
			virtual void ProcessMessage(const std::string & i_Message, void* i_Payload) = 0;
		};

		void AddMessageReciever(std::string i_MessageRecieverName, MessageHandler* i_pReciver);
		void RemoveMessageReciever(std::string i_MessageRecieverName, MessageHandler* i_pReciever);
		void SendMessageToRecievers(std::string i_Message, void* i_pPayload);
		static std::map<std::string, std::vector<MessageHandler*>>* m_messageRecieverMap;
		void Initialize();
		void Shutdown();
	}
}
