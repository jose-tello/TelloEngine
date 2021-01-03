#ifndef __C_MATERIAL_H__
#define __C_MATERIAL_H__

#include "Component.h"
#include "Color.h"

#include <string>

#define CHECKERS_WIDTH 80
#define CHECKERS_HEIGHT 80

struct UniformHandle;
class R_Shader;

class C_Material : public Component
{
public:
	C_Material();
	~C_Material();

	void SetMaterial(int newMat);
	void SetTexture(int newTex);
	void SetShader(int newShader);

	void GetColor(float& r, float& g, float& b, float& a) const;
	std::string GetTexturePath() const;
	void GetTextureSize(int& width, int& height) const;
	int GetShader() const;
	std::string GetShaderName() const;
	std::vector<UniformHandle> GetUniformVector() const;

	bool GetTextureEnabled() const;
	void SetTextureEnable(bool enable);
	
	bool GetColorEnabled() const;
	void SetColorEnable(bool enable);

	bool GetCheckerTextureEnabled() const;
	void SetCheckerTextureEnable(bool enable);

	void GetDrawVariables(Color& col, unsigned int& texId, unsigned int& shaderProgramId) const;

	void Load(Config&) override;
	void Save(Config&) const override;

private:
	void InitCheckerTex();	

	void GetDrawColor(Color&) const;
	unsigned int GetTextureId() const;
	unsigned int GetShaderProgram() const;

	void UpdateUniformArray(R_Shader* shader);

private:
	int materialId = 0;
	int textureId = 0;
	int shaderId = 0;

	bool textureEnabled;
	bool colorEnabled;

	std::vector<UniformHandle> shaderUniformsVector;

	//This will be a material on its own, but for now this is "accetable"
	unsigned int checkerTexId;
	bool useCheckerTex;
};

#endif // !__C_MATERIAL_H__
