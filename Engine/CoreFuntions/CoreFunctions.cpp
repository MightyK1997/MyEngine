#include "CoreFunctions.h"


using namespace Engine;
using namespace Renderer;
Engine::Renderer::Sprites::Sprite* Engine::Core::CoreFunctions::CreateSprite(const char* i_pFileName)
{
		assert(i_pFileName);

		size_t sizeTextureFile = 0;

		// Load the source file (texture data)
		void * pTextureFile = LoadFile(i_pFileName, sizeTextureFile);

		// Ask GLib to create a texture out of the data (assuming it was loaded successfully)
		Texture * pTexture = pTextureFile ? CreateTexture(pTextureFile, sizeTextureFile) : nullptr;

		// exit if something didn't work
		// probably need some debug logging in here!!!!
		if (pTextureFile)
			delete[] pTextureFile;

		if (pTexture == nullptr)
			return NULL;

		m_width = 0;
		m_height = 0;
		m_depth = 0;

		// Get the dimensions of the texture. We'll use this to determine how big it is on screen
		bool result = GetDimensions(pTexture, m_width, m_height, m_depth);
		assert(result == true);
		assert((m_width > 0) && (m_height > 0));

		// Define the sprite edges
		Sprites::SpriteEdges	Edges = { -float(m_width / 2.0f), float(m_height), float(m_width / 2.0f), 0.0f };
		Sprites::SpriteUVs	UVs = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } };
		RGBA							Color = { 255, 255, 255, 255 };

		// Create the sprite
		Engine::Renderer::Sprites::Sprite * pSprite = Engine::Renderer::Sprites::CreateSprite(Edges, 0.1f, Color, UVs);
		if (pSprite == nullptr)
		{
			Release(pTexture);
			return nullptr;
		}

		// Bind the texture to sprite
		Sprites::SetTexture(*pSprite, *pTexture);

		m_CreatedSprite = pSprite;
		return pSprite;
}

void* Engine::Core::CoreFunctions::LoadFile(const char* i_pFilename, size_t & o_sizeFile)
{
	assert(i_pFilename != NULL);

	FILE * pFile = NULL;

	errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
	if (fopenError != 0)
		return NULL;

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

	o_sizeFile = FileSize;
	
	return pBuffer;
}

void Engine::Core::FileDetails::ProcessFileData()
{
	m_returnGameObject = new Engine::GameObject::GameObject();
	int result = 0;
	char* file = reinterpret_cast<char*>(m_fileData);
	result = luaL_loadbuffer(m_luaState, reinterpret_cast<char*>(m_fileData), m_sizeOfFile, NULL);
	assert(result == 0);
	result = lua_pcall(m_luaState, 0, 0, 0);
	assert(result == 0);

	int type = lua_getglobal(m_luaState, "GameObject");
	assert(type == LUA_TTABLE);

	lua_pushstring(m_luaState, "name");
	type = lua_gettable(m_luaState, -2);
	assert(type == LUA_TSTRING);
	std::string gameObjectName;
	if (type == LUA_TSTRING)
	{
		gameObjectName = lua_tostring(m_luaState, -1);
		ConsolePrint_Windows(gameObjectName.c_str(), __LINE__, __FILE__, "lua");
	}

	lua_pop(m_luaState, 1);

	lua_pushstring(m_luaState, "class");
	type = lua_gettable(m_luaState, -2);
	std::string gameObjectClass;
	if (type == LUA_TSTRING)
	{
		gameObjectClass = lua_tostring(m_luaState, -1);
		ConsolePrint_Windows(gameObjectClass.c_str(), __LINE__, __FILE__, "lua");
	}

	lua_pop(m_luaState, 1);

	float postionFloats[3];
	int someValue = 0;
	lua_pushstring(m_luaState, "position");
	type = lua_gettable(m_luaState, -2);
	if (type == LUA_TTABLE)
	{
		lua_pushnil(m_luaState);
		while (lua_next(m_luaState, -2) != 0)
		{
			type = lua_type(m_luaState, -1);
			assert(type == LUA_TNUMBER);

			postionFloats[someValue] = (float)lua_tonumber(m_luaState, -1);
			someValue++;
			lua_pop(m_luaState, 1);
		}
	}

	lua_pop(m_luaState, 1);

	float rotationFloats[3];
	someValue = 0;
	lua_pushstring(m_luaState, "rotation");
	type = lua_gettable(m_luaState, -2);
	if (type == LUA_TTABLE)
	{
		lua_pushnil(m_luaState);
		while (lua_next(m_luaState, -2) != 0)
		{
			type = lua_type(m_luaState, -1);
			assert(type == LUA_TNUMBER);

			rotationFloats[someValue] = (float)lua_tonumber(m_luaState, -1);
			someValue++;
			lua_pop(m_luaState, 1);
		}
	}

	lua_pop(m_luaState, 1);

	int temp;
	lua_pushstring(m_luaState, "isKinematic");
	type = lua_gettable(m_luaState, -2);
	assert(type == LUA_TBOOLEAN);
	temp = lua_toboolean(m_luaState, -1);
	ConsolePrint_Windows(gameObjectClass.c_str(), temp, __FILE__, "lua");

	lua_pop(m_luaState, 1);

	std::string spriteFileLocation;
	lua_pushstring(m_luaState, "sprite");
	type = lua_gettable(m_luaState, -2);
	assert(type == LUA_TTABLE);
	if (type == LUA_TTABLE)
	{
		lua_pushstring(m_luaState, "fileLocation");
		type = lua_gettable(m_luaState, -2);
		assert(type == LUA_TSTRING);
		if (type == LUA_TSTRING)
		{
			spriteFileLocation = lua_tostring(m_luaState, -1);
			ConsolePrint_Windows(spriteFileLocation.c_str(), __LINE__, __FILE__, "lua");
		}
	}

	lua_pop(m_luaState, 1);
	ConsolePrint_Windows("In Lua things", __LINE__, __FILE__, "LuaThing");
	Engine::Core::CoreFunctions co;
	co.CreateSprite(spriteFileLocation.c_str());
	m_returnGameObject->SetGameObjectSprite((co.ReturnSprite()));
	m_returnGameObject->SetGameObjectPosition(Vector3D(postionFloats[0], postionFloats[1], postionFloats[2]));
	m_returnGameObject->SetGameObjectRotation(Vector3D(rotationFloats[0], rotationFloats[1], rotationFloats[2]));
	m_returnGameObject->SetGameObjectAABB(Vector3D(static_cast<float>(co.GetWidth() / 2), static_cast<float>(co.GetHeight() / 2), 0), Vector3D(static_cast<float>(co.GetWidth() / 2) , static_cast<float>(co.GetHeight()/2), 0));
	if (temp == 0)
	{
		m_returnGameObject->SetIsKinematic(true);
	}
	else
	{
		m_returnGameObject->SetIsKinematic(false);
	}
	m_returnGameObject->SetGameObjectName(gameObjectName);
	m_returnGameObject->SetGameObjectClass(gameObjectClass);
	Engine::GameObject::m_allGameObjects->operator[](gameObjectName) = *m_returnGameObject;
}

void Engine::Core::FileDetails::LoadFile(const char* i_pFilepath)
{
	assert(i_pFilepath != NULL);

	FILE * pFile = NULL;

	errno_t fopenError = fopen_s(&pFile, i_pFilepath, "rb");
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

void Engine::Core::FileDetails::Run()
{
	LoadFile(m_filePath.c_str());
	ProcessFileData();
}
