#ifndef __C_POINT_LIGHT_H__
#define __C_POINT_LIGHT_H__

#include "Component.h"

class C_Material;

class C_PointLight : public Component
{
public:
	C_PointLight();
	~C_PointLight() override;

	bool Update(float dt) override;

	void PushLightUniforms(C_Material* material, int lightNumber);
	
	void Load(Config&) override;
	void Save(Config&) const override;

	void GetLightColor(float*) const;
	void SetLightColor(float*);

	void GetAmbientColor(float*) const;
	void SetAmbientColor(float*);

	void GetDiffuse(float*) const;
	void SetDiffuse(float*);

	float GetSpecular() const;
	void SetSpecular(float);

	float GetLightIntensity() const;
	void SetLightIntensity(float);

private:
	float lightColor[3];
	float ambientColor[3];
	float diffuse[3];
	float specular;

	float lightIntensity;
};

#endif // !__C_POINT_LIGHT_H__
