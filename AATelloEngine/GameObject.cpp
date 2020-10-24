#include "GameObject.h"

GameObject::GameObject(std::string& name) :
	name(name),
	parent(nullptr),

	transform(this)
{
	components.push_back(&transform);
}


GameObject::~GameObject()
{
	name.clear();
	parent = nullptr;

	int count = components.size();
	for (int i = 0; i < count; i++)
	{
		delete components[i];
		components[i] = nullptr;
	}
	components.clear();

	count = childs.size();
	for (int i = 0; i < count; i++)
	{
		delete childs[i];
		childs[i] = nullptr;
	}
	childs.clear();
}


Component* GameObject::GetComponent(COMPONENT_TYPE componentType)
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		if (components[i]->GetType() == componentType)
			return components[i];
	}
}


bool GameObject::AddComponent(Component* component)
{
	if (CheckNotRepeated(component->GetType()) == false)
		return false;
	
	components.push_back(component);
	
	return true;
}


bool GameObject::DeleteComponent(COMPONENT_TYPE componentType)
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		if (components[i]->GetType() == componentType)
		{
			delete components[i];
			components.erase(components.begin() + i);

			return true;
		}
	}
}


void GameObject::PreUpdate(float dt)
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		components[i]->PreUpdate(dt);
	}
}


void GameObject::Update(float dt)
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		components[i]->Update(dt);
	}
}


void GameObject::PostUpdate(float dt)
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		components[i]->PostUpdate(dt);
	}
}


bool GameObject::CheckNotRepeated(COMPONENT_TYPE componentType)
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		if (components[i]->GetType() == componentType)
			return true;
	}

	return true;
}