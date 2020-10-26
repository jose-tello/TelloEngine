#include "GameObject.h"
#include "Component.h"
#include "C_Mesh.h"

GameObject::GameObject(GameObject* parent) :
	name(),
	parent(parent),

	transform(this)
{
	components.push_back(&transform);
}


GameObject::GameObject(std::string& name, GameObject* parent) :
	name(name),
	parent(parent),

	transform(this)
{
	components.push_back(&transform);
}


GameObject::~GameObject()
{
	name.clear();
	parent = nullptr;

	int count = components.size();
	for (int i = 1; i < count; i++)
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

	return nullptr;
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


const char* GameObject::GetName() const
{
	return name.c_str();
}


void GameObject::SetName(const char* n)
{
	name = n;
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