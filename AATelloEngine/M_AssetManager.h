#ifndef __M_ASSET_MANAGER_H__
#define __M_ASSET_MANAGER_H__

#include "Module.h"

#include <vector>
#include <string>

enum class FILE_TYPE : int
{
	NONE = -1,
	MODEL,
	TEXTURE,
	MAX
};


class M_AssetManager : public Module
{
public:
	M_AssetManager(bool start_enabled = true);
	~M_AssetManager();

	bool Init() override;
	bool Start() override;

	bool CleanUp() override;
	
	void LoadFromExporter(const char* path);

private:

	std::string NormalizePath(const char*);
	void SplitPath(const char* fullPath, std::string* path, std::string* file, std::string* extension);

	bool DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath);
	FILE_TYPE GetFileType(const char* extension);

	unsigned int ReadBytes(const char* path, char** buffer) const;

};

#endif // __M_ASSET_MANAGER_H__
