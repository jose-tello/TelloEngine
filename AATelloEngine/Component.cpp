#include "Component.h"

Component::Component(COMPONENT_TYPE type, GameObject* owner) : 
	type(type),
	owner(owner)
{
}


Component::~Component()
{
	owner = nullptr;
	type = COMPONENT_TYPE::NONE;
}


bool Component::Update(float dt)
{
	return true;
}


bool Component::PostUpdate(float dt)
{
	return true;
}


COMPONENT_TYPE Component::GetType()
{
	return type;
}