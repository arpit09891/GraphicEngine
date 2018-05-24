#include "Mesh.h"
#include"Tools\MeshBuilder\cMeshBuilder.h"
#include "Tools\AssetBuildLibrary\Functions.h"
#include <Engine/Asserts/Asserts.h>
#include <iostream>

eae6320::Assets::cManager<Mesh> Mesh::s_manager;

eae6320::cResult Mesh::Load(const char* const i_path, Mesh*& o_mesh)
{
	auto result = eae6320::Results::Success;
	o_mesh->Factory(o_mesh, i_path);
	
//	result = o_mesh->ReadMeshDetailsValues(i_path);
	result = o_mesh->LoadBinaryData(i_path);
	//o_mesh->indicesCount = static_cast<uint16_t>(o_mesh->indexData.length);

	//o_mesh->verticesCount = static_cast<uint16_t>(o_mesh->vertexData.size());
	//result = o_mesh->InitializeGeometry();
	return result;
}

eae6320::cResult Mesh::Factory(Mesh *& mesh, const char* const i_path)
{
	auto result = eae6320::Results::Success;

	Mesh * tempMesh = new Mesh();
	mesh = tempMesh;

	if (mesh == nullptr)
	{
		result = eae6320::Results::Failure;
		goto OnExit;
	}

OnExit:
	return result;
}


eae6320::cResult Mesh::LoadBinaryData(const char* const i_path)
{
	auto result = eae6320::Results::Success;
	// Load the compiled binary vertex shader for the input layout
	eae6320::Platform::sDataFromFile meshDataFromFile;
	std::string errorMessage;
	if (result = eae6320::Platform::LoadBinaryFile(i_path, meshDataFromFile, &errorMessage))
	{
		auto currentOffset = reinterpret_cast<uintptr_t>(meshDataFromFile.data);
		const auto finalOffset = currentOffset + meshDataFromFile.size;

		//Getting vertex count
		verticesCount = *reinterpret_cast<uint16_t*>(currentOffset);
		/*verticesCount = vertexCount;*/

		//Getting index count
		currentOffset += sizeof(verticesCount);
		indicesCount = *reinterpret_cast<uint16_t*>(currentOffset);

		//Getting vertex data 
		currentOffset += sizeof(indicesCount);
		vertexData = new eae6320::Graphics::VertexFormats::sMesh[verticesCount];
		vertexData = reinterpret_cast<eae6320::Graphics::VertexFormats::sMesh*>(currentOffset);
		
		//Getting index data
		currentOffset += sizeof(eae6320::Graphics::VertexFormats::sMesh) * verticesCount;
		indexData = new uint16_t[indicesCount];
		indexData = reinterpret_cast<uint16_t*>(currentOffset);

		//meshDataFromFile.Free();

	}
	else
	{
		EAE6320_ASSERTF(false, errorMessage.c_str());
		eae6320::Logging::OutputError("The mesh file couldn't be loaded: %s", errorMessage.c_str());		
	}

	result = InitializeGeometry();
	meshDataFromFile.Free();
	
	return result;
}
