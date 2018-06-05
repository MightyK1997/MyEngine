#pragma once


#include "GameClass.h"
#include "InputHandler.h"

namespace Game
{
	void Initialize();
	void Shutdown();
	void Update();
	extern Game* pong;
}