#ifndef __COMPONENT_H__
#define __COMPONENT_H__

struct Config;
struct GameObject;

#include "MathGeoLib/src/MathGeoLibFwd.h"

enum class COMPONENT_TYPE : int
{
	NONE = -1,
	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA,
	POINT_LIGHT,
	PROCEDURAL_MESH,
	ABERRATION,
	MAX

};


class Component
{
public:
	Component(COMPONENT_TYPE type);
	virtual ~Component();

	virtual bool Update(float dt);
	virtual void OnUpdateTransform(float4x4 &);

	GameObject* GetOwner() const;
	bool SetOwner(GameObject*);
	COMPONENT_TYPE GetType();

	virtual void Load(Config&);
	virtual void Save(Config&) const;

protected:
	GameObject* owner = nullptr;

private:
	COMPONENT_TYPE type;

};

#endif // !__COMPONENT_H__

