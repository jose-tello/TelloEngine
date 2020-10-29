#include "GameObject.h"
#include "Component.h"
#include "C_Mesh.h"

GameObject::GameObject(GameObject* parent) :
	parent(parent),
	toDelete(false),
	transform(this),
	name()
{
	components.push_back(&transform);
}


GameObject::GameObject(std::string& name, GameObject* parent) :
	parent(parent),
	toDelete(false),
	transform(this),
	name(name)
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

	name.clear();
}


Component* GameObject::GetComponent(COMPONENT_TYPE componentType) const
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		if (components[i]->GetType() == componentType)
			return components[i];
	}

	return nullptr;
}


void GameObject::GetAllComponents(std::vector<Component*>& vec) const
{
	vec = components;
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


void GameObject::SearchDeletedChilds()
{
	int childsCount = childs.size();
	for (int i = 0; i < childsCount; i++)
	{
		if (childs[i]->toDelete == true)
		{
			childs.erase(childs.begin() + i);
			return;								//If multiple childs are deleted simultaneously, this ensures all will be deleted properly
		}
	}
}


bool GameObject::CheckNotRepeated(COMPONENT_TYPE componentType)
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		if (components[i]->GetType() == componentType)
			return false;
	}

	return true;
}