#ifndef __C_MATERIAL_H__
#define __C_MATERIAL_H__

#include "Component.h"
#include "Color.h"

#include <string>

#define CHECKERS_WIDTH 80
#define CHECKERS_HEIGHT 80

class C_Material : public Component
{
public:
	C_Material();
	~C_Material();

	void SetTexture(unsigned int newTex);
	void GetColor(float& r, float& g, float& b, float& a) const;
	void SetColor(Color& col);

	bool GetTextureEnabled() const;
	void SetTextureEnable(bool enable);
	
	bool GetColorEnabled() const;
	void SetColorEnable(bool enable);

	bool GetCheckerTextureEnabled() const;
	void SetCheckerTextureEnable(bool enable);

	void GetDrawVariables(unsigned int& texId, Color& col) const;

	void Load(Config&) override;
	void Save(Config&) const override;

private:
	void InitTextureSize();
	void InitCheckerTex();

public:
	//TODO: this information will be held by the material/texture resource, but for now it will stay here
	std::string texturePath;
	std::string textureName;
	std::string materialPath;
	int textureWidth;
	int textureHeight;

private:
	unsigned int textureId;
	Color color;

	bool textureEnabled;
	bool colorEnabled;

	//This will be a material on its own, but for now this is "accetable"
	unsigned int checkerTexId;
	bool useCheckerTex;
};

#endif // !__C_MATERIAL_H__
