#ifndef __R_SHADER_H__
#define __R_SHADER_H__

#include "Resource.h"

class R_Shader : public Resource
{
public:
	R_Shader(int uid, const char* assetPath, RESOURCE_TYPE type);
	~R_Shader() override;

	void Load() override;
	void UnLoad() override;

	unsigned int GetProgramId() const;
	const char* GetProgramCode() const;
	
	const char* GetShaderName() const;
	void SetShaderName(const char* name);

	void InitShader(const char* vertCode, const char* fragCode);
	void InitShaderFromBinary(const char* programCode, unsigned int size);

	/*void SetUniform(char* uniformName, bool boolVar);
	void SetUniform(char* uniformName, unsigned int uintVar);
	void SetUniform(char* uniformName, int intVar);
	void SetUniform(char* uniformName, float floatVar);*/

private:
	void CreateShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);

	bool CheckShadersCompile(unsigned int vertShader, unsigned int fragShader) const;
	bool CheckProgramCompiles() const;

private:
	unsigned int programId;
	std::string name;

	bool disabled = true; //Allways start disabled, onlly enabled if it compiles correctlly
};


#endif // !__R_SHADER_H__
