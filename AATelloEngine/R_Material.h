#ifndef __R_MATERIAL_H__
#define __R_MATERIAL_H__

#include "Resource.h"

#include "Color.h"

class R_Material : public Resource
{
public:
	R_Material(int uid, const char* assetPath, RESOURCE_TYPE type);
	~R_Material() override;

	void Load() override;
	void UnLoad() override;

	void SetTexture(unsigned int newTex);
	void SetColor(Color& col);

	void GetDrawVariables(unsigned int& texId, Color& col) const;

private:
	void InitTextureSize();

public:
	int textureWidth;
	int textureHeight;

private:
	unsigned int textureId;
	Color color;
};

#endif // !__R_MATERIAL_H__
