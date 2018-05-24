#pragma once
#include "Engine\Graphics\Mesh.h"
#include"Engine\Graphics\OpaqueMesh.h"
#include"Engine\Graphics\TranslucentMesh.h"
#include"Engine\Physics\sRigidBodyState.h"

class GameObject {

public:
	const OpaqueMesh* opaqueMesh;
	const TranslucentMesh* translucentMesh;
	
	int currentHealth;
	int maxHealth;
	eae6320::Physics::sRigidBodyState RigidBody;
	float height;
	float width;
	
	bool IsAlive();
	void Destroy();
	void RePosition(eae6320::Physics::sRigidBodyState i_RigidBody);
};