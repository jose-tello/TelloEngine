#ifndef __M_ASSET_MANAGER_H__
#define __M_ASSET_MANAGER_H__

#include "Module.h"

#include <vector>
#include <string>

#define ASSETS_LENGHT 7
#define MESH_LIBRARY "/Library/Mesh/"
#define MATERIAL_LIBRARY "/Library/Material/"
#define TEXTURE_LIBRARY "/Library/Textures/"
#define SCENE_LIBRARY "/Library/Scenes/"
#define MODEL_LIBRARY "/Library/Model/"

enum class RESOURCE_TYPE;

class M_FileManager : public Module
{
public:
	M_FileManager(bool startEnabled = true);
	~M_FileManager() override;

	bool Init() override;
	bool Start() override;

	bool CleanUp() override;
	
	void LoadFromExporter(const char* path);

	void Save(const char* fileName, const void* buffer, unsigned int size) const;
	unsigned int Load(const char* fileName, char** buffer) const;

	void AdaptPath(std::string& path);
	unsigned int ReadBytes(const char* path, char** buffer) const;

	std::string NormalizePath(const char*);
	void SplitPath(const char* fullPath, std::string* path, std::string* file, std::string* extension);
	std::string RemoveExtension(const char* path);

	void ExploreDirectory(const char* directory, std::vector<std::string>& filesVec, std::vector<std::string>& dirVec) const;

	void RemoveFile(const char* path);
	unsigned __int64 GetLastModTime(const char* file) const;
	RESOURCE_TYPE GetFileType(const char* path);

	bool FileExists(const char*);

private:
	void TransformPath(std::string& path);
	void CreateFolder(const char* directory);

	std::string DuplicateFile(const char* file, const char* dstFolder);
	
};

#endif // __M_ASSET_MANAGER_H__
