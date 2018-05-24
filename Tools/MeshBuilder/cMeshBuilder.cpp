#include"cMeshBuilder.h"
#include <Engine/Platform/Platform.h>
#include "Tools\AssetBuildLibrary\Functions.h"
#include <Engine/Asserts/Asserts.h>
#include <iostream>
//#include"MeshLuaReader.h"


eae6320::cResult eae6320::Assets::cMeshBuilder::Build(const std::vector<std::string>& i_arguments)
{	
	eae6320::cResult result = eae6320::Results::Success;
	// Copy the file
	{
		std::string errorMessage;
		// There are many reasons that a source should be rebuilt,
		// and so even if the target already exists it should just be written over
		constexpr bool noErrorIfTargetAlreadyExists = false;
		// Since we rely on timestamps to determine when a target was built
		// its file time should be updated when the source gets copied
		constexpr bool updateTheTargetFileTime = true;
		/*if (eae6320::Platform::CopyFile(m_path_source, m_path_target, noErrorIfTargetAlreadyExists, updateTheTargetFileTime, &errorMessage))
		{*/
			pFile = fopen(m_path_target, "wb");
			std::cout << "Path Target !!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
		/*}
		else
		{
			result = eae6320::Results::Failure;
			eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__,
				"Mesh File Copying Failed");
		}*/
	}
	result = ReadMeshDetailsValues();
	//result = ReadMeshDetailsValues();
	std::cout << "Path Target ----------------------" << m_path_target << std::endl;
	CreateBinaryFile();
	fclose(pFile);
	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::ReadMeshDetailsValues()
{
	auto result = eae6320::Results::Success;
	//constexpr auto* const path = "readNestedTableValues.lua";
	if (!(result = LoadAsset(m_path_source)))
	{
		return result;
	}

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadAsset(const char * const i_path)
{
	auto result = eae6320::Results::Success;

	// Create a new Lua state
	lua_State* luaState = nullptr;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			result = eae6320::Results::OutOfMemory;
			eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__,
				"Failed to create a new Lua state");
			goto OnExit;
		}
	}

	// Load the asset file as a "chunk",
	// meaning there will be a callable function at the top of the stack
	const auto stackTopBeforeLoad = lua_gettop(luaState);
	{
		const auto luaResult = luaL_loadfile(luaState, i_path);
		if (luaResult != LUA_OK)
		{
			result = eae6320::Results::Failure;
			eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__,
				lua_tostring(luaState, -1));
			//std::cerr << lua_tostring(luaState, -1) << std::endl;
			// Pop the error message
			lua_pop(luaState, 1);
			goto OnExit;
		}
	}
	// Execute the "chunk", which should load the asset
	// into a table at the top of the stack
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
					eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__,
						"Asset files must return a table (instead of a value)");
					/*std::cerr << "Asset files must return a table (instead of a " <<
						luaL_typename(luaState, -1) << ")" << std::endl;*/
					// Pop the returned non-table value
					lua_pop(luaState, 1);
					goto OnExit;
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__,
					"Asset files must return a single table (instead of values)");
				/*std::cerr << "Asset files must return a single table (instead of " <<
					returnedValueCount << " values)" << std::endl;*/
				// Pop every value that was returned
				lua_pop(luaState, returnedValueCount);
				goto OnExit;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__,
				lua_tostring(luaState, -1));
			/*std::cerr << lua_tostring(luaState, -1) << std::endl;*/
			// Pop the error message
			lua_pop(luaState, 1);
			goto OnExit;
		}
	}

	// If this code is reached the asset file was loaded successfully,
	// and its table is now at index -1
	result = LoadTableValues(*luaState);

	// Pop the table
	lua_pop(luaState, 1);

