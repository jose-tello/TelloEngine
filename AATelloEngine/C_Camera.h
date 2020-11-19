#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

class C_Camera : public Component
{
public:
	C_Camera();
	~C_Camera() override;

	float* GetViewMat() const;
	float* GetProjectionMat() const;
	
	float GetNearPlaneDst() const;
	void SetNearPlaneDst(float);
	
	float GetFarPlaneDst() const;
	void SetFarPlaneDst(float);

	float GetVerticalFov() const;
	float GetAspectRatio() const;
	void SetProjectionMat(float fov, float aspect);

	void LookAt(float3& pos);
	void UpdateTransform();

private:
	Frustum frustum;
};

#endif // !__C_CAMERA_H__
