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
	/*float3 pos = worldTransform.TranslatePart();
	x = pos.x;
	y = pos.y;
	z = pos.z;*/

	x = localPosition.x;
	y = localPosition.y;
	z = localPosition.z;
}


void C_Transform::SetPos(float x, float y, float z)
{
	localPosition = { x, y, z };
	UpdateLocalTransform();
}


void C_Transform::GetRotation(float& angle, float& x, float& y, float& z) const
{
	x = localRotation.x;
	y = localRotation.y;
	z = localRotation.z;
	angle = localRotation.Angle() * RADTODEG;
}


//TODO: Cange order
void C_Transform::SetRotation(float angle, float x, float y, float z)
{
	localRotation = { x, y, z, angle };
	UpdateLocalTransform();
}


void C_Transform::GetEulerAngles(float& x, float& y, float& z) const
{
	float3 rot = localRotation.ToEulerXYZ();

	x = rot.x * RADTODEG;
	y = rot.y * RADTODEG;
	z = rot.z * RADTODEG;
}


void C_Transform::SetEulerAngles(float x, float y, float z)
{
	localRotation = Quat::FromEulerXYZ(x * DEGTORAD, y * DEGTORAD, z * DEGTORAD);
	UpdateLocalTransform();
}


void C_Transform::GetEscale(float& x, float& y, float& z) const
{
	x = localScale.x;
	y = localScale.y;
	z = localScale.z;
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
	localTransform = transform;
	UpdateTRS();

	needUpdate = true;
}


void C_Transform::SetGlobalTransform(float4x4 transform)
{
	worldTransform = transform;
	needUpdate = true;
}


void C_Transform::NotifyNeedUpdate()
{
	needUpdate = true;
}


void C_Transform::UpdateLocalTransform()
{
	localRotation.Normalize();
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
		worldTransform = owner->parent->transform.worldTransform * localTransform;

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