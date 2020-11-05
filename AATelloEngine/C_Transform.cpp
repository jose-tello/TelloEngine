#include "C_Transform.h"

#include "GameObject.h"

#include "Globals.h"

C_Transform::C_Transform() : Component(COMPONENT_TYPE::TRANSFORM),
	localTransform(localTransform.identity),
	worldTransform(worldTransform.identity),

	localPosition(float3::zero),
	localRotation(Quat::identity),
	localScale(float3::one),

	needUpdate(true)
{
}


C_Transform::~C_Transform()
{
}


bool C_Transform::PostUpdate(float dt)
{
	if (needUpdate == true)
	{
		UpdateWorldTransform();
		
		NotifyChildsNeedUpdate();
	}

	return true;
}


void C_Transform::GetPos(float& x, float& y, float& z) const
{
	float3 pos = worldTransform.TranslatePart();
	x = pos.x;
	y = pos.y;
	z = pos.z;
}


void C_Transform::SetPos(float x, float y, float z)
{
	localPosition = { x, y, z };
	UpdateLocalTransform();
}


void C_Transform::GetRotation(float& angle, float& x, float& y, float& z) const
{
	Quat rotation;
	float3 pos;
	float3 scl;

	worldTransform.Decompose(scl, rotation, pos);
	float3 rotationAxis = rotation.Axis();

	x = rotationAxis.x;
	y = rotationAxis.y;
	z = rotationAxis.z;
	angle = rotation.Angle() * RADTODEG;
}


void C_Transform::SetRotation(float angle, float x, float y, float z)
{
	localRotation = { x, y, z, angle };
	UpdateLocalTransform();
}


void C_Transform::GetEscale(float& x, float& y, float& z) const
{
	float3 scale = worldTransform.GetScale();

	x = scale.x;
	y = scale.y;
	z = scale.z;
}


void C_Transform::SetEscale(float x, float y, float z)
{
	localScale = { x, y, z };
	UpdateLocalTransform();
}


float4x4 C_Transform::GetMatTransform() const
{
	return worldTransform.Transposed();
}


void C_Transform::AddTransform(float4x4 transform)
{
	localTransform = transform * localTransform;
	UpdateTRS();

	needUpdate = true;
}


void C_Transform::UpdateLocalTransform()
{
	localTransform = float4x4::FromTRS(localPosition, localRotation, localScale);
	needUpdate = true;
}


void C_Transform::UpdateTRS()
{
	localTransform.Decompose(localPosition, localRotation, localScale);
}


void C_Transform::UpdateWorldTransform()
{
	if (owner->parent != nullptr)
	{
		float3 pos;
		Quat rot;
		float3 scl;

		owner->parent->transform.worldTransform.Decompose(pos, rot, scl);

		pos = pos + localPosition;
		rot = rot * localRotation;
		scl.x = scl.x * localScale.x;
		scl.y = scl.y * localScale.y;
		scl.z = scl.z * localScale.z;

		worldTransform = float4x4::FromTRS(pos, rot, scl);
	}
		

	else
		worldTransform = localTransform;

	needUpdate = false;
}


void C_Transform::NotifyChildsNeedUpdate()
{
	int childsCount = owner->childs.size();
	
	for (int i = 0; i < childsCount; i++)
		owner->childs[i]->transform.needUpdate = true;
}