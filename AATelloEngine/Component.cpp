#include "Component.h"

Component::Component(COMPONENT_TYPE type) : 
	type(type),
	owner(nullptr)
{
}


Component::~Component()
{
	owner = nullptr;
	type = COMPONENT_TYPE::NONE;
}


bool Component::PreUpdate(float dt)
{
	return true;
}


bool Component::Update(float dt)
{
	return true;
}


void Component::OnUpdateTransform(float4x4 &)
{
}


GameObject* Component::GetOwner() const
{
	return owner;
}


bool Component::SetOwner(GameObject* gameObject)
{
	if (owner == nullptr)
	{
		owner = gameObject;
		return true;
	}
	else
		return false;
}


COMPONENT_TYPE Component::GetType()
{
	return type;
}


void Component::Load(Config& node)
{
}

void Component::Save(Config& node) const
{
}