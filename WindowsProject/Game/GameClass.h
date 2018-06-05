#pragma once

#include <string>
#include <array>

#include "InputHandler.h"

namespace Game
{
	class Game
	{
	public:
		void Initialize();
		void Shutdown();
		void Update();
	private:
		void CreateGameObjects();
		void SetGameData();
		void ResetPositionAndVelocity();
		int m_player1Score = 0;
		int m_player2Score = 0;
		bool m_isPlayer1LastWinner = false;
		bool m_isPlayer2LastWinner = false;
		bool m_isWinScreen = false;
		Engine::ReadGameObject rGo;
		Engine::GameObject::GameObject* gameObject;
		Engine::GameObject::GameObject* badGuygameObject;
		Engine::Physics::PhysicsObject* goodGuy;
		Engine::Physics::PhysicsObject* badGuy;
		Engine::GameObject::GameObject* ballGameObject;
		Engine::Physics::PhysicsObject* ball;
		InputClass* m_IClassPointer;
		std::array<Engine::GameObject::GameObject*, 5> scoreArray;
		Engine::GameObject::GameObject* p1GameObject;
		Engine::GameObject::GameObject* p2GameObject;
		Engine::GameObject::GameObject* restartGameObject;
	};
}