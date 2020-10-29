#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "C_Transform.h"

#include <vector>
#include <string>

struct GameObject
{
public:
	GameObject(GameObject* parent);
	GameObject(std::string& name, GameObject* parent);
	~GameObject();

	Component* GetComponent(COMPONENT_TYPE componentType) const;
	void GetAllComponents(std::vector<Component*>& vec) const;

	bool AddComponent(Component* component);
	bool DeleteComponent(COMPONENT_TYPE componentType);

	void Update(float dt);
	void PostUpdate(float dt);

	const char* GetName() const;
	void SetName(const char*);

private:
	bool CheckNotRepeated(COMPONENT_TYPE componentType);

public:
	GameObject* parent = nullptr;
	bool toDelete;

	std::vector<GameObject*> childs;

	C_Transform transform;

private:
	std::string name;

	std::vector<Component*> components;
};

#endif // __GAME_OBJECT_H__
