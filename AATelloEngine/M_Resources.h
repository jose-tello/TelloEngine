#ifndef __M_RESOURCES_H__
#define __M_RESOURCES_H__

#include "Module.h"
#include <string>
#include <map>

class Resource;
struct GameObject;
struct Config;

class M_Resources : public Module
{
public:
	M_Resources(bool startEnabled = true);
	~M_Resources() override;

	bool Start() override;

	Resource* RequestResource(int uid);
	int CreateMeta(const char* assetPath);

	//Used when the created resource does not have a .meta (meshes)
	void PushResource(Resource*, int id);

	int SearchMetaFile(const char*);

	void DragAndDropImport(const char*, GameObject*);

private:
	void LoadAllAssets(const char* folder = "/Assets/");

	void CreateResource(int uid, int type, const char* path);
	bool CheckMetaExist(std::string& fileName, std::string& meta, const char* folder);
	bool CheckLibFileExists(int id, int resourceType);
	
	void CreateResourceFromMeta(const char* metaPath);
	void CreateResourcesFromModelMeta(Config&);


private:
	std::map<int, Resource*> resources;
};

#endif // !__M_RESOURCES_H__
