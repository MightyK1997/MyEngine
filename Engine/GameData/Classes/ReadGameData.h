#pragma once

#include <Windows.h>

#ifdef __cplusplus
#include <lua.hpp>
#elif
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#endif

#include "..\..\GameObject\GameObject.h"
#include "..\..\CoreFuntions\CoreFunctions.h"
#include "..\..\JobSystem\JobSystem.h"

namespace Engine
{
	class ReadGameObject
	{
	public:
		static void Initialize();
		void CreateGameObject(std::string);
		static void Shutdown();
	private:
		static lua_State* m_pLuaState;
	};
}