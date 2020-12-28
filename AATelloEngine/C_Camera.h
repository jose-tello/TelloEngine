#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

class E_Window;

class C_Camera : public Component
{
public:
	C_Camera(bool isSceneCamera = false);
	~C_Camera() override;

	bool Update(float dt) override;

	void OnUpdateTransform(float4x4& transform) override;

	bool IsInsideFrustum(AABB&);
	void DrawFrustum() const;

	float4x4 GetViewMat() const;
	float4x4 GetProjectionMat() const;
	
	float GetNearPlaneDst() const;
	void SetNearPlaneDst(float);
	
	float GetFarPlaneDst() const;
	void SetFarPlaneDst(float);

	float GetVerticalFov() const;
	void SetVerticalFov(float fov);

	float GetAspectRatio() const;
	void SetAspectRatio(float aspRatio);

	void SetProjectionMat(float fov, float aspect);

	void LookAt(float3& pos);

	E_Window* GetWindow() const;

	void Load(Config&) override;
	void Save(Config&) const override;
	
public:
	Frustum frustum;

private:
	E_Window* window;
};

#endif // !__C_CAMERA_H__
