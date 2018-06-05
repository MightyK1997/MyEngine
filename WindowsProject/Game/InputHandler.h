#pragma once

#include <string>
#include "..\..\Engine\Renderer\Renderer.h"
#include "..\..\Engine\Renderer\System\Input.h"
#include "..\..\Engine\Physics\Physics.h"
#include "..\..\Engine\Timer\Timer.h"
#include "..\..\Engine\Logging\ConsolePrint.h"
#include "..\..\Engine\GameData\Classes\ReadGameData.h"
#include "..\..\Engine\Profiling\Profiler.h"
#include "..\..\Messaging\Messaging.h"
namespace Game
{
	class InputClass : public Engine::Messaging::MessageHandler
	{
	public:
		~InputClass() {};

		void ProcessMessage(const std::string& i_Message, void* i_Payload) override;
	};
}
