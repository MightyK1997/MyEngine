#pragma once

#ifdef __cplusplus

#include <lua.hpp>

#elif

#include <lua.h>

#include <lualib.h>

#include <lauxlib.h>

#endif

#include <string>
#include <cassert>
#include "../Math/Vector/Vector3.h"
#include "../Math/Vector/Vector4.h"

namespace Engine
{
	namespace GameObject
	{

		struct AABB
		{
			Math::Vector::Vector3 m_Top;
			Math::Vector::Vector3 m_Bottom;
		};
		class GameObject
		{
		public:
			void Initialize();
			void Update();
			void Shutdown();
			void LoadFile(std::string i_FileLocation);
			void ProcessFileData();
			void CreateSprite(std::string i_FileLocation);
		private:
			static lua_State* m_pLuaState;
			uint8_t* m_fileData;
			size_t m_sizeOfFile;
			bool m_isKinematic;
			/*Engine::Renderer::Sprite* m_Spite;*/
			Math::Vector::Vector3 m_Position;
			Math::Vector::Vector3 m_Rotation;
			AABB m_GameObjectAABB;
			std::string m_Name;
			std::string m_Class;
		};
	}
}