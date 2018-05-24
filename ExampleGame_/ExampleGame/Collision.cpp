#include "Collision.h"

bool Collision::checkCollision(GameObject & collidend, GameObject & collider)
{

	if (collidend.RigidBody.position.y - (collidend.height / 2) < collider.RigidBody.position.y + (collider.height / 2)
		&&
		collidend.RigidBody.position.y + (collidend.height / 2) > collider.RigidBody.position.y + (collider.height / 2)
		&&
		collidend.RigidBody.position.x - (collidend.width / 2) < collider.RigidBody.position.x - (collider.width / 2)
		&&
		collidend.RigidBody.position.x + (collidend.width / 2) > collider.RigidBody.position.x + (collider.width / 2)
		)
	{
		collidend.currentHealth--;
		collider.currentHealth--;
		return true;
	}
	else
	{
		return false;
	}
}
