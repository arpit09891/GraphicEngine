/*
This class builds hardware-ready meshes from source images
*/

#ifndef EAE6320_CMESHBUILDER_H
#define EAE6320_CMESHBUILDER_H

// Include Files
//==============

#include <Tools/AssetBuildLibrary/cbBuilder.h>
#include <External/Lua/Includes.h>
// Class Declaration
//==================

namespace eae6320
{
	namespace meshStruct{
		struct sMesh
		{
			// POSITION
			// 3 floats == 12 bytes
			// Offset = 0
			float x, y, z;

			// TEXCOORD0
			// 2 floats == 8 bytes
			// Offset = ?
			float u, v;

			//Colors
			uint8_t r, g, b, a;
		};
	}
	namespace Assets
	{
		class cMeshBuilder : public cbBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			virtual cResult Build(const std::vector<std::string>& i_arguments) override;
			eae6320::cResult ReadMeshDetailsValues();
			eae6320::cResult LoadAsset(const char* const i_path);
			
			eae6320::cResult LoadTableValues(lua_State& io_luaState);

			//TODO have to edit them according to mesh file requirements
			eae6320::cResult LoadTableValues_vertices(lua_State& io_luaState);
			eae6320::cResult LoadTableValues_vertices_values(lua_State& io_luaState);

			eae6320::cResult LoadTableValues_vertices_position(lua_State& io_luaState);
			eae6320::cResult LoadTableValues_vertices_position_values(lua_State& io_luaState);
			
			eae6320::cResult LoadTableValues_vertices_colors(lua_State& io_luaState);
			eae6320::cResult LoadTableValues_vertices_colors_values(lua_State& io_luaState);

			eae6320::cResult LoadTableValues_vertices_uv(lua_State& io_luaState);
			eae6320::cResult LoadTableValues_vertices_uv_values(lua_State& io_luaState);
			
			eae6320::cResult LoadTableValues_indices(lua_State& io_luaState);
			eae6320::cResult LoadTableValues_indices_values(lua_State& io_luaState);

			eae6320::cResult LoadTableValues_position(lua_State& io_luaState);
			eae6320::cResult LoadTableValues_positions_values(lua_State& io_luaState);
			void CreateBinaryFile();
			FILE* pFile;

			uint16_t indicesCount;
			uint16_t verticesCount;
			eae6320::meshStruct::sMesh* vertexData;
			uint16_t* indexData;
			int count;

		};
	}
}

#endif	// EAE6320_CMESHBUILDER_H
