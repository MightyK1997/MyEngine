#pragma once

#include <cassert>
#include <sstream>
#include <windows.h>
#include <string.h>

#ifdef __cplusplus
#include <lua.hpp>
#elif
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#endif

#include "..\GameObject\GameObject.h"
#include "..\Renderer\Renderer.h"
#include "..\JobSystem\JobSystem.h"

namespace Engine
{
	namespace Core
	{
		class CoreFunctions
		{
		public:
			CoreFunctions() {}
			CoreFunctions(std::string i_pFilePath): m_FilePath(i_pFilePath) {}
			Engine::Renderer::Sprites::Sprite* CreateSprite(const char*);
			Engine::Renderer::Sprites::Sprite* ReturnSprite() { return m_CreatedSprite; }
			void* LoadFile(const char*, size_t &);
			unsigned int GetWidth() { return m_width; }
			unsigned int GetHeight() { return m_height; }
		private:
			Engine::Renderer::Sprites::Sprite* m_CreatedSprite;
			std::string m_FilePath;
			unsigned int m_width;
			unsigned int m_height;
			unsigned int  m_depth;
		};

		class FileDetails : public Engine::JobSystem::IJob
		{
		public:
			FileDetails(const char* i_inputFile, lua_State* i_pLuaState) : IJob("FileDetails"), m_filePath(i_inputFile), m_luaState(i_pLuaState) {}
			std::string GetFilePath() { return m_filePath; }
			size_t GetFileSize() { return m_sizeOfFile; }
			uint8_t* GetFileData() { return m_fileData; }
			void SetFilePath(char* i_filePath) { m_filePath = i_filePath; }
			void SetFileSize(size_t i_fileSize) { m_sizeOfFile = i_fileSize; }
			void SetFileData(uint8_t* i_fileData) { m_fileData = i_fileData; }
			void LoadFile(const char*);
			void ProcessFileData();
			void Run();
			Engine::GameObject::GameObject* GetGameObject() { return m_returnGameObject; }
			~FileDetails()
			{
				delete m_fileData;
				delete m_returnGameObject;
			}
		private:
			std::string m_filePath;
			size_t m_sizeOfFile;
			uint8_t* m_fileData;
			Engine::GameObject::GameObject* m_returnGameObject;
			lua_State* m_luaState;
		};
	}
}
