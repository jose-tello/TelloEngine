#ifndef __C_MATERIAL_H__
#define __C_MATERIAL_H__

#include "Component.h"
#include "Color.h"

#include <string>

class R_Texture;
class R_Material;

#define CHECKERS_WIDTH 80
#define CHECKERS_HEIGHT 80

class C_Material : public Component
{
public:
	C_Material();
	~C_Material();

	void SetTexture(int newTex);
	void SetMaterial(int newMat);

	void GetColor(float& r, float& g, float& b, float& a) const;
	std::string GetTexturePath() const;
	void GetTextureSize(int& width, int& height) const;

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
	void InitCheckerTex();	

private:
	R_Material* material = nullptr;
	R_Texture* texture = nullptr;

	bool textureEnabled;
	bool colorEnabled;

	//This will be a material on its own, but for now this is "accetable"
	unsigned int checkerTexId;
	bool useCheckerTex;
};

#endif // !__C_MATERIAL_H__
