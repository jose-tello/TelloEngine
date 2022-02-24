#ifndef __R_SHADER_H__
#define __R_SHADER_H__

#include "Resource.h"

#include <vector>
#include <string>

struct UniformHandle;

enum class SHADER_TYPE : int
{
	ERROR_TYPE = -1,
	VERTEX,
	FRAGMENT,
	COMPUTE
};

class R_Shader : public Resource
{
public:
	R_Shader(int uid, const char* assetPath, RESOURCE_TYPE type);
	~R_Shader() override;

	void Load() override;
	void UnLoad() override;

	unsigned int GetProgramId() const;
	void GetProgramUniforms(std::vector<UniformHandle>&) const;
	
	const char* GetShaderName() const;
	void SetShaderName(const char* name);

	void InitShaderProgram(std::string& vertCode, std::string& fragCode, std::string& computeCode);
	unsigned int CreateShader(std::string& code, SHADER_TYPE type);

	void UseShaderProgram() const;
	void UnuseShaderProgram() const;

	void SetBoolUniform(const char* uniformName, bool boolVar);
	void SetUintUniform(const char* uniformName, unsigned int uintVar);
	void SetIntUniform(const char* uniformName, int intVar);
	void SetFloatUniform(const char* uniformName, float floatVar);

private:
	void CreateShaderProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int computeShader);

	bool CheckShaderCompile(unsigned int shader) const;
	bool CheckProgramCompiles() const;

	int GetUniformType(unsigned int) const;

private:
	unsigned int programId;
	std::string name;

	bool disabled = true; //Allways start disabled, onlly enabled if it compiles correctlly
};


#endif // !__R_SHADER_H__
