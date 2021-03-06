#include "OpaqueMesh.h"

Effects * OpaqueMesh::EffectData(char * vertexTarget, char * fragmentTarget, uint8_t defaultRenderState)
{
	auto result = eae6320::Results::Success;
	Effects * tempEffect;

	bool check = Effects::Factory(tempEffect);

	if (!check)
		return nullptr;

	result = tempEffect->InitializeShadingData(vertexTarget, fragmentTarget, defaultRenderState);
	if (!result)
		return nullptr;

	return tempEffect;
}

Mesh * OpaqueMesh::SubmitMeshData(const char * const i_path)
{
	auto result = eae6320::Results::Success;
	Mesh * tempMesh;

	result = Mesh::Factory(tempMesh, i_path);


	RigidBody.velocity.x = 0.f;
	RigidBody.velocity.y = 0.f;
	RigidBody.velocity.z = 0.f;

	if (!result)
		return nullptr;

	return tempMesh;
}

void OpaqueMesh::UpdatePositionBasedOnTimeAndVelocity(float time)
{
	RigidBody.position = RigidBody.position + (RigidBody.velocity * time);
}

OpaqueMesh OpaqueMesh::BindMeshAndEffect(Mesh *i_Mesh, Effects *i_Effect)
{
	mesh = i_Mesh;
	effect = i_Effect;	

	return *this;
}

void OpaqueMesh::DrawMeshWithEffect()
{
	effect->Bind();

	//auto* const Texture = eae6320::Graphics::cTexture::s_manager.Get(s_Texture);
	EAE6320_ASSERT(p_Texture);
	if (p_Texture != nullptr)
		p_Texture->Bind(0);

	mesh->Draw();
}

void OpaqueMesh::CleanUp()
{
	if (mesh != nullptr)
		mesh->CleanUp();

	effect->CleanUp();

	if (p_Texture != nullptr)
		p_Texture->DecrementReferenceCount();
}

void OpaqueMesh::IncrementRefrenceCount()
{
	if (mesh != nullptr)
		mesh->IncrementReferenceCount();

	effect->IncrementReferenceCount();

	if (p_Texture != nullptr)
		p_Texture->IncrementReferenceCount();
}
