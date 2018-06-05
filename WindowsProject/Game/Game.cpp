#include "Game.h"

using namespace Engine;
using namespace Renderer;
using namespace std;

namespace Game
{
	Game* pong;

	void Initialize()
	{
		pong = new Game();
		pong->Initialize();
	}

	void Shutdown()
	{
		pong->Shutdown();
		delete pong;
	}

	void Update()
	{
		if (true)
		{
			pong->Update();
		}
	}
}