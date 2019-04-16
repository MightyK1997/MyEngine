#include "cMaterialBuilder.h"
#include "Tools/AssetBuildLibrary/Functions.h"

#include "Tools/TextureBuilder/cTextureBuilder.h"
#include <External/Lua/Includes.h>
#include <algorithm>
#include "Engine/Graphics/GraphicsHelper.h"
#include "Engine/Graphics/sRGB.h"

namespace
{
	std::string m_EffectLocation;
	uint8_t m_ConstantType;
	uint8_t m_ConstantVariant;
	std::vector<float> m_ConstantData;
	std::string m_TextureLocation;
}


eae6320::cResult LoadTableValues(lua_State& i_LuaState)
{
	auto result = eae6320::Results::Success;
	const auto* const key = "Material";
	lua_pushstring(&i_LuaState, key);
	lua_gettable(&i_LuaState, -2);
	if (lua_istable(&i_LuaState, -1))
	{
		auto* key2 = "EffectLocation";
		lua_pushstring(&i_LuaState, key2);
		lua_gettable(&i_LuaState, -2);
		if (lua_isstring(&i_LuaState, -1))
		{
			m_EffectLocation = lua_tostring(&i_LuaState, -1);
		}
		lua_pop(&i_LuaState, 1);

		key2 = "ConstantType";
		lua_pushstring(&i_LuaState, key2);
		lua_gettable(&i_LuaState, -2);
		if (lua_isstring(&i_LuaState, -1))
		{
			std::string tempConstType = lua_tostring(&i_LuaState, -1);
			if (tempConstType == "float")
			{
				m_ConstantType = 0;
			}
			else if (tempConstType == "float2")
			{
				m_ConstantType = 1;
			}
			else if(tempConstType == "float3")
			{
				m_ConstantType = 2;
			}
			else if(tempConstType == "float4")
			{
				m_ConstantType = 3;
			}
			else if(tempConstType =="float4x4")
			{
				m_ConstantType = 4;
			}
		}
		lua_pop(&i_LuaState, 1);

		key2 = "ConstantName";
		lua_pushstring(&i_LuaState, key2);
		lua_gettable(&i_LuaState, -2);
		if (lua_isstring(&i_LuaState, -1))
		{
			if (lua_tostring(&i_LuaState, -1) == "Color")
			{
				m_ConstantVariant = 1<<0;
			}
		}
		lua_pop(&i_LuaState, 1);

		key2 = "ConstantValue";
		lua_pushstring(&i_LuaState, key2);
		lua_gettable(&i_LuaState, -2);
		if (lua_istable(&i_LuaState, -1))
		{
			lua_pushnil(&i_LuaState);
			while (lua_next(&i_LuaState, -2))
			{
				m_ConstantData.push_back(static_cast<float>(lua_tonumber(&i_LuaState, -1)));
				lua_pop(&i_LuaState, 1);
			}
			lua_pop(&i_LuaState, 1);
		}
		

		key2 = "TextureLocation";
		lua_pushstring(&i_LuaState, key2);
		lua_gettable(&i_LuaState, -2);
		if (lua_isstring(&i_LuaState, -1))
		{
			m_TextureLocation = lua_tostring(&i_LuaState, -1);
		}
		else
		{
			m_TextureLocation = "Textures/default_diffuse.tga";
		}
		lua_pop(&i_LuaState, 1);
		lua_pop(&i_LuaState, 1);
	}
	return result;
}

eae6320::cResult LoadFile(const char* const i_FileName)
{
	eae6320::cResult result = eae6320::Results::Success;

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


eae6320::cResult eae6320::Assets::cMaterialBuilder::Build(const std::vector<std::string>& i_arguments)
{
	cResult result = Results::Success;
	std::string m_errorString;
	result = LoadFile(m_path_source);

	if (!result) { OutputErrorMessageWithFileInfo(m_path_source, "Error Reading file"); }
	std::vector<uint8_t> defaultColor = { 0,0,0,1 };
	//Writing to file
	FILE * fptr;

	std::string o_SourceDirectory;
	std::string o_DestinationDir;
	std::string o_OutputDir;
	std::string o_ErrorMessage;

	std::string tempTextureLocation = m_TextureLocation;

	//Calling texture builder.
	std::replace(m_TextureLocation.begin(), m_TextureLocation.end(), '/', '\\');
	eae6320::Platform::GetEnvironmentVariable("GameSourceContentDir", o_SourceDirectory, &o_ErrorMessage);
	o_SourceDirectory = o_SourceDirectory + m_TextureLocation;
	if (!eae6320::Platform::DoesFileExist(o_SourceDirectory.c_str(), &o_ErrorMessage))
	{
		eae6320::Platform::GetEnvironmentVariable("EngineSourceContentDir", o_SourceDirectory, &o_ErrorMessage);
		o_SourceDirectory = o_SourceDirectory + m_TextureLocation;
	}
	m_TextureLocation = "data\\" + m_TextureLocation;
	eae6320::Platform::GetEnvironmentVariable("GameInstallDir", o_DestinationDir, &o_ErrorMessage);
	o_DestinationDir = o_DestinationDir + m_TextureLocation;

	eae6320::Platform::GetEnvironmentVariable("OutputDir", o_OutputDir, &o_ErrorMessage);

	std::string commandToTextureBuilder = o_OutputDir + "\\TextureBuilder.exe" + " " + o_SourceDirectory + " " + o_DestinationDir + " " + std::to_string(0);

	std::system(commandToTextureBuilder.c_str());


	//Writing to material binary file
	fptr = fopen(m_path_target, "w+b");
	m_EffectLocation = "data/" + m_EffectLocation + "binary";
	m_TextureLocation = "data/" + tempTextureLocation;
	fwrite(m_EffectLocation.c_str(), m_EffectLocation.length(), 1, fptr);
	fwrite("\0", sizeof(uint8_t), 1, fptr);
	fwrite(&m_ConstantType, sizeof(uint8_t), 1, fptr);
	fwrite(&m_ConstantVariant, sizeof(uint8_t), 1, fptr);
	if (m_ConstantData.size() < 1)
	{
		for (auto x : defaultColor)
		{
			fwrite(&x, sizeof(float), 1, fptr);
		}
	}
	else
	{
		for (auto x : m_ConstantData)
		{
			fwrite(&x, sizeof(float), 1, fptr);
		}
	}
	fwrite(m_TextureLocation.c_str(), m_TextureLocation.length(), 1, fptr);
	fwrite("\0", sizeof(uint8_t), 1, fptr);
	fclose(fptr);
	return result;
}
