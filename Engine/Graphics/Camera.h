#pragma once
#include"Engine\Physics\sRigidBodyState.h"
#include"Engine\Math\cMatrix_transformation.h"

class Camera {
	
	float i_verticalFieldOfView_inRadians;
	float i_aspectRatio;
	float i_z_nearPlane;
	float i_z_farPlane;

public:
	eae6320::Physics::sRigidBodyState RigidBody;
	eae6320::Math::cMatrix_transformation transform_WorldToCamera;
	eae6320::Math::cMatrix_transformation transform_CameraToProjected;
	void UpdateTransformWorldToCamera();
	void UpdateTransformCameraToProjected();
	Camera();

};