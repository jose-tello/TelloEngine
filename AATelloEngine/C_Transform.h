#ifndef __C_TRANSFORM_H__
#define __C_TRANSFORM_H__

#include "Component.h"
#include "glmath.h"

class C_Transform : public Component
{
public:
	C_Transform(GameObject* owner);
	~C_Transform();

	bool PostUpdate(float dt) override;

	void SetPos(float x, float y, float z);
	void SetEscale(float x, float y, float z);

	mat4x4 GetMatTransform() const;

private:
	void UpdateTransform();
	void NotifyChildsNeedUpdate();

private:
	mat4x4 localTransform;
	mat4x4 worldTransform;

	bool needUpdate;

};

#endif // !__C_TRANSFORM_H__

