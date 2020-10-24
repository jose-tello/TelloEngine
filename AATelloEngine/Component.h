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
	Component(COMPONENT_TYPE type, GameObject* owner);
	virtual ~Component();

	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);

	COMPONENT_TYPE GetType();

protected:
	GameObject* owner;

private:
	COMPONENT_TYPE type;

};

#endif // !__COMPONENT_H__

