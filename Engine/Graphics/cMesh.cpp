#include "cMesh.h"

#include <External/Lua/Includes.h>

eae6320::Assets::cManager<eae6320::Graphics::cMesh> eae6320::Graphics::cMesh::s_Manager;

namespace
{
	eae6320::Graphics::VertexFormats::sMesh* m_TempMesh;
	eae6320::Graphics::VertexFormats::sIndex* m_TempIndex;
	size_t m_NumberOfVertices;
	size_t m_NumberOfIndices;
}

//Function to load IndexBuffer from file
eae6320::cResult LoadIndexBuffer(lua_State& i_LuaState)
{
	auto result = eae6320::Results::Success;
	const auto indeCount = luaL_len(&i_LuaState, -1);
	for (int i = 1; i <= indeCount; ++i)
	{
		lua_pushinteger(&i_LuaState, i);
		lua_gettable(&i_LuaState, -2);
		m_TempIndex[i - 1].indexValue = static_cast<uint16_t>(lua_tointeger(&i_LuaState, -1));
		lua_pop(&i_LuaState, 1);
	}
	return result;
}

eae6320::cResult LoadVertexData(lua_State& i_LuaState, eae6320::Graphics::VertexFormats::sMesh* io_mesh)
{
	auto result = eae6320::Results::Success;
	constexpr auto* const key = "Position";
	lua_pushstring(&i_LuaState, key);
	lua_gettable(&i_LuaState, -2);
	unsigned int i = 0;
	if (lua_istable(&i_LuaState, -1))
	{
		lua_pushnil(&i_LuaState);
		while (lua_next(&i_LuaState, -2))
		{
			if (i ==0 )
			{
				io_mesh->x = static_cast<float>(lua_tonumber(&i_LuaState, -1));
			}
			else if (i == 1)
			{
				io_mesh->y = static_cast<float>(lua_tonumber(&i_LuaState, -1));
			}
			else if (i == 2)
			{
				io_mesh->z = static_cast<float>(lua_tonumber(&i_LuaState, -1));
			}
			i++;
			lua_pop(&i_LuaState, 1);
		}
		lua_pop(&i_LuaState, 1);
	}
	i = 0;
	constexpr auto* const key2 = "Color";
	lua_pushstring(&i_LuaState, key2);
	lua_gettable(&i_LuaState, -2);
	if (lua_istable(&i_LuaState, -1))
	{
		lua_pushnil(&i_LuaState);
		while (lua_next(&i_LuaState, -2))
		{
			if (i == 0)
			{
				io_mesh->r = static_cast<uint8_t>(lua_tonumber(&i_LuaState, -1)) * 255;
			}
			else if (i == 1)
			{
				io_mesh->g = static_cast<uint8_t>(lua_tonumber(&i_LuaState, -1)) * 255;
			}
			else if (i == 2)
			{
				io_mesh->b = static_cast<uint8_t>(lua_tonumber(&i_LuaState, -1)) * 255;
			}
			else if (i == 3)
			{
				io_mesh->a = static_cast<uint8_t>(lua_tonumber(&i_LuaState, -1)) * 255;
			}
			i++;
			lua_pop(&i_LuaState, 1);
		}
		lua_pop(&i_LuaState, 1);
	}

	return result;
}

//Load all table values from file
eae6320::cResult LoadTableValues(lua_State& i_LuaState)
{
	auto result = eae6320::Results::Success;

	const auto* const key = "VertexBuffer";
	lua_pushstring(&i_LuaState, key);
	lua_gettable(&i_LuaState, -2);
	unsigned int i = 0;
	if (lua_istable(&i_LuaState, -1))
	{
		m_NumberOfVertices = lua_rawlen(&i_LuaState, -1);
		if (m_NumberOfVertices != 0)
		{
			m_TempMesh = new eae6320::Graphics::VertexFormats::sMesh[m_NumberOfVertices];
		}
		lua_pushnil(&i_LuaState);
		while (lua_next(&i_LuaState, -2))
		{
			if (!(result = LoadVertexData(i_LuaState, &m_TempMesh[i])))
			{
				return result;
			}
			i++;
			lua_pop(&i_LuaState, 1);
		}
		lua_pop(&i_LuaState, 1);
	}
	const auto* const key2 = "IndexBuffer";
	lua_pushstring(&i_LuaState, key2);
	lua_gettable(&i_LuaState, -2);
	if (lua_istable(&i_LuaState, -1))
	{
		m_NumberOfIndices = lua_rawlen(&i_LuaState, -1);
		if (m_NumberOfIndices != 0)
		{
			m_TempIndex = new eae6320::Graphics::VertexFormats::sIndex[m_NumberOfIndices];
		}
		if (!(result = LoadIndexBuffer(i_LuaState)))
		{
			return result;
		}
		lua_pop(&i_LuaState, 1);
	}
	return result;
}

//Load file
eae6320::cResult LoadFile(const char* const i_FileName)
{
	auto result = eae6320::Results::Success;
	lua_State* luaState = nullptr;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			return result = eae6320::Results::OutOfMemory;
		}
	}

	const auto stackTopBeforeLoad = lua_gettop(luaState);
	{
		const auto luaResult = luaL_loadfile(luaState, i_FileName);
		if (luaResult != LUA_OK)
		{
			result = eae6320::Results::Failure;
			// Pop the error message
			lua_pop(luaState, 1);
			return result;
		}
	}

	{
		constexpr int argumentCount = 0;
		constexpr int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
		constexpr int noMessageHandler = 0;
		const auto luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
		if (luaResult == LUA_OK)
		{
			// A well-behaved asset file will only return a single value
			const auto returnedValueCount = lua_gettop(luaState) - stackTopBeforeLoad;
			if (returnedValueCount == 1)
			{
				// A correct asset file _must_ return a table
				if (!lua_istable(luaState, -1))
				{
					result = eae6320::Results::InvalidFile;
					// Pop the returned non-table value
					lua_pop(luaState, 1);
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				std::string err = "Asset files must return a single table (instead of " + returnedValueCount;
				eae6320::Logging::OutputError(err.c_str());
				// Pop every value that was returned
				lua_pop(luaState, returnedValueCount);
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Logging::OutputError(lua_tostring(luaState, -1));
			// Pop the error message
			lua_pop(luaState, 1);
		}
	}
	result = LoadTableValues(*luaState);
	return result;
}

//Manager load Function
eae6320::cResult eae6320::Graphics::cMesh::Load(const char * const i_FilePath, cMesh *& o_Mesh)
{
	cResult result = Results::Success;
	cMesh* outputMesh = nullptr;
	if (!(result = LoadFile(i_FilePath)))
	{
		return result;
	}
	if (!(CreateMesh(m_TempMesh, m_TempIndex, static_cast<unsigned int>(m_NumberOfIndices), outputMesh)))
	{
		return result;
	}
	o_Mesh = outputMesh;
	return result;
}