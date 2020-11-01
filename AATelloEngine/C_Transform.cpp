#include "C_Transform.h"

#include "GameObject.h"

#include "Globals.h"

C_Transform::C_Transform() : Component(COMPONENT_TYPE::TRANSFORM),
	localTransform(localTransform.identity),
	worldTransform(worldTransform.identity),

	needUpdate(false)
{
}


C_Transform::~C_Transform()
{
}


bool C_Transform::PostUpdate(float dt)
{
	if (needUpdate == true)
	{
		UpdateTransform();
		
		NotifyChildsNeedUpdate();
	}

	return true;
}


void C_Transform::GetPos(float& x, float& y, float& z) const
{
	float3 translation = localTransform.TranslatePart() + worldTransform.TranslatePart();

	x = translation.x;
	y = translation.y;
	z = translation.z;
}


void C_Transform::SetPos(float x, float y, float z)
{
	localTransform.SetTranslatePart(float3(x, y, z));
	needUpdate = true;
}


void C_Transform::GetRotation(float& angle, float& x, float& y, float& z) const
{
	float4x4 matrix = localTransform * worldTransform;

	Quat rotation;
	float3 pos;
	float3 scl;

	matrix.Decompose(scl, rotation, pos);
	float3 rotationAxis = rotation.Axis();

	x = rotationAxis.x;
	y = rotationAxis.y;
	z = rotationAxis.z;
	angle = rotation.Angle() * RADTODEG;
}


void C_Transform::SetRotation(float angle, float x, float y, float z)
{
	localTransform.RotateAxisAngle(float3(x, y, z), angle);
	needUpdate = true;
}


void C_Transform::GetEscale(float& x, float& y, float& z) const
{
	float4x4 matrix = localTransform * worldTransform;

	float3 scale = matrix.GetScale();

	x = scale.x;
	y = scale.y;
	z = scale.z;
}


void C_Transform::SetEscale(float x, float y, float z)
{
	localTransform = localTransform.ScaleAlongAxis(float3(x, y, z), 1);
	needUpdate = true;
}


float4x4 C_Transform::GetMatTransform() const
{
	float4x4 mat = worldTransform * localTransform;
	mat.Transpose();
	return mat;
}


void C_Transform::AddTransform(float4x4 transform)
{
	localTransform = transform * localTransform;
	needUpdate = true;
}


void C_Transform::UpdateTransform()
{
	if (owner->parent != nullptr)
		worldTransform = owner->parent->transform.localTransform * owner->parent->transform.worldTransform;

	needUpdate = false;
}


void C_Transform::NotifyChildsNeedUpdate()
{
	int childsCount = owner->childs.size();
	
	for (int i = 0; i < childsCount; i++)
		owner->childs[i]->transform.needUpdate = true;
}