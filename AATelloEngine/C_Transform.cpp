#include "C_Transform.h"

#include "GameObject.h"

#include "Globals.h"

C_Transform::C_Transform() : Component(COMPONENT_TYPE::TRANSFORM),
	localTransform(localTransform.identity),
	worldTransform(worldTransform.identity),

	position(float3::zero),
	rotation(Quat::identity),
	scale(float3::one),

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
	x = position.x;
	y = position.y;
	z = position.z;
}


void C_Transform::SetPos(float x, float y, float z)
{
	position = { x, y, z };
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
	rotation = { x, y, z, angle };
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
	scale = { x, y, z };
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
	localTransform = float4x4::FromTRS(position, rotation, scale);
	needUpdate = true;
}


void C_Transform::UpdateTRS()
{
	localTransform.Decompose(position, rotation, scale);
}


void C_Transform::UpdateWorldTransform()
{
	if (owner->parent != nullptr)
		worldTransform = owner->parent->transform.worldTransform * localTransform;

	else
		worldTransform = float4x4::identity * localTransform;

	needUpdate = false;
}


void C_Transform::NotifyChildsNeedUpdate()
{
	int childsCount = owner->childs.size();
	
	for (int i = 0; i < childsCount; i++)
		owner->childs[i]->transform.needUpdate = true;
}