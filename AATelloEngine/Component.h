#ifndef __COMPONENT_H__
#define __COMPONENT_H__

enum class COMPONENT_TYPE : int
{
	NONE = -1,
	TRANSFORM,
	MESH,
	MATERIAL,
	MAX

};

struct GameObject;

class Component
{
public:
	Component(COMPONENT_TYPE type);
	virtual ~Component();

	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);

	bool SetOwner(GameObject*);
	COMPONENT_TYPE GetType();

protected:
	GameObject* owner = nullptr;

private:
	COMPONENT_TYPE type;

};

#endif // !__COMPONENT_H__

