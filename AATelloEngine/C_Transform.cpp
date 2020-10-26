#include "C_Transform.h"

#include "GameObject.h"

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


void C_Transform::SetPos(float x, float y, float z)
{
	localTransform.translate(x, y, z);
	NotifyChildsNeedUpdate();
}


void C_Transform::SetEscale(float x, float y, float z)
{
	localTransform.scale(x, y, z);
	NotifyChildsNeedUpdate();
}


mat4x4 C_Transform::GetMatTransform() const
{
	mat4x4 transform = localTransform * worldTransform;

	return transform;
}


void C_Transform::UpdateTransform()
{
	//So... here goes some mathematic code, ill ask adri later
	needUpdate = false;
}


void C_Transform::NotifyChildsNeedUpdate()
{
	int childsCount = owner->childs.size();
	
	for (int i = 0; i < childsCount; i++)
		owner->childs[i]->transform.needUpdate = true;
}