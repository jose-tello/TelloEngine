#ifndef __M_RESOURCES_H__
#define __M_RESOURCES_H__

#include "Module.h"
#include <string>
#include <map>

class Resource;

class M_Resources : public Module
{
public:
	M_Resources(bool startEnabled = true);
	~M_Resources() override;

	bool Start() override;

	Resource* RequestResource(int uid);

private:
	void LoadAllAssets(const char* folder = "/Assets/");

	void CreateResource(int uid, int type, const char* path);
	bool CheckResourceExist(std::string& fileName, std::string& meta, const char* folder);
	void CreateResourceFromMeta(const char* metaPath);
	void CreateMeta(const char* assetPath);


private:
	std::map<int, Resource*> resources;
};

#endif // !__M_RESOURCES_H__
