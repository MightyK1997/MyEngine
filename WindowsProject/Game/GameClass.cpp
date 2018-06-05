#include "GameClass.h"

using namespace Engine;
using namespace Renderer;
using namespace std;
namespace Game
{
	void Game::Initialize()
	{
		this->CreateGameObjects();
		Sleep(2);
		this->SetGameData();
	}

	void Game::Shutdown()
	{
		Engine::Renderer::Sprites::Release(badGuygameObject->GetGameObjectSprite());
		Engine::Renderer::Sprites::Release(ballGameObject->GetGameObjectSprite());
		for (int i = 0;i < 5;i++)
		{
			Engine::Renderer::Sprites::Release(scoreArray[i]->GetGameObjectSprite());
		}
		Engine::Renderer::Sprites::Release(p1GameObject->GetGameObjectSprite());
		Engine::Renderer::Sprites::Release(p2GameObject->GetGameObjectSprite());
		Engine::Renderer::Sprites::Release(restartGameObject->GetGameObjectSprite());
		delete m_IClassPointer;
		//delete goodGuy;
		/*delete badGuygameObject;*/
		//delete badGuy;
		//delete ball;
#ifdef PROFILING
		float someValue = Engine::Profiler::g_Profiler->GetAccumulator("Collision").Average();
		c_printf("%f", "Collision Data", someValue);
#endif
	}

	void Game::Update()
	{
		if (ball->GetCurrentPosition().getY() > 300.0f || ball->GetCurrentPosition().getY() < -300.0f)
		{
			ball->SetCurrentVelocity(Vector3D(ball->GetCurrentVelocity().getX(), ball->GetCurrentVelocity().getY() * -1, 0.0f));
		}
		if (goodGuy->GetCurrentPosition().getY() > 165.0f || goodGuy->GetCurrentPosition().getY() < -300.0f)
		{
			goodGuy->SetCurrentVelocity(Vector3D(0));
		}
		if (badGuy->GetCurrentPosition().getY() > 165.0f || badGuy->GetCurrentPosition().getY() < -300.0f)
		{
			badGuy->SetCurrentVelocity(Vector3D(0));
		}
		Engine::KeyTypes returnValue = Engine::Renderer::returnVal;
		if (returnVal != KeyTypes::Escape || returnVal != KeyTypes::Null)
		{
			if (returnValue == KeyTypes::W)
			{
				Engine::Messaging::SendMessageToRecievers(string("Move Up"), static_cast<void*>(goodGuy));
				returnVal = KeyTypes::Null;
			}
			else if (returnValue == KeyTypes::S)
			{
				Engine::Messaging::SendMessageToRecievers(string("Move Down"), static_cast<void*>(goodGuy));
				returnVal = KeyTypes::Null;
			}
			else if (returnValue == KeyTypes::I)
			{
				Engine::Messaging::SendMessageToRecievers(string("Move Up"), static_cast<void*>(badGuy));
				returnVal = KeyTypes::Null;
			}
			else if (returnValue == KeyTypes::K)
			{
				Engine::Messaging::SendMessageToRecievers(string("Move Down"), static_cast<void*>(badGuy));
				returnVal = KeyTypes::Null;
			}
		}
		if (ball->GetCurrentPosition() == Vector3D(0, 0, 0) && returnVal == KeyTypes::Space && !m_isWinScreen)
		{
			if (m_isPlayer1LastWinner)
			{
				Engine::Messaging::SendMessageToRecievers(string("Add Force Left"), static_cast<void*>(ball));
				returnVal = KeyTypes::Null;
				m_isPlayer1LastWinner = false;
				m_isPlayer2LastWinner = false;
			}
			else if (m_isPlayer2LastWinner)
			{
				Engine::Messaging::SendMessageToRecievers(std::string("Add Force Right"), static_cast<void*>(ball));
				returnVal = KeyTypes::Null;
				m_isPlayer1LastWinner = false;
				m_isPlayer2LastWinner = false;
			}
			else
			{
				Engine::Messaging::SendMessageToRecievers(string("Add Force Left"), static_cast<void*>(ball));
				returnVal = KeyTypes::Null;
			}
		}
		if ((ball->GetCurrentPosition().getX() > 370) || (ball->GetCurrentPosition().getX() < -370))
		{
			if (ball->GetCurrentPosition().getX() > 370)
			{
				m_player1Score++;
				ResetPositionAndVelocity();
				m_isPlayer1LastWinner = true;
			}
			else if (ball->GetCurrentPosition().getX() < -370)
			{
				m_player2Score++;
				ResetPositionAndVelocity();
				m_isPlayer2LastWinner = true;
			}
		}
		if (m_player1Score > 0 && m_player1Score < 6)
		{
			Engine::Renderer::Sprites::RenderSprite(*(scoreArray[m_player1Score - 1]->GetGameObjectSprite()), Point2D{ -50.0f, 250.0f }, 0.0f);
		}
		if (m_player2Score > 0 && m_player2Score < 6)
		{
			Engine::Renderer::Sprites::RenderSprite(*(scoreArray[m_player2Score - 1]->GetGameObjectSprite()), Point2D{ 50.0f, 250.0f }, 0.0f);
		}
		if (m_player1Score >= 5 || m_player2Score >= 5)
		{
			p1GameObject = &(Engine::GameObject::m_allGameObjects->operator[]("p1"));
			p2GameObject = &(Engine::GameObject::m_allGameObjects->operator[]("p2"));
			restartGameObject = &(Engine::GameObject::m_allGameObjects->operator[]("restart"));
			if (m_player1Score >= 5)
			{
				Engine::Renderer::Sprites::RenderSprite(*(p1GameObject->GetGameObjectSprite()), Point2D{ 0 }, 0.0f);
			}
			else if (m_player2Score >= 1)
			{
				Engine::Renderer::Sprites::RenderSprite(*(p2GameObject->GetGameObjectSprite()), Point2D{ 0 }, 0.0f);
			}
			Engine::Renderer::Sprites::RenderSprite(*(restartGameObject->GetGameObjectSprite()), Point2D{ 0, -100 }, 0.0f);
			m_isWinScreen = true;
		}
		if (m_isWinScreen && returnVal == KeyTypes::Space)
		{
			m_player1Score = 0;
			m_player2Score = 0;
			m_isWinScreen = false;
			ResetPositionAndVelocity();
		}
		Engine::Renderer::Sprites::RenderSprite(*(gameObject->GetGameObjectSprite()), Point2D{ goodGuy->GetCurrentPosition().getX(), goodGuy->GetCurrentPosition().getY() }, 0.0f);
		Engine::Renderer::Sprites::RenderSprite(*(badGuygameObject->GetGameObjectSprite()), Point2D{ badGuy->GetCurrentPosition().getX(), badGuy->GetCurrentPosition().getY() }, 0.0f);
		Engine::Renderer::Sprites::RenderSprite(*(ballGameObject->GetGameObjectSprite()), Point2D{ ball->GetCurrentPosition().getX(), ball->GetCurrentPosition().getY() }, 0.0f);
	}


