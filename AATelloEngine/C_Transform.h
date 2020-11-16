#ifndef __C_TRANSFORM_H__
#define __C_TRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

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

	float4x4 GetMatTransform() const;
	void AddTransform(float4x4 transform);
	void SetGlobalTransform(float4x4 transform);

	void NotifyNeedUpdate();

private:
	void UpdateLocalTransform();
	void UpdateTRS();
	void UpdateWorldTransform();
	void NotifyChildsNeedUpdate();

private:
	float4x4 localTransform;
	float4x4 worldTransform;

	float3 localPosition;
	Quat localRotation;
	float3 localScale;

	bool needUpdate;

};

#endif // !__C_TRANSFORM_H__

