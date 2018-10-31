#include "cEffectBuilder.h"
#include <fstream>
#include <Tools/AssetBuildLibrary/Functions.h>
#include <External/Lua/Includes.h>


namespace
{
	std::string m_VertexShaderLocation;
	std::string m_FragmentShaderLocation;
	uint8_t m_RenderStateValue = 0;
	std::string m_TempRenderState;
}

eae6320::cResult LoadTableValues(lua_State& i_LuaState)
{
	auto result = eae6320::Results::Success;

	const auto* const key = "Effect";
	lua_pushstring(&i_LuaState, key);
	lua_gettable(&i_LuaState, -2);
	if (lua_istable(&i_LuaState, -1))
	{
		auto* key2 = "VertexShaderLocation";
		lua_pushstring(&i_LuaState, key2);
		lua_gettable(&i_LuaState, -2);
		if (lua_isstring(&i_LuaState, -1))
		{
			m_VertexShaderLocation = lua_tostring(&i_LuaState, -1);
		}
		lua_pop(&i_LuaState, 1);

		key2 = "FragmentShaderLocation";
		lua_pushstring(&i_LuaState, key2);
		lua_gettable(&i_LuaState, -2);
		if (lua_isstring(&i_LuaState, -1))
		{
			m_FragmentShaderLocation = lua_tostring(&i_LuaState, -1);
		}
		lua_pop(&i_LuaState, 1);

		key2 = "RenderState";
		lua_pushstring(&i_LuaState, key2);
		lua_gettable(&i_LuaState, -2);
		if (lua_isstring(&i_LuaState, -1))
		{
			m_TempRenderState = lua_tostring(&i_LuaState, -1);
			if (m_TempRenderState == "AlphaTransparency")
			{
				m_RenderStateValue = 0;
			}
			else if (m_TempRenderState == "DepthBuffering")
			{
				m_RenderStateValue = 1;
			}
			else if (m_TempRenderState == "DrawBothTriangleSides")
			{
				m_RenderStateValue = 2;
			}
		}
		lua_pop(&i_LuaState, 1);
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
				eae6320::Assets::OutputErrorMessage(err.c_str());
				// Pop every value that was returned
				lua_pop(luaState, returnedValueCount);
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessage(lua_tostring(luaState, -1));
			// Pop the error message
			lua_pop(luaState, 1);
		}
	}
	result = LoadTableValues(*luaState);
	return result;
}

eae6320::cResult eae6320::Assets::cEffectBuilder::Build(const std::vector<std::string>& i_arguments)
{
	cResult result = Results::Success;
	std::string m_ErrorString;

	result = LoadFile(m_path_source);
	if (!result) { OutputErrorMessageWithFileInfo(m_path_source, "Error Reading file"); }

	FILE* fptr;

	size_t length = m_VertexShaderLocation.length();

	m_VertexShaderLocation = "data/" + m_VertexShaderLocation;
	m_FragmentShaderLocation = "data/" + m_FragmentShaderLocation;

	fptr = fopen(m_path_target, "w+b");
	fwrite(&m_RenderStateValue, sizeof(uint8_t), 1, fptr);
	fwrite(&length, sizeof(uint8_t), 1, fptr);
	fwrite(m_VertexShaderLocation.c_str(), m_VertexShaderLocation.length(), 1, fptr);
	fwrite("\0", sizeof(uint8_t), 1, fptr);
	fwrite(m_FragmentShaderLocation.c_str(), m_FragmentShaderLocation.length(), 1, fptr);
	fwrite("\0", sizeof(uint8_t), 1, fptr);
	fclose(fptr);
	return result;
}