#include "EngineInit.h"
#include <crtdbg.h>
#include <malloc.h>

namespace Engine
{
	MemoryAllocators::AllocatingAllocators* m_heapManager;


	//Initialize all the Different Engine systems.
	bool Initialize(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
	{
		bool bSuccess = Engine::Renderer::Initialize(i_hInstance, i_nCmdShow, "GLibTest", -1, 800, 600);
		if (bSuccess)
		{
			GetAllocator();
			Engine::Profiler::Initialize();
			Engine::Timer::Initialize();
			Engine::GameObject::Initialize();
			Engine::Physics::Initialize();
			Engine::ReadGameObject::Initialize();
			Engine::Messaging::Initialize();
			Engine::JobSystem::Initialize();
			return true;
		}
		return false;
	}

	//Run the main Game loop
	void Run()
	{
		bool bQuit = false;
		do
		{
			Engine::Renderer::Service(bQuit);
			if (!bQuit)
			{
				Timer::Update();
				Engine::Renderer::BeginRendering();
				Engine::Renderer::Sprites::BeginRendering();
				Game::Update();
				Engine::Physics::Update();
			}
			Engine::Renderer::Sprites::EndRendering();
			Engine::Renderer::EndRendering();
		} while (bQuit == false);
	}

	//Closes everything and checks for memory leaks
	void Shutdown()
	{
		Engine::Timer::Shutdown();
		Engine::GameObject::Shutdown();
		Engine::Physics::Shutdown();
		Engine::Messaging::Shutdown();
		Engine::JobSystem::Shutdown();
		Engine::Renderer::Shutdown();
		Engine::Profiler::Shutdown();
		Engine::ReadGameObject::Shutdown();
		CleanUp();
	}

	//Return the Allocating allocator or call create function.
	MemoryAllocators::AllocatingAllocators* GetAllocator()
	{
		if (!m_heapManager)
		{
			SetAllocator();
		}
		return m_heapManager;
	}


	//Create New Allocator if there is none present.
	void SetAllocator()
	{
		m_heapManager = reinterpret_cast<MemoryAllocators::AllocatingAllocators*>(malloc(sizeof(MemoryAllocators::AllocatingAllocators)));
		m_heapManager->_Init();
	}

	//Cleanup all the allocators and check for memory leaks.

	void CleanUp()
	{
		//Call all destructors
		GetAllocator()->Destroy();
		free(m_heapManager);
	}
}