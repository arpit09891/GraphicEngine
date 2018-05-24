#include "SpriteWithEffects.h"
#include"sContext.h"

Sprite * SpriteWithEffects::SubmitSpriteData(float width, float height, eae6320::Graphics::VertexFormats::sSprite center)
{
	auto result = eae6320::Results::Success;
	Sprite * tempSprite;
	bool check = Sprite::Factory(tempSprite);
	
	if (!check)
		return nullptr;
	
	result = tempSprite->InitializeGeometry(width, height, center);
	if (!result)
		return nullptr;
	return tempSprite;
}

Mesh * SpriteWithEffects::SubmitMeshData( const char* const i_path)
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

Effects * SpriteWithEffects::EffectData(char * vertexTarget, char * fragmentTarget, uint8_t defaultRenderState)
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

void SpriteWithEffects::UpdatePositionBasedOnTimeAndVelocity(float time)
{
	RigidBody.position = RigidBody.position + (RigidBody.velocity * time);
}

SpriteWithEffects SpriteWithEffects::BindSpriteAndEffect(Sprite* i_Sprite, Effects* i_Effect)
{
	sprite = i_Sprite;
	effect = i_Effect;
	mesh = nullptr;

	return *this;
}

SpriteWithEffects SpriteWithEffects::BindMeshAndEffect(Mesh* i_Mesh, Effects* i_Effect)
{
	mesh = i_Mesh;
	effect = i_Effect;
	sprite = nullptr;

	return *this;
}

void SpriteWithEffects::CleanUp()
{
	if(sprite != nullptr)
		sprite->CleanUp();
	
	if (mesh != nullptr)
		mesh->CleanUp();
	
		effect->CleanUp();

	if(p_Texture != nullptr)
		p_Texture->DecrementReferenceCount();

}

void SpriteWithEffects::IncrementRefrenceCount()
{
	if (sprite != nullptr)
		sprite->IncrementReferenceCount();
	
	if (mesh != nullptr)
		mesh->IncrementReferenceCount();
	
	effect->IncrementReferenceCount();
	
	if(p_Texture != nullptr)
		p_Texture->IncrementReferenceCount();
}

void SpriteWithEffects::DrawSpriteWithEffect()
{

	effect->Bind();
	
	//auto* const Texture = eae6320::Graphics::cTexture::s_manager.Get(s_Texture);
	if (p_Texture != nullptr)
	{
		EAE6320_ASSERT(p_Texture);
		p_Texture->Bind(0);
	}

	if(sprite != nullptr)
		sprite->Draw();
	
	if (mesh != nullptr)
		mesh->Draw();	
	
}

void SpriteWithEffects::DrawMeshWithEffect()
{
	effect->Bind();

	//auto* const Texture = eae6320::Graphics::cTexture::s_manager.Get(s_Texture);
	EAE6320_ASSERT(p_Texture);
	if (p_Texture != nullptr)
		p_Texture->Bind(0);

	mesh->Draw();

}
