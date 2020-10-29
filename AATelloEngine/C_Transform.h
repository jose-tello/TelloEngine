#ifndef __C_TRANSFORM_H__
#define __C_TRANSFORM_H__

#include "Component.h"
#include "glmath.h"

class C_Transform : public Component
{
public:
	C_Transform();
	~C_Transform() override;

	bool PostUpdate(float dt) override;

	void GetPos(float& x, float& y, float& z) const;
	void SetPos(float x, float y, float z);

	void GetRotation(float& angle, float& x, float& y, float& z) const;
	void SetRotation(float angle, float x, float y, float z);

	void GetEscale(float& x, float& y, float& z) const;
	void SetEscale(float x, float y, float z);

	mat4x4 GetMatTransform() const;
	void AddTransform(mat4x4 transform);

private:
	void UpdateTransform();
	void NotifyChildsNeedUpdate();

private:
	mat4x4 localTransform;
	mat4x4 worldTransform;

	bool needUpdate;

};

#endif // !__C_TRANSFORM_H__

