#include "GameObject.h"

using namespace Engine;
using namespace Math;
using namespace Vector;
namespace Engine
{
	namespace GameObject
	{
		void GameObject::Initialize()
		{
			m_pLuaState = luaL_newstate();
			luaL_openlibs(m_pLuaState);
		}

		void GameObject::LoadFile(std::string i_FileLocation)
		{
			assert(i_FileLocation.c_str() != NULL);
			FILE * pFile = NULL;
			errno_t fopenError = fopen_s(&pFile, i_FileLocation.c_str(), "rb");
			if (fopenError != 0)
				m_fileData = nullptr;
			assert(pFile != NULL);
			int FileIOError = fseek(pFile, 0, SEEK_END);
			assert(FileIOError == 0);
			long FileSize = ftell(pFile);
			assert(FileSize >= 0);
			FileIOError = fseek(pFile, 0, SEEK_SET);
			assert(FileIOError == 0);
			uint8_t * pBuffer = new uint8_t[FileSize];
			assert(pBuffer);
			size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
			assert(FileRead == FileSize);
			fclose(pFile);
			m_sizeOfFile = FileSize;
			m_fileData = pBuffer;
		}

		void GameObject::ProcessFileData()
		{
			char* m_fileInfo = reinterpret_cast<char*>(m_fileData);
			int result = 0;
			result = luaL_loadbuffer(m_pLuaState, m_fileInfo, m_sizeOfFile, NULL);
			assert(result == 0);
			result = lua_pcall(m_pLuaState, 0, 0, 0);
			assert(result == 0);
			int type = lua_getglobal(m_pLuaState, "GameObject");

			assert(type == LUA_TTABLE);



			lua_pushstring(m_pLuaState, "name");

			type = lua_gettable(m_pLuaState, -2);

			assert(type == LUA_TSTRING);

			std::string gameObjectName;

			if (type == LUA_TSTRING)

			{

				gameObjectName = lua_tostring(m_pLuaState, -1);

				//ConsolePrint_Windows(gameObjectName.c_str(), __LINE__, __FILE__, "lua");

			}



			lua_pop(m_pLuaState, 1);



			lua_pushstring(m_pLuaState, "class");

			type = lua_gettable(m_pLuaState, -2);

			std::string gameObjectClass;

			if (type == LUA_TSTRING)

			{

				gameObjectClass = lua_tostring(m_pLuaState, -1);

				//ConsolePrint_Windows(gameObjectClass.c_str(), __LINE__, __FILE__, "lua");

			}



			lua_pop(m_pLuaState, 1);

			float postionFloats[3];

			int someValue = 0;

			lua_pushstring(m_pLuaState, "position");

			type = lua_gettable(m_pLuaState, -2);
			if (type == LUA_TTABLE)
			{
				lua_pushnil(m_pLuaState);
				while (lua_next(m_pLuaState, -2) != 0)
				{
					type = lua_type(m_pLuaState, -1);
					assert(type == LUA_TNUMBER);
					postionFloats[someValue] = static_cast<float>(lua_tonumber(m_pLuaState, -1));
					someValue++;
					lua_pop(m_pLuaState, 1);
				}
			}
			lua_pop(m_pLuaState, 1);
			float rotationFloats[3];
			someValue = 0;
			lua_pushstring(m_pLuaState, "rotation");
			type = lua_gettable(m_pLuaState, -2);
			if (type == LUA_TTABLE)
			{
				lua_pushnil(m_pLuaState);
				while (lua_next(m_pLuaState, -2) != 0)
				{
					type = lua_type(m_pLuaState, -1);
					assert(type == LUA_TNUMBER);
					rotationFloats[someValue] = static_cast<float>(lua_tonumber(m_pLuaState, -1));

					someValue++;

					lua_pop(m_pLuaState, 1);

				}
			}
			lua_pop(m_pLuaState, 1);
			int temp;
			lua_pushstring(m_pLuaState, "isKinematic");
			type = lua_gettable(m_pLuaState, -2);
			assert(type == LUA_TBOOLEAN);
			temp = lua_toboolean(m_pLuaState, -1);
			//ConsolePrint_Windows(gameObjectClass.c_str(), temp, __FILE__, "lua");
			lua_pop(m_pLuaState, 1);
			std::string spriteFileLocation;

			lua_pushstring(m_pLuaState, "sprite");

			type = lua_gettable(m_pLuaState, -2);

			assert(type == LUA_TTABLE);

			if (type == LUA_TTABLE)

			{

				lua_pushstring(m_pLuaState, "fileLocation");

				type = lua_gettable(m_pLuaState, -2);

				assert(type == LUA_TSTRING);

				if (type == LUA_TSTRING)
				{
					spriteFileLocation = lua_tostring(m_pLuaState, -1);
					//ConsolePrint_Windows(spriteFileLocation.c_str(), __LINE__, __FILE__, "lua");
				}
			}
			lua_pop(m_pLuaState, 1);
			//ConsolePrint_Windows("In Lua things", __LINE__, __FILE__, "LuaThing");			
			//m_Sprite = CreateSprite(spriteFileLocation);
			m_Position = Vector3(postionFloats[0], postionFloats[1], postionFloats[2]);
			m_Rotation = Vector3(rotationFloats[0], rotationFloats[1], rotationFloats[2]);
			//m_GameObjectAABB = { Vector3(static_cast<float>(co.GetWidth() / 2), static_cast<float>(co.GetHeight() / 2), 0), Vector3(static_cast<float>(co.GetWidth() / 2), static_cast<float>(co.GetHeight() / 2), 0) };
			m_isKinematic = temp == 0;
			m_Name = gameObjectName;
			m_Class = gameObjectClass;
			//Engine::GameObject::m_allGameObjects->operator[](gameObjectName) = *m_returnGameObject;
		}
	}
}