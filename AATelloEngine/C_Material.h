#ifndef __C_MATERIAL_H__
#define __C_MATERIAL_H__

#include "Component.h"
#include "Color.h"

class C_Material : public Component
{
public:
	C_Material();
	~C_Material();

	void SetTexture(unsigned int newTex);
	void SetColor(Color& col);

	bool GetTextureEnabled() const;
	void SetTextureEnable(bool enable);
	
	bool GetColorEnabled() const;
	void SetColorEnable(bool enable);

	void GetDrawVariables(unsigned int& texId, Color& col) const;

private:
	unsigned int textureId;
	Color color;

	bool textureEnabled;
	bool colorEnabled;
};

#endif // !__C_MATERIAL_H__
