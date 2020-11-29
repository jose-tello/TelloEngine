#include "M_Resources.h"

#include "R_Mesh.h"
#include "R_Material.h"
#include "R_Model.h"
#include "R_Texture.h"

#include "ModelImporter.h"
#include "TextureImporter.h"

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
	std::map<int, Resource*>::iterator iterator = resources.find(uid);
	Resource* ret = nullptr;

	if (iterator != resources.end())
	{
		ret = iterator._Ptr->_Myval.second;

		if (ret->IsLoaded() == false)
			ret->Load();
	}	
		
	
	return ret;
}


int M_Resources::CreateMeta(const char* assetPath)
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

	CreateResource(uid, (int)type, assetPath);

	return uid;
}


void M_Resources::PushResource(Resource* resource, int id)
{
	resources.insert(std::pair<int, Resource*>(id, resource));
}


int M_Resources::SearchMetaFile(const char* fileName)
{
	std::string path = App->fileManager->RemoveExtension(fileName);
	path = "/Assets/" + path + ".meta";

	std::string name("/Assets/");
	name += fileName;
	if (App->fileManager->FileExists(path.c_str()))
	{
		std::map<int, Resource*>::iterator it = resources.begin();
		for (it; it != resources.end(); it++)
		{
			RESOURCE_TYPE type = it->second->GetType();
			if (type == RESOURCE_TYPE::MODEL || type == RESOURCE_TYPE::TEXTURE)
			{
				std::string pathName = it->second->GetAssetPath().c_str();
				if (pathName == name.c_str())
					return it->first;
			}
		}
	}

	return 0;
}


void M_Resources::DragAndDropImport(const char* path)
{
	std::string filePath(path);
	App->fileManager->AdaptPath(filePath);

	int id = SearchMetaFile(filePath.c_str());
	if (id == 0)
		id = CreateMeta(filePath.c_str());

	Resource* resource = RequestResource(id);

	switch (resource->GetType())
	{
	case RESOURCE_TYPE::TEXTURE:

		break;

	case RESOURCE_TYPE::MODEL:
		ModelImporter::LoadToScene((R_Model*)resource);
		break;

	default:
		break;
	}
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
			if (CheckMetaExist(files[i], files[i + 1], folder) == true)
				continue;

		if (i != 0)
			if (CheckMetaExist(files[i], files[i - 1], folder) == true)
				continue;


		std::string path = folder;
		path += files[i].c_str();
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
	Resource* resource;// = new Resource(uid, path, (RESOURCE_TYPE)type);

	//TODO: do the corresponding news and subclasses and import
	switch ((RESOURCE_TYPE)type)
	{
	case RESOURCE_TYPE::MODEL:
		resource = new R_Model(uid, path, (RESOURCE_TYPE)type);
		
		if (CheckLibFileExists(uid, type) == false)
			ModelImporter::Import(path, (R_Model*)resource);

		break;

	case RESOURCE_TYPE::TEXTURE:
		resource = new R_Texture(uid, path, (RESOURCE_TYPE)type);
		
		if (CheckLibFileExists(uid, type) == false)
			TextureImporter::Import(path, (R_Texture*)resource);

		break;

	default:
		assert("Forgot to add resource type");
		break;
	}

	resources.insert(std::pair<int, Resource*>(uid, resource));
}


bool M_Resources::CheckMetaExist(std::string& fileName, std::string& meta, const char* folder)
{
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


bool M_Resources::CheckLibFileExists(int id, int resourceType)
{
	std::string path;
	switch ((RESOURCE_TYPE)resourceType)
	{
	case RESOURCE_TYPE::MESH:
		path = MESH_LIBRARY + std::to_string(id);
		break;

	case RESOURCE_TYPE::MODEL:
		path = MODEL_LIBRARY + std::to_string(id);
		break;

	case RESOURCE_TYPE::MATERIAL:
		path = MATERIAL_LIBRARY + std::to_string(id);
		break;

	case RESOURCE_TYPE::TEXTURE:
		path = TEXTURE_LIBRARY + std::to_string(id);
		break;
	
	default:
		assert("Forgot to add resources");
		break;
	}

	return App->fileManager->FileExists(path.c_str());
}


void M_Resources::CreateResourceFromMeta(const char* metaPath)
{
	char* fileBuffer;
	App->fileManager->Load(metaPath, &fileBuffer);

	Config node(fileBuffer);

	int type = node.GetNum("type");

	if (type == (int)RESOURCE_TYPE::MODEL)
		CreateResourcesFromModelMeta(node);

	CreateResource(node.GetNum("uid"), type, node.GetString("asset_path"));

	delete[] fileBuffer;
	fileBuffer = nullptr;
}


void M_Resources::CreateResourcesFromModelMeta(Config& rootNode)
{
	ConfigArray meshArray = rootNode.GetArray("meshes");

	int meshesCount = meshArray.GetSize();
	for (int i = 0; i < meshesCount; i++)
	{
		Config node = meshArray.GetNode(i);
		int id = node.GetNum("id");
		PushResource(new R_Mesh(id, rootNode.GetString("asset_path"), RESOURCE_TYPE::MESH), id);
	}

	ConfigArray materialArray = rootNode.GetArray("materials");

	int materialsCount = materialArray.GetSize();
	for (int i = 0; i < materialsCount; i++)
	{
		Config node = materialArray.GetNode(i);
		int id = node.GetNum("id");
		PushResource(new R_Material(id, rootNode.GetString("asset_path"), RESOURCE_TYPE::MATERIAL), id);
	}
}