#include "C_Camera.h"

#include "Application.h"
#include "M_Editor.h"


#include "Config.h"

C_Camera::C_Camera() : Component(COMPONENT_TYPE::CAMERA),
	window(nullptr)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	
	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
	
	frustum.SetViewPlaneDistances(0.1, 1000);
	frustum.SetPerspective(1, 1);

	window = App->editor->AddSceneWindow(this);
}


C_Camera::~C_Camera()
{
	App->editor->DeleteWindow(window);
}


void C_Camera::OnUpdateTransform(float4x4& transform)
{
	frustum.SetUp(transform.WorldY());
	frustum.SetFront(transform.WorldZ());

	frustum.SetPos(transform.TranslatePart());
}


float* C_Camera::GetViewMat() const
{
	float4x4 mat = frustum.ViewMatrix();
	return mat.Transposed().ptr();
}


float* C_Camera::GetProjectionMat() const
{
	return frustum.ProjectionMatrix().Transposed().ptr();
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


void C_Camera::SetVerticalFov(float fov)
{
	frustum.SetVerticalFovAndAspectRatio(fov, frustum.AspectRatio());
}


float C_Camera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}


void C_Camera::SetAspectRatio(float aspRatio)
{
	frustum.SetVerticalFovAndAspectRatio(frustum.VerticalFov(), aspRatio);
}


void C_Camera::SetProjectionMat(float fov, float aspect)
{
	frustum.SetVerticalFovAndAspectRatio(fov, aspect);
}


void C_Camera::LookAt(float3& pos)
{
	float3 vector = pos - frustum.Pos();
	float3x3 look = float3x3::LookAt(frustum.Front(), vector.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(look.MulDir(frustum.Front().Normalized()));
	frustum.SetUp(look.MulDir(frustum.Up().Normalized()));
}


void C_Camera::Load(Config& node)
{
	frustum.SetVerticalFovAndAspectRatio(node.GetNum("vertical fov"), node.GetNum("aspect ratio"));

	frustum.SetViewPlaneDistances(node.GetNum("near plane distance"), node.GetNum("far plane distance"));
}


void C_Camera::Save(Config& node) const
{
	node.AppendNum("type", (int)COMPONENT_TYPE::CAMERA);

	node.AppendNum("vertical fov", frustum.VerticalFov());
	node.AppendNum("aspect ratio", frustum.AspectRatio());
	node.AppendNum("far plane distance", frustum.FarPlaneDistance());
	node.AppendNum("near plane distance", frustum.NearPlaneDistance());
}