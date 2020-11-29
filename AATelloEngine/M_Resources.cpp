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
#include "C_Material.h"
#include "C_Mesh.h"

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
			std::string path(folder);
			path.append(files[i]);

			CheckMetaIsUpdated(path.c_str());
			continue;
		}

		else
		{
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


int M_Resources::CreateMeta(const char* assetPath, int uid)
{
	if (uid == 0)
	{
		LCG randomNumber;
		uid = randomNumber.IntFast();
	}
	
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
		id = CreateMeta(filePath.c_str());

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

		C_Material* material;
		Component* mat = object->GetComponent(COMPONENT_TYPE::MATERIAL);
		if (mat == nullptr)
		{
			material = new C_Material();
			object->AddComponent(material);
		}
		else
			material = (C_Material*)mat;

		material->SetTexture(id);
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
		switch (resource->GetType())
		{
		case RESOURCE_TYPE::MESH:
		{
			if (object == nullptr)
			{
				object = new GameObject(nullptr);
				App->scene->AddGameObject(object);
			}

			C_Mesh* mesh;
			Component* meshComp = object->GetComponent(COMPONENT_TYPE::MESH);
			if (meshComp == nullptr)
			{
				mesh = new C_Mesh();
				object->AddComponent(mesh);
			}
			else
				mesh = (C_Mesh*)meshComp;

			mesh->SetMesh(id);
		}
		break;

		case RESOURCE_TYPE::MODEL:
			ModelImporter::LoadToScene((R_Model*)resource);
			break;

		case RESOURCE_TYPE::MATERIAL:
		{
			if (object == nullptr)
			{
				object = new GameObject(nullptr);
				App->scene->AddGameObject(object);
			}

			C_Material* material;
			Component* mat = object->GetComponent(COMPONENT_TYPE::MATERIAL);
			if (mat == nullptr)
			{
				material = new C_Material();
				object->AddComponent(material);
			}
			else
				material = (C_Material*)mat;

			material->SetMaterial(id);
		}
		break;

		case RESOURCE_TYPE::TEXTURE:
		{
			if (object == nullptr)
			{
				object = new GameObject(nullptr);
				App->scene->AddGameObject(object);
			}

			C_Material* material;
			Component* mat = object->GetComponent(COMPONENT_TYPE::MATERIAL);
			if (mat == nullptr)
			{
				material = new C_Material();
				object->AddComponent(material);
			}
			else
				material = (C_Material*)mat;

			material->SetTexture(id);
		}
		break;

		default:
			assert("Forgot to add resources");
			break;
		}
	}
	
}


void M_Resources::GetAllResources(std::vector<Resource*>& meshes, std::vector<Resource*>& materials, std::vector<Resource*>& textures, 
								  std::vector<Resource*>& models)
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


void M_Resources::CreateResource(int uid, int type, const char* path)
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


bool M_Resources::CheckMetaIsUpdated(const char* meta)
{
	char* fileBuffer;
	App->fileManager->Load(meta, &fileBuffer);

	Config metaNode(fileBuffer);

	unsigned __int64 metaTime = metaNode.GetNum("time");
	const char* assetPath = metaNode.GetString("asset_path");

	unsigned __int64 assetTime = App->fileManager->GetLastModTime(assetPath);

	if (assetTime == -1)	//asset was deleted
		DeleteMetaAndLibFiles(metaNode);

	else if (assetTime != metaTime)
	{
		int type = metaNode.GetNum("type");
		if ((RESOURCE_TYPE)type == RESOURCE_TYPE::MODEL)
			DeleteMetaAndLibFiles(metaNode);

		else if ((RESOURCE_TYPE)type == RESOURCE_TYPE::TEXTURE)
		{
			int id = metaNode.GetNum("uid");
			DeleteLibFile(id, (int)RESOURCE_TYPE::TEXTURE);
			DeleteResource(id);
			CreateMeta(assetPath, id);
			
		}
	}

	delete[] fileBuffer;
	fileBuffer = nullptr;

	return true;
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