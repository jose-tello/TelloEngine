#ifndef __M_RESOURCES_H__
#define __M_RESOURCES_H__

#include "Module.h"
#include <string>
#include <map>
#include <vector>

class Resource;
class R_Mesh;
struct GameObject;
struct Config;

enum class DEFAULT_RESOURCE : int
{
	NONE = 0,
	RASTER_SHADER = 1,
	RAY_TRACING_SHADER = 2
};

class M_Resources : public Module
{
public:
	M_Resources(bool startEnabled = true);
	~M_Resources() override;

	bool Start() override;
	bool CleanUp() override;

	void UpdateAllAssets(const char* folder = "/Assets/");

	Resource* RequestResource(int uid);
	int CreateResource(const char* assetPath, int id = 0);
	//Used when the created resource does not have a .meta (meshes)
	void PushResource(Resource*, int id);

	int SearchMetaFile(const char*);

	void DragAndDropImport(const char*, GameObject*);
	void WindowLoad(int id, GameObject*);

	void GetAllResources(std::vector<Resource*>& meshes, std::vector<Resource*>& materials, std::vector<Resource*>& textures, 
						 std::vector<Resource*>& models, std::vector<Resource*>& shaders);

	std::vector<R_Mesh*> GetAllLoadedMeshes();

	Resource* GetDefaultResource(DEFAULT_RESOURCE resourceId);
	Resource* GetResourceByPath(const char* name);

private:
	void UpdateFile(std::string& file, std::string* previousFile, std::string* nextFile, const char* folder);
	void UpdateMetaFile(std::string& file, const char* folder);

	int CreateMeta(const char* assetPath, int id);
	bool CheckMetaExist(std::string& fileName, std::string& meta, const char* folder) const;
	unsigned __int64 CheckMetaIsUpdated(Config& metaNode) const;

	bool CheckLibFileExists(int id, int resourceType) const;
	
	void InitResource(int uid, int type, const char* path);
	void InitResourceFromMeta(const char* metaPath);
	void InitResourcesFromModelMeta(Config&);

	void DeleteResource(int id);
	void DeleteMetaAndLibFiles(Config& metaNode);
	void DeleteLibFile(int id, int type);

private:
	std::map<int, Resource*> resources;
};

#endif // !__M_RESOURCES_H__