OnExit:

	if (luaState)
	{
		// If I haven't made any mistakes
		// there shouldn't be anything on the stack,
		// regardless of any errors encountered while loading the file:
		//TODO removeAssert
		
		//EAE6320_ASSERT(lua_gettop(luaState) == 0);

		if (lua_gettop(luaState) != 0)
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__, "luaState is not 0");
		}
		lua_close(luaState);
		luaState = nullptr;
	}

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues(lua_State & io_luaState)
{
	auto result = eae6320::Results::Success;

	if (!(result = LoadTableValues_vertices(io_luaState)))
	{
		return result;
	}

	if (!(result = LoadTableValues_indices(io_luaState)))
	{
		return result;
	}

	/*if (!(result = LoadTableValues_position(io_luaState)))
	{
		return result;
	}
*/
	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_vertices(lua_State & io_luaState)
{
	auto result = eae6320::Results::Success;

	constexpr auto* const key = "vertexData";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	
	if (lua_istable(&io_luaState, -1))
	{
		if (!(result = LoadTableValues_vertices_values(io_luaState)))
		{
			goto OnExit;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;

		eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__, "This value at this key must be a table");
	/*	std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;*/
		goto OnExit;
	}

OnExit:

	// Pop the vertexData table
	lua_pop(&io_luaState, 1);

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_vertices_values(lua_State & io_luaState)
{	
	auto result = eae6320::Results::Success;

	std::cout << "Iterating through every vertex table" << std::endl;
	const auto vertexCount = luaL_len(&io_luaState, -1);
	verticesCount = static_cast<uint16_t>(vertexCount);	
	count = 0;
	vertexData = new eae6320::meshStruct::sMesh[verticesCount];
	for (int i = 1; i <= vertexCount; ++i)
	{
		std::cout << "on Vertex Data " << i << std::endl;
		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);
		
		if (lua_istable(&io_luaState, -1))
		{
			if (!(result = LoadTableValues_vertices_position(io_luaState)))
			{
				goto OnExit;
			}

			if (!(result = LoadTableValues_vertices_colors(io_luaState)))
			{
				goto OnExit;
			}
			if (!(result = LoadTableValues_vertices_uv(io_luaState)))
			{
				goto OnExit;
			}
			count++;
			//pop vertex value table
			lua_pop(&io_luaState, 1);
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__, "This value at this key must be a table");
			/*std::cerr << "The value at \"" << i << "\" must be a table "
				"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;*/
			lua_pop(&io_luaState, 1);
			goto OnExit;
		}
	}
	
OnExit:
	
	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_vertices_position(lua_State & io_luaState)
{
	auto result = eae6320::Results::Success;

	constexpr auto* const key = "position";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	
	if (lua_istable(&io_luaState, -1))
	{
		if (!(result = LoadTableValues_vertices_position_values(io_luaState)))
		{
			goto OnExit;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__, "This value at this key must be a table");
		/*std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;*/
		goto OnExit;
	}

OnExit:
	// Pop the position table
	lua_pop(&io_luaState, 1);

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_vertices_position_values(lua_State & io_luaState)
{
	auto result = eae6320::Results::Success;

	std::cout << "Iterating through every positon valuess:" << std::endl;
	const auto textureCount = luaL_len(&io_luaState, -1);
	//x value
	{
		lua_pushinteger(&io_luaState, 1);
		lua_gettable(&io_luaState, -2);
		vertexData[count].x = static_cast<float>(lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
	}
	//y value
	{
		lua_pushinteger(&io_luaState, 2);
		lua_gettable(&io_luaState, -2);
		vertexData[count].y = static_cast<float>(lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
	}

	//z value
	{
		lua_pushinteger(&io_luaState, 3);
		lua_gettable(&io_luaState, -2);
		vertexData[count].z = static_cast<float>(lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
	}

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_vertices_colors(lua_State & io_luaState)
{
	auto result = eae6320::Results::Success;

	constexpr auto* const key = "color";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);

	if (lua_istable(&io_luaState, -1))
	{
		if (!(result = LoadTableValues_vertices_colors_values(io_luaState)))
		{
			goto OnExit;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__, "This value at this key must be a table");
		/*std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;*/
		goto OnExit;
	}

OnExit:

	// Pop the color table
	lua_pop(&io_luaState, 1);

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_vertices_colors_values(lua_State & io_luaState)
{
	auto result = eae6320::Results::Success;

	std::cout << "Iterating through every color value:" << std::endl;
	const auto colorCount = luaL_len(&io_luaState, -1);
	{
		lua_pushinteger(&io_luaState, 1);
		lua_gettable(&io_luaState, -2);
		float floatValue = static_cast<float>(lua_tonumber(&io_luaState, -1));
		const auto convertedValue = static_cast<uint8_t>(floatValue * 255.0f);
		vertexData[count].r = convertedValue;
		lua_pop(&io_luaState, 1);
	}
	{
		lua_pushinteger(&io_luaState, 2);
		lua_gettable(&io_luaState, -2);
		float floatValue = static_cast<float>(lua_tonumber(&io_luaState, -1));
		const auto convertedValue = static_cast<uint8_t>(floatValue * 255.0f);
		vertexData[count].g = convertedValue;
		lua_pop(&io_luaState, 1);
	}
	{
		lua_pushinteger(&io_luaState, 3);
		lua_gettable(&io_luaState, -2);
		float floatValue = static_cast<float>(lua_tonumber(&io_luaState, -1));
		const auto convertedValue = static_cast<uint8_t>(floatValue * 255.0f);
		vertexData[count].b = convertedValue;
		lua_pop(&io_luaState, 1);
	}
	{
		lua_pushinteger(&io_luaState, 4);
		lua_gettable(&io_luaState, -2);
		float floatValue = static_cast<float>(lua_tonumber(&io_luaState, -1));
		const auto convertedValue = static_cast<uint8_t>(floatValue * 255.0f);
		vertexData[count].a = convertedValue;
		lua_pop(&io_luaState, 1);
	}

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_vertices_uv(lua_State & io_luaState)
{
	auto result = eae6320::Results::Success;

	constexpr auto* const key = "uv";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);

	if (lua_istable(&io_luaState, -1))
	{
		if (!(result = LoadTableValues_vertices_uv_values(io_luaState)))
		{
			goto OnExit;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__, "This value at this key must be a table");
		/*std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;*/
		goto OnExit;
	}

OnExit:

	// Pop the uv table
	lua_pop(&io_luaState, 1);

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_vertices_uv_values(lua_State & io_luaState)
{
	auto result = eae6320::Results::Success;

	std::cout << "Iterating through every uv value:" << std::endl;
	const auto uvCount = luaL_len(&io_luaState, -1);

	{
		lua_pushinteger(&io_luaState, 1);
		lua_gettable(&io_luaState, -2);
		vertexData[count].u = static_cast<float>(lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
	}
	{
		lua_pushinteger(&io_luaState, 2);
		lua_gettable(&io_luaState, -2);
		vertexData[count].v = static_cast<float>(lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
	}

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_indices(lua_State & io_luaState)
{
	auto result = eae6320::Results::Success;

	constexpr auto* const key = "indexData";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	if (lua_istable(&io_luaState, -1))
	{
		if (!(result = LoadTableValues_indices_values(io_luaState)))
		{
			goto OnExit;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__, "This value at this key must be a table");
		/*std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;*/
		goto OnExit;
	}

OnExit:

	// Pop the indexValue table
	lua_pop(&io_luaState, 1);

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_indices_values(lua_State & io_luaState)
{
	auto result = eae6320::Results::Success;

	std::cout << "Iterating through every index value:" << std::endl;
	const auto indexCount = luaL_len(&io_luaState, -1);
	indicesCount = static_cast<uint16_t>(indexCount);
	indexData = new uint16_t[indicesCount];
	for (int i = 1; i <= indexCount; ++i)
	{
		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);
		uint16_t index = static_cast<uint16_t>(lua_tonumber(&io_luaState, -1));
		indexData[i-1] = (index);
		lua_pop(&io_luaState, 1);
	}

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_position(lua_State & io_luaState)
{
	auto result = eae6320::Results::Success;

	constexpr auto* const key = "meshPosition";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	if (lua_istable(&io_luaState, -1))
	{
		if (!(result = LoadTableValues_positions_values(io_luaState)))
		{
			goto OnExit;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		eae6320::Assets::OutputErrorMessageWithFileInfo(__FILE__, __LINE__, "This value at this key must be a table");
		/*std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;*/
		goto OnExit;
	}

OnExit:

	// Pop the parameters table
	lua_pop(&io_luaState, 1);

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_positions_values(lua_State & io_luaState)
{	
	auto result = eae6320::Results::Success;

	std::cout << "Iterating through the meshPosition:" << std::endl;
	const auto count = luaL_len(&io_luaState, -1);
	for (int i = 1; i <= count; ++i)
	{
		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);
		std::cout << "\t" << lua_tostring(&io_luaState, -1) << std::endl;
		lua_pop(&io_luaState, 1);
	}

	return result;
}

void eae6320::Assets::cMeshBuilder::CreateBinaryFile()
{
	//winding order
#if defined( EAE6320_PLATFORM_D3D )
	uint16_t temp;
	for (int i = 0; i < indicesCount; i++)
	{
		if (i % 3 == 1)
		{
			temp = indexData[i];
			indexData[i] = indexData[i + 1];
			indexData[i + 1] = temp;
		}
	}

	for (int i = 0; i< verticesCount; i++)
	{
		vertexData[i].v = (1 - (vertexData[i].v));

	}

#elif defined( EAE6320_PLATFORM_GL )


#endif
	//vertices Count
	uint16_t* numberOfVertices = &verticesCount;
	//void* nov = reinterpret_cast<void*>(numberOfVertices);
	fwrite(&verticesCount, sizeof(uint16_t), 1, pFile);

	//indices count 
	uint16_t* numberOfIndices = &indicesCount;
	//void* noi = reinterpret_cast<void*>(numberOfIndices);
	fwrite(&indicesCount, sizeof(uint16_t), 1, pFile);

	//vertex Array
	//void* voidVertexData = reinterpret_cast<void*>(vertexData);
	fwrite(vertexData, sizeof(eae6320::meshStruct::sMesh), verticesCount, pFile);

	//index Array
	//void* voidindexData = reinterpret_cast<void*>(indexData);
	fwrite(indexData, sizeof(uint16_t), indicesCount, pFile);
}









