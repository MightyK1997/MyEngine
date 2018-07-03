#include "GameObject.h"



namespace Engine
{
	namespace GameObject
	{
		void GameObject::Initialize()
		{
			m_pLuaState = luaL_newstate();
			luaL_openlibs(m_pLuaState);
		}
	}
}