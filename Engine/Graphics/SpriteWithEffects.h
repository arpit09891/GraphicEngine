#pragma once

#include"Engine\Graphics\Sprite.h"
#include"Engine\Graphics\Effects.h"
#include "Engine\Graphics\cTexture.h"
#include"Engine\Graphics\Mesh.h"
#include"Engine\Math\sVector.h"
#include"Engine\Math\cMatrix_transformation.h"
#include"Engine\Physics\sRigidBodyState.h"


class SpriteWithEffects {

public:
	Sprite * sprite;
	Effects * effect;
	Mesh * mesh;
	eae6320::Graphics::cTexture* p_Texture;
	eae6320::Physics::sRigidBodyState RigidBody;
	eae6320::Math::cMatrix_transformation transform_localToWorld;
	eae6320::Math::sVector objectWRTOCamera;
	Sprite* SubmitSpriteData(float width, float height, eae6320::Graphics::VertexFormats::sSprite center);
	Effects* EffectData(char * vertexTarget, char * fragmentTarget, uint8_t defaultRenderState);
	Mesh* SubmitMeshData(const char* const i_path);

	void UpdatePositionBasedOnTimeAndVelocity(float time);
	SpriteWithEffects BindSpriteAndEffect(Sprite*, Effects*);
	SpriteWithEffects BindMeshAndEffect(Mesh*, Effects*);
	
	void DrawSpriteWithEffect();
	void DrawMeshWithEffect();
	
	void CleanUp();
	void IncrementRefrenceCount();
};