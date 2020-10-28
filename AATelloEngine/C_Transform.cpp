#include "C_Transform.h"

#include "GameObject.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include "Globals.h"

C_Transform::C_Transform(GameObject* owner) :Component(COMPONENT_TYPE::TRANSFORM, owner),
	localTransform(),
	worldTransform(),

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
	vec3 translation = localTransform.translation() + worldTransform.translation();

	x = translation.x;
	y = translation.y;
	z = translation.z;
}


void C_Transform::SetPos(float x, float y, float z)
{
	localTransform.translate(x, y, z);
	needUpdate = true;
}


void C_Transform::GetRotation(float& angle, float& x, float& y, float& z) const
{
	mat4x4 mat = localTransform * worldTransform;

	float4x4 matrix(mat[0],	 mat[1],  mat[2],  mat[3],
					mat[4],  mat[5],  mat[6],  mat[7],
					mat[8],  mat[9],  mat[10], mat[11],
					mat[12], mat[13], mat[14], mat[15]);

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
	localTransform.rotate(angle, vec3(x, y, z));
	needUpdate = true;
}


void C_Transform::GetEscale(float& x, float& y, float& z) const
{
	mat4x4 mat = localTransform * worldTransform;

	float4x4 matrix(mat[0],  mat[1],  mat[2],  mat[3],
					mat[4],  mat[5],  mat[6],  mat[7],
					mat[8],  mat[9],  mat[10], mat[11],
					mat[12], mat[13], mat[14], mat[15]);

	float3 scale = matrix.GetScale();

	x = scale.x;
	y = scale.y;
	z = scale.z;
}


void C_Transform::SetEscale(float x, float y, float z)
{
	localTransform.scale(x, y, z);
	needUpdate = true;
}


mat4x4 C_Transform::GetMatTransform() const
{
	return worldTransform * localTransform;
}


void C_Transform::AddTransform(mat4x4 transform)
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