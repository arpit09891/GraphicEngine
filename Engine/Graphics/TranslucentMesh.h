#pragma once

#include"Engine\Graphics\Effects.h"
#include "Engine\Graphics\cTexture.h"
#include"Engine\Graphics\Mesh.h"
#include"Engine\Math\sVector.h"
#include"Engine\Math\cMatrix_transformation.h"
#include"Engine\Physics\sRigidBodyState.h"

class TranslucentMesh {

public:

	Effects * effect;
	Mesh * mesh;

	eae6320::Graphics::cTexture* p_Texture;
	eae6320::Physics::sRigidBodyState RigidBody;
	eae6320::Math::cMatrix_transformation transform_localToWorld;
	eae6320::Math::sVector objectWRTOCamera;

	Effects* EffectData(char * vertexTarget, char * fragmentTarget, uint8_t defaultRenderState);
	Mesh* SubmitMeshData(const char* const i_path);

	void UpdatePositionBasedOnTimeAndVelocity(float time);
	TranslucentMesh BindMeshAndEffect(Mesh*, Effects*);

	void DrawMeshWithEffect();
	void CleanUp();
	void IncrementRefrenceCount();
};