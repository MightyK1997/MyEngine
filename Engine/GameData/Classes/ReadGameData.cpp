#include "ReadGameData.h"

namespace Engine
{
	lua_State* ReadGameObject::m_pLuaState;
	void Engine::ReadGameObject::Initialize()
	{
#ifdef _DEBUG
		ConsolePrint_Windows("Lua Initialize", __LINE__, __FILE__, "Lua");
#endif // _DEBUG
		m_pLuaState = luaL_newstate();
		luaL_openlibs(m_pLuaState);
	}

	void Engine::ReadGameObject::Shutdown()
	{
#ifdef _DEBUG
		ConsolePrint_Windows("Lua Shutdown", __LINE__, __FILE__, "Lua");
#endif // _DEBUG
		lua_close(m_pLuaState);
	}

	void ReadGameObject::CreateGameObject(std::string i_FileName)
	{
		Engine::Core::FileDetails* fd =  new Engine::Core::FileDetails(i_FileName.c_str(), m_pLuaState);
		Engine::JobSystem::RunJob(fd, "FileProcessing");
	}
}