#include "C_Camera.h"
#include "GameObject.h"

C_Camera::C_Camera() : Component(COMPONENT_TYPE::CAMERA)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	
	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
	
	frustum.SetViewPlaneDistances(0.1, 1000);
	frustum.SetPerspective(1, 1);
}


C_Camera::~C_Camera()
{

}


float* C_Camera::GetViewMat() const
{
	return frustum.ViewMatrix().ptr();
}


float* C_Camera::GetProjectionMat() const
{
	return frustum.ProjectionMatrix().ptr();
}


float C_Camera::GetNearPlaneDst() const
{
	return frustum.NearPlaneDistance();
}


void C_Camera::SetNearPlaneDst(float dist)
{
	frustum.SetViewPlaneDistances(dist, frustum.FarPlaneDistance());
}


float C_Camera::GetFarPlaneDst() const
{
	return frustum.FarPlaneDistance();
}


void C_Camera::SetFarPlaneDst(float dist)
{
	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), dist);
}


float C_Camera::GetVerticalFov() const
{
	return frustum.VerticalFov();
}


float C_Camera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}


void C_Camera::SetProjectionMat(float fov, float aspect)
{
	frustum.SetVerticalFovAndAspectRatio(fov, aspect);
}


void C_Camera::LookAt(float3& pos)
{
	float3x3 look = float3x3::LookAt(frustum.Front(), pos, frustum.Up(), float3::unitY);

	frustum.SetFront(look.MulDir(frustum.Front().Normalized()));
	frustum.SetUp(look.MulDir(frustum.Up().Normalized()));
}


void C_Camera::UpdateTransform()
{
	float4x4 global = owner->transform.GetMatTransform();
	frustum.SetUp(global.WorldZ());
	frustum.SetFront(global.WorldY());

	frustum.SetPos(global.TranslatePart());
}