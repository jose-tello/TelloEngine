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

	void GetAmbientColor(float*) const;
	void SetAmbientColor(float*);

	void GetDiffuse(float*) const;
	void SetDiffuse(float*);

	void GetSpecular(float*) const;
	void SetSpecular(float*);


private:
	float ambientColor[3];
	float diffuse[3];
	float specular[3];

	float constant;
	float linear;
	float quadratic;
};

#endif // !__C_LIGHT_SOURCE_H__
