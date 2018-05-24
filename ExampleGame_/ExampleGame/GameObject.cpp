#include "GameObject.h"

bool GameObject::IsAlive()
{
	if(currentHealth == 0)
		return false;
	if (currentHealth > 0)
		return true;

	return false;
}

void GameObject::Destroy()
{
}

void GameObject::RePosition(eae6320::Physics::sRigidBodyState i_RigidBodys)
{
 	if (currentHealth == 0)
	{ 
		RigidBody.position = i_RigidBodys.position;
		RigidBody.position.y = -2.6f;

		currentHealth = maxHealth;
	}
}
