#include "M_Resources.h"

#include "R_Mesh.h"
#include "R_Material.h"
#include "R_Model.h"
#include "R_Texture.h"

#include "ModelImporter.h"
#include "TextureImporter.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Scene.h"

#include "Config.h"

#include "GameObject.h"

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
	UpdateAllAssets();
	return true;
}


bool M_Resources::CleanUp()
{
	std::map<int, Resource*>::iterator it = resources.begin();
	for (it; it != resources.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}

	resources.clear();
	return true;
}


void M_Resources::UpdateAllAssets(const char* folder)
{
	std::vector<std::string> files;
	std::vector<std::string> folderDirs;

	App->fileManager->ExploreDirectory(folder, files, folderDirs);

	int filesCount = files.size();
	for (int i = 0; i < filesCount; i++)
	{
		if (files[i].find(".meta") != -1)										// Check if its a .meta
		{
			UpdateMetaFile(files[i], folder);
			continue;
		}

		else
		{
			std::string* nextFile = nullptr;
			std::string* previousFile = nullptr;
			if (i < filesCount - 1)												//I assume all .meta are adjacent to its asset
				nextFile = &files[i + 1];

			if (i != 0)
				previousFile = &files[i - 1];

			UpdateFile(files[i], previousFile, nextFile, folder);
		}
	}

	int foldersCount = folderDirs.size();
	for (int i = 0; i < foldersCount; i++)
	{
		std::string path = folder;
		path += folderDirs[i];

		UpdateAllAssets(path.c_str());
	}
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


int M_Resources::CreateResource(const char* assetPath, int id)
{
	id = CreateMeta(assetPath, id);
	InitResource(id, (int)App->fileManager->GetFileType(assetPath), assetPath);

	return id;
}


void M_Resources::PushResource(Resource* resource, int id)
{
	resources.insert(std::pair<int, Resource*>(id, resource));
}


int M_Resources::SearchMetaFile(const char* fileName)
{
	std::string path(fileName);

	if (App->fileManager->FileExists(path.c_str()))
	{
		std::map<int, Resource*>::iterator it = resources.begin();
		for (it; it != resources.end(); it++)
		{
			RESOURCE_TYPE type = it->second->GetType();
			if (type == RESOURCE_TYPE::MODEL || type == RESOURCE_TYPE::TEXTURE)
			{
				std::string pathName = it->second->GetAssetPath().c_str();
				if (pathName == path.c_str())
					return it->first;
			}
		}
	}

	return 0;
}


void M_Resources::DragAndDropImport(const char* path, GameObject* object)
{
	std::string filePath(path);

	int id = SearchMetaFile(filePath.c_str());
	if (id == 0)
		id = CreateResource(filePath.c_str());

	Resource* resource = RequestResource(id);

	switch (resource->GetType())
	{
	case RESOURCE_TYPE::TEXTURE:
	{
		if (object == nullptr)
		{
			std::string name;
			App->fileManager->SplitPath(path, nullptr, &name, nullptr);
			object = new GameObject(name, nullptr, 0);
			App->scene->AddGameObject(object);
		}

		object->AddResource(resource->GetUid(), (int)RESOURCE_TYPE::TEXTURE);
		break;
	}
	case RESOURCE_TYPE::MODEL:
		ModelImporter::LoadToScene((R_Model*)resource);
		break;

	default:
		break;
	}
}


void M_Resources::WindowLoad(int id, GameObject* object)
{
	Resource* resource = RequestResource(id);

	if (resource != nullptr)
	{
		if (resource->GetType() == RESOURCE_TYPE::MODEL)
			ModelImporter::LoadToScene((R_Model*)resource);

		else
		{
			if (object == nullptr)
			{
				object = new GameObject(nullptr);
				App->scene->AddGameObject(object);
			}

			object->AddResource(resource->GetUid(), (int)resource->GetType());
		}
	}
	
}


void M_Resources::GetAllResources(std::vector<Resource*>& meshes, std::vector<Resource*>& materials, 
								  std::vector<Resource*>& textures, std::vector<Resource*>& models)
{
	std::map<int, Resource*>::iterator it = resources.begin();
	for (it; it != resources.end(); it++)
	{
		switch (it->second->GetType())
		{
		case RESOURCE_TYPE::MESH:
			meshes.push_back(it->second);
			break;

		case RESOURCE_TYPE::MODEL:
			models.push_back(it->second);
			break;

		case RESOURCE_TYPE::MATERIAL:
			materials.push_back(it->second);
			break;

		case RESOURCE_TYPE::TEXTURE:
			textures.push_back(it->second);
			break;

		default:
			assert("Forgot to add resources");
			break;
		}
	}
}


void M_Resources::UpdateFile(std::string& file, std::string* previousFile, std::string* nextFile, const char* folder)
{
	if (previousFile != nullptr)
	{
		if (CheckMetaExist(file, *previousFile, folder) == true)
		{
			std::string path(folder);
			path.append(previousFile->c_str());
			InitResourceFromMeta(path.c_str());
			return;
		}
	}

	if (nextFile != nullptr)
	{
		if (CheckMetaExist(file, *nextFile, folder) == true)
		{
			std::string path(folder);
			path.append(nextFile->c_str());
			InitResourceFromMeta(path.c_str());
			return;
		}
	}

	std::string path = folder;
	path += file.c_str();
	CreateResource(path.c_str());
}


void M_Resources::UpdateMetaFile(std::string& file, const char* folder)
{
	std::string path(folder);
	path.append(file);

	char* fileBuffer;
	App->fileManager->Load(path.c_str(), &fileBuffer);

	Config metaNode(fileBuffer);

	float timeDiference = CheckMetaIsUpdated(metaNode);

	if (timeDiference == -1)	//asset was deleted
		DeleteMetaAndLibFiles(metaNode);

	else if (timeDiference > 0)
	{
		int type = metaNode.GetNum("type");
		if ((RESOURCE_TYPE)type == RESOURCE_TYPE::MODEL)
			DeleteMetaAndLibFiles(metaNode);

		else if ((RESOURCE_TYPE)type == RESOURCE_TYPE::TEXTURE)
		{
			int id = metaNode.GetNum("uid");
			const char* assetPath = metaNode.GetString("asset_path");
			DeleteLibFile(id, (int)RESOURCE_TYPE::TEXTURE);
			DeleteResource(id);
			CreateResource(assetPath, id);
		}
	}

	delete[] fileBuffer;
	fileBuffer = nullptr;
}


int M_Resources::CreateMeta(const char* assetPath, int uid)
{
	if (uid == 0)
	{
		LCG randomNumber;
		uid = randomNumber.IntFast();
	}

	Config node;
	node.AppendNum("uid", uid);
	node.AppendNum("time", App->fileManager->GetLastModTime(assetPath));
	node.AppendNum("type", (int)App->fileManager->GetFileType(assetPath));
	node.AppendString("asset_path", assetPath);

	std::string metaName = App->fileManager->RemoveExtension(assetPath);
	metaName += ".meta";

	char* fileBuffer;
	unsigned int size = node.Serialize(&fileBuffer);
	App->fileManager->Save(metaName.c_str(), fileBuffer, size);

	delete[] fileBuffer;
	fileBuffer = nullptr;

	return uid;
}


bool M_Resources::CheckMetaExist(std::string& fileName, std::string& meta, const char* folder) const
{
	std::string file = App->fileManager->RemoveExtension(fileName.c_str());
	if ((meta.find(file.c_str()) + file.length()) == meta.find(".meta"))
		return true;

	return false;
}


unsigned __int64 M_Resources::CheckMetaIsUpdated(Config& metaNode) const
{
	unsigned __int64 metaTime = metaNode.GetNum("time");
	const char* assetPath = metaNode.GetString("asset_path");

	unsigned __int64 assetTime = App->fileManager->GetLastModTime(assetPath);

	if (assetTime == -1) // file was deleted
		return -1;

	else if (assetTime < metaTime) //file is older than meta
		return 0;

	unsigned __int64 timeDiference = assetTime - metaTime;
	return timeDiference;
}


bool M_Resources::CheckLibFileExists(int id, int resourceType) const
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


void M_Resources::InitResource(int uid, int type, const char* path)
{
	Resource* resource;

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


void M_Resources::InitResourceFromMeta(const char* metaPath)
{
	char* fileBuffer;
	App->fileManager->Load(metaPath, &fileBuffer);

	Config node(fileBuffer);

	int type = node.GetNum("type");

	if (type == (int)RESOURCE_TYPE::MODEL)
		InitResourcesFromModelMeta(node);

	InitResource(node.GetNum("uid"), type, node.GetString("asset_path"));

	delete[] fileBuffer;
	fileBuffer = nullptr;
}


void M_Resources::InitResourcesFromModelMeta(Config& rootNode)
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


void M_Resources::DeleteResource(int id)
{
	std::map<int, Resource*>::iterator iterator = resources.find(id);

	if (iterator != resources.end())
	{
		delete iterator->second;
		iterator->second = nullptr;
		resources.erase(iterator);
	}
}


void M_Resources::DeleteMetaAndLibFiles(Config& metaNode)
{	
	int type = metaNode.GetNum("type");
	int uid = metaNode.GetNum("uid");

	if ((RESOURCE_TYPE)type == RESOURCE_TYPE::MODEL)
	{
		ConfigArray meshArray = metaNode.GetArray("meshes");

		int meshesCount = meshArray.GetSize();
		for (int i = 0; i < meshesCount; i++)
		{
			Config node = meshArray.GetNode(i);
			int id = node.GetNum("id");

			DeleteLibFile(id, (int)RESOURCE_TYPE::MESH);
		}

		ConfigArray materialArray = metaNode.GetArray("materials");

		int materialsCount = materialArray.GetSize();
		for (int i = 0; i < materialsCount; i++)
		{
			Config node = materialArray.GetNode(i);
			int id = node.GetNum("id");

			DeleteLibFile(id, (int)RESOURCE_TYPE::MATERIAL);
		}
	}

	std::string path = App->fileManager->RemoveExtension(metaNode.GetString("asset_path"));
	path.append(".meta");
	App->fileManager->RemoveFile(path.c_str());

	DeleteLibFile(uid, type);
}


void M_Resources::DeleteLibFile(int uid, int type)
{
	std::string path;

	switch ((RESOURCE_TYPE)type)
	{
	case RESOURCE_TYPE::MESH:
		path = MESH_LIBRARY + std::to_string(uid);
		break;

	case RESOURCE_TYPE::MODEL:
		path = MODEL_LIBRARY + std::to_string(uid);
		break;

	case RESOURCE_TYPE::MATERIAL:
		path = MATERIAL_LIBRARY + std::to_string(uid);
		break;

	case RESOURCE_TYPE::TEXTURE:
		path = TEXTURE_LIBRARY + std::to_string(uid);
		break;

	default:
		assert("Forgot to add resource type");
		break;
	}

	DeleteResource(uid);
	App->fileManager->RemoveFile(path.c_str());
}