#pragma once

#include "../Graphics/VertexFormats.h"
#include"../Platform/Platform.h"
#include"../Assets/ReferenceCountedAssets.h"
#include <Engine/Assets/cHandle.h>
#include <Engine/Assets/cManager.h>
#include <External/Lua/Includes.h>
#include"Engine\Physics\sRigidBodyState.h"

#if defined( EAE6320_PLATFORM_D3D )
struct ID3D11Buffer;
struct ID3D11InputLayout;
struct ID3D11DeviceContext;

#elif EAE6320_PLATFORM_GL
#include"OpenGL\Includes.h"
#endif

class Mesh {

	EAE6320_ASSETS_DECLAREREFERENCECOUNT();
public:
	
	using Handle = eae6320::Assets::cHandle<Mesh>;
	static eae6320::Assets::cManager<Mesh> s_manager;

	static eae6320::cResult Load(const char* const i_path, Mesh*& o_Mesh);
	static eae6320::cResult Factory(Mesh*& s_Mesh, const char* const i_path);
	void Draw();
	//for testing 
	
	EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();

#if EAE6320_PLATFORM_D3D
	ID3D11Buffer* s_vertexBuffer = nullptr;
	ID3D11Buffer* s_indexBuffer = nullptr;
	ID3D11InputLayout* s_vertexInputLayout = nullptr;
	ID3D11DeviceContext* direct3dImmediateContext;
	void CleanUp();
#elif EAE6320_PLATFORM_GL
	GLuint s_vertexArrayId = 0;
	GLuint s_vertexBufferId = 0;
	GLuint s_indexBufferId = 1;
	eae6320::cResult CleanUp();
#endif
	eae6320::cResult InitializeGeometry();
	EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(Mesh);
	eae6320::cResult LoadBinaryData(const char* const i_path);

	//eae6320::cResult ReadMeshDetailsValues(const char* const i_path);
	//eae6320::cResult LoadAsset(const char* const i_path);

	//eae6320::cResult LoadTableValues(lua_State& io_luaState);

	////TODO have to edit them according to mesh file requirements
	//eae6320::cResult LoadTableValues_vertices(lua_State& io_luaState);
	//eae6320::cResult LoadTableValues_vertices_values(lua_State& io_luaState);

	//eae6320::cResult LoadTableValues_vertices_position(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sMesh& tempMesh);
	//eae6320::cResult LoadTableValues_vertices_position_values(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sMesh& tempMesh);

	//eae6320::cResult LoadTableValues_vertices_colors(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sMesh& tempMesh);
	//eae6320::cResult LoadTableValues_vertices_colors_values(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sMesh& tempMesh);

	//eae6320::cResult LoadTableValues_vertices_uv(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sMesh& tempMesh);
	//eae6320::cResult LoadTableValues_vertices_uv_values(lua_State& io_luaState, eae6320::Graphics::VertexFormats::sMesh& tempMesh);


	//eae6320::cResult LoadTableValues_indices(lua_State& io_luaState);
	//eae6320::cResult LoadTableValues_indices_values(lua_State& io_luaState);

	//eae6320::cResult LoadTableValues_position(lua_State& io_luaState);
	//eae6320::cResult LoadTableValues_positions_values(lua_State& io_luaState);
	//eae6320::Physics::sRigidBodyState RigidBody;

private:
	uint16_t indicesCount;
	uint16_t verticesCount;
	eae6320::Graphics::VertexFormats::sMesh* vertexData;
	uint16_t* indexData;
	
	Mesh() = default;
	~Mesh() = default;
};