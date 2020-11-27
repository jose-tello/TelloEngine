#include "M_Resources.h"
#include "Resource.h"

#include "Application.h"
#include "M_FileManager.h"

#include "Config.h"

#include "MathGeoLib/src/Algorithm/Random/LCG.h"

#include <vector>
#include <assert.h>

M_Resources::M_Resources(bool startEnabled) : Module(startEnabled)
{
}


M_Resources::~M_Resources()
{
}


bool M_Resources::Start()
{
	LoadAllAssets();
	return true;
}


Resource* M_Resources::RequestResource(int uid)
{
	Resource* ret = resources.find(uid)._Ptr->_Myval.second;

	if (ret->IsLoaded() == false)
		ret->Load();

	return ret;
}


void M_Resources::LoadAllAssets(const char* folder)
{
	std::vector<std::string> files;
	std::vector<std::string> folderDirs;

	App->fileManager->ExploreDirectory(folder, files, folderDirs);

	int filesCount = files.size();
	for (int i = 0; i < filesCount; i++)
	{
		if (files[i].find(".meta") != -1)										// Check its not a .meta
			continue;

		if (i < filesCount - 1)													//I assume all .meta are adjacent to its asset
			if (CheckResourceExist(files[i], files[i + 1], folder) == true)
				continue;
		
		if (i != 0)
			if (CheckResourceExist(files[i], files[i - 1], folder) == true)
				continue;


		std::string path = folder;
		path += "/" + App->fileManager->RemoveExtension(files[i].c_str());
		CreateMeta(path.c_str());
	}

	int foldersCount = folderDirs.size();
	for (int i = 0; i < foldersCount; i++)
	{
		std::string path = folder;
		path += folderDirs[i];

		LoadAllAssets(path.c_str());
	}
}


void M_Resources::CreateResource(int uid, int type, const char* path)
{
	Resource* resource = new Resource(uid, path, (RESOURCE_TYPE)type);

	//TODO: do the corresponding news and subclasses and import
	switch ((RESOURCE_TYPE)type)
	{
	case RESOURCE_TYPE::MESH:
		break;

	case RESOURCE_TYPE::MODEL:
		break;

	case RESOURCE_TYPE::MATERIAL:
		break;

	default:
		assert("Forgot to add resource type");
		break;
	}
}


bool M_Resources::CheckResourceExist(std::string& fileName, std::string& meta, const char* folder)
{
	//REDO
	std::string file = App->fileManager->RemoveExtension(fileName.c_str());
	if ((meta.find(file.c_str()) + file.length()) == meta.find(".meta"))
	{
		std::string path(folder);
		path.append(file + ".meta");
		CreateResourceFromMeta(path.c_str());
		return true;
	}

	return false;
}


void M_Resources::CreateResourceFromMeta(const char* metaPath)
{
	char* fileBuffer;
	App->fileManager->Load(metaPath, &fileBuffer);

	Config node(fileBuffer);
	
	CreateResource(node.GetNum("uid"), node.GetNum("type"), node.GetString("asset_path"));

	delete[] fileBuffer;
	fileBuffer = nullptr;
}


void M_Resources::CreateMeta(const char* assetPath)
{
	LCG randomNumber;
	int uid = randomNumber.IntFast();
	unsigned __int64 time = App->fileManager->GetLastModTime(assetPath);
	RESOURCE_TYPE type = App->fileManager->GetFileType(assetPath);

	Config node;
	node.AppendNum("uid", uid);
	node.AppendNum("time", time);
	node.AppendNum("type", (int)type);
	node.AppendString("asset_path", assetPath);

	std::string metaName = App->fileManager->RemoveExtension(assetPath);
	metaName += ".meta";

	char* fileBuffer;
	unsigned int size = node.Serialize(&fileBuffer);
	App->fileManager->Save(metaName.c_str(), fileBuffer, size);

	delete[] fileBuffer;
	fileBuffer = nullptr;

	//Call create resource
	CreateResource(uid, (int)type, assetPath);
}