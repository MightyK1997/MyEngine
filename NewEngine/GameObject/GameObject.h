#pragma once

#ifdef __cplusplus

#include <lua.hpp>

#elif

#include <lua.h>

#include <lualib.h>

#include <lauxlib.h>

#endif

namespace Engine
{
	namespace GameObject
	{
		class GameObject
		{
		public:
			void Initialize();
			void Update();
			void Shutdown();
		private:
			static lua_State* m_pLuaState;
		};
	}
}