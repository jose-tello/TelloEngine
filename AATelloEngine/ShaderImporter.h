#ifndef __SHADER_IMPORTER_H__
#define __SHADER_IMPORTER_H__

class R_Shader;

#include <string>

#define DEFAULT_SHADER_PATH "/Assets/defaultAssets/defaultShader.txt"
#define VERTEX_SHADER_KEY "__VERTEX_SHADER__"
#define FRAGMENT_SHADER_KEY "__FRAGMENT_SHADER__"
#define COMPUTE_SHADER_KEY "__COMPUTE_SHADER__"

#define VERTEX_KEY_LENGHT 18
#define FRAGMENT_KEY_LENGHT 20
#define COMPUTE_KEY_LENGHT 19

namespace ShaderImporter
{
	void Import(const char* path, R_Shader*);
	
	void Load(R_Shader* shader);
	void Save(std::string& shaderCode, int uid);
	void SaveAsAsset(int shaderId, const char* code);
}

#endif // !__SHADER_IMPORTER_H__