	void Game::CreateGameObjects()
	{
		badGuygameObject = new Engine::GameObject::GameObject();
		rGo.CreateGameObject(string("data/PaddleObjectInfo.lua"));
		rGo.CreateGameObject(string("data/BallObjectInfo.lua"));
		for (int i = 0;i < 5;i++)
		{
			rGo.CreateGameObject(string("data/Number") + to_string(i + 1) + string("Info.lua"));
		}
		rGo.CreateGameObject(string("data/p1.lua"));
		rGo.CreateGameObject(string("data/p2.lua"));
		rGo.CreateGameObject(string("data/restart.lua"));
	}

	void Game::SetGameData()
	{
		gameObject = &(Engine::GameObject::m_allGameObjects->operator[]("go"));
		ballGameObject = &(Engine::GameObject::m_allGameObjects->operator[]("ball"));
		for (int i = 0;i < 5;i++)
		{
			string temp = "Number" + to_string(i + 1);
			scoreArray[i] = &(Engine::GameObject::m_allGameObjects->operator[](temp));
		}
		p1GameObject = &(Engine::GameObject::m_allGameObjects->operator[]("p1"));
		p2GameObject = &(Engine::GameObject::m_allGameObjects->operator[]("p2"));
		restartGameObject = &(Engine::GameObject::m_allGameObjects->operator[]("restart"));
		*badGuygameObject = *gameObject;
		badGuygameObject->SetGameObjectPosition(Vector3D(350, 0, 0));
		goodGuy = new Physics::PhysicsObject(gameObject, 1000, 10);
		badGuy = new Physics::PhysicsObject(badGuygameObject, 1000, 0);
		ball = new Physics::PhysicsObject(ballGameObject, 10, 10);
		m_IClassPointer = new InputClass();
		Engine::Messaging::AddMessageReciever(string("Move Down"), m_IClassPointer);
		Engine::Messaging::AddMessageReciever(string("Move Up"), m_IClassPointer);
		Engine::Messaging::AddMessageReciever(string("Add Force Left"), m_IClassPointer);
		Engine::Messaging::AddMessageReciever(string("Add Force Right"), m_IClassPointer);
#ifdef PROFILING
		Profiler::g_Profiler->RegisterAccumulator("Collision");
#endif
	}

	void Game::ResetPositionAndVelocity()
	{
		badGuy->SetCurrentPosition(Vector3D(350, 0, 0));
		goodGuy->SetCurrentPosition(Vector3D(-350, 0, 0));
		ball->SetCurrentPosition(Vector3D(0, 0, 0));
		ball->SetCurrentVelocity(Vector3D(0, 0, 0));
		badGuy->SetCurrentVelocity(Vector3D(0, 0, 0));
		goodGuy->SetCurrentVelocity(Vector3D(0, 0, 0));
	}
}