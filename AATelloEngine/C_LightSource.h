#ifndef __C_LIGHT_SOURCE_H__
#define __C_LIGHT_SOURCE_H__

#include "Component.h"

class C_LightSource : public Component
{
public:
	C_LightSource();
	~C_LightSource() override;

	bool Update(float dt) override;

	void Load(Config&) override;
	void Save(Config&) const override;

	void GetLightColor(float*) const;
	void SetLightColor(float*);

	void GetAmbientColor(float*) const;
	void SetAmbientColor(float*);

	void GetDiffuse(float*) const;
	void SetDiffuse(float*);

	void GetSpecular(float*) const;
	void SetSpecular(float*);

	float GetLightPower() const;
	void SetLightPower(float);

private:
	float lightColor[3];
	float ambientColor[3];
	float diffuse[3];
	float specular[3];

	float lightPower;
};

#endif // !__C_LIGHT_SOURCE_H__
