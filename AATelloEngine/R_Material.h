#ifndef __R_MATERIAL_H__
#define __R_MATERIAL_H__

#include "Resource.h"

#include "Color.h"

class R_Texture;

class R_Material : public Resource
{
public:
	R_Material(int uid, const char* assetPath, RESOURCE_TYPE type);
	~R_Material() override;

	void Load() override;
	void UnLoad() override;

	void GetColor(float& r, float& g, float& b, float& a) const;
	void SetColor(Color& col);
	
	int GetResourceTexture() const;
	void SetResourceTexture(int resource);

private:
	Color color;

	int resourceTexture;
};

#endif // !__R_MATERIAL_H__
