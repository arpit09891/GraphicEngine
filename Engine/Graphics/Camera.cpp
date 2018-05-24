#include"Camera.h"
#include"Engine\Math\Functions.h"
#include"Engine\UserSettings\UserSettings.h"

void Camera::UpdateTransformWorldToCamera()
{
	transform_WorldToCamera = eae6320::Math::cMatrix_transformation::CreateWorldToCameraTransform(
		RigidBody.orientation, RigidBody.position);
}

void Camera::UpdateTransformCameraToProjected()
{
	transform_CameraToProjected = eae6320::Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(
		i_verticalFieldOfView_inRadians,i_aspectRatio,i_z_nearPlane,i_z_farPlane
	);
}

Camera::Camera()
{
	float i_verticalFieldOfView_degree = 45.f;
	i_verticalFieldOfView_inRadians = eae6320::Math::ConvertDegreesToRadians(i_verticalFieldOfView_degree);
	uint16_t resolutionWidth;
	uint16_t resoutionHeight;

	eae6320::UserSettings::GetDesiredInitialResolutionWidth(resolutionWidth);
	eae6320::UserSettings::GetDesiredInitialResolutionHeight(resoutionHeight);

	i_aspectRatio = ((float)resolutionWidth)/ ((float)resoutionHeight); //TODO change Aspect Ratio
	i_z_nearPlane = 0.1f;
	i_z_farPlane = 100.f;

	transform_WorldToCamera = eae6320::Math::cMatrix_transformation();
	transform_CameraToProjected = eae6320::Math::cMatrix_transformation();
}