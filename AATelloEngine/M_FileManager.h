#ifndef __M_ASSET_MANAGER_H__
#define __M_ASSET_MANAGER_H__

#include "Module.h"

#include <vector>
#include <string>

#define ASSETS_LENGHT 7

enum class FILE_TYPE : int
{
	NONE = -1,
	MODEL,
	TEXTURE,
	MAX
};


class M_FileManager : public Module
{
public:
	M_FileManager(bool start_enabled = true);
	~M_FileManager();

	bool Init() override;
	bool Start() override;

	bool CleanUp() override;
	
	void LoadFromExporter(const char* path);

	void AdaptPath(std::string& path);
	unsigned int ReadBytes(const char* path, char** buffer) const;

	void SplitPath(const char* fullPath, std::string* path, std::string* file, std::string* extension);

private:

	std::string NormalizePath(const char*);
	void TransformPath(std::string& path);

	//bool DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath);
	FILE_TYPE GetFileType(const char* extension);

	

};

#endif // __M_ASSET_MANAGER_H__
