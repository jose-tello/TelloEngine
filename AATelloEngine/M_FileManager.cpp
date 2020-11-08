#include "M_FileManager.h"

#include "Application.h"
#include "M_Editor.h"
#include "W_Inspector.h"

#include "GameObject.h"
#include "C_Material.h"

#include "MeshImporter.h"
#include "ImageImporter.h"

//#include <fstream>
//#include <filesystem>
#include <assert.h>

#include "PhysFS/include/physfs.h"
#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

M_FileManager::M_FileManager(bool startEnabled) : Module(startEnabled)
{
	PHYSFS_init(nullptr);

	// We only need this when compiling in debug. In Release we don't need it.
	PHYSFS_mount(".", nullptr, 1);
	PHYSFS_mount("Assets.zip", nullptr, 1);
}


M_FileManager::~M_FileManager()
{
	PHYSFS_deinit();
}


bool M_FileManager::Init()
{
	if (PHYSFS_isInit())
		App->editor->AddLog("Log: Asset Manager is succefully loaded");

	else
	{
		App->editor->AddLog("ERROR: Failed loading Asset Manager");
		return false;
	}

	PHYSFS_permitSymbolicLinks(1);
	PHYSFS_mount("Assets", nullptr, 1);

	return true;
}

bool M_FileManager::Start()
{
	ImageImporter::Init();
	ModelImporter::InitDebuggerOptions();

	return true;
}


bool M_FileManager::CleanUp()
{
	ModelImporter::CleanUp();

	return false;
}


void M_FileManager::LoadFromExporter(const char* path)
{
	FILE_TYPE type = GetFileType(path);

	switch (type)
	{
	case FILE_TYPE::MODEL:
	{
		std::string filePath(path);
		AdaptPath(filePath);

		char* buffer = nullptr;
		unsigned int bytes = ReadBytes(filePath.c_str(), &buffer);

		ModelImporter::Import(buffer, bytes);
		App->editor->AddLog("Log: Loaded a model");

		delete[] buffer;
		buffer = nullptr;
	}
	break;

	case FILE_TYPE::TEXTURE:

		GameObject* object = App->editor->GetFocusedGameObject();
		if (object != nullptr)
		{
			unsigned int texId = ImageImporter::Import(path);
			App->editor->AddLog("Log: Loaded a texture");

			C_Material* material = (C_Material*)object->GetComponent(COMPONENT_TYPE::MATERIAL);
			if (material == nullptr)
			{
				material = new C_Material();
				object->AddComponent(material);
			}
			 

			material->SetTexture(texId);
			material->texturePath = path;
		}

		else
			App->editor->AddLog("WARNING: Dont have any game object selected");
		
		break;
	}

}


void M_FileManager::Save(const char* fileName, const void* buffer, unsigned int size) const
{
	PHYSFS_File* file = PHYSFS_openWrite(fileName);

	if (file != nullptr)
	{
		unsigned int writtedBytes = PHYSFS_write(file, buffer, 1, size);

		if (writtedBytes != size)
			App->editor->AddLog("ERROR: Error while writting file %s: %s", fileName, PHYSFS_getLastError());
		
		if (PHYSFS_close(file) == 0)
			App->editor->AddLog("ERROR: Error while closing file %s: %s", fileName, PHYSFS_getLastError());
	}

}


void M_FileManager::AdaptPath(std::string& path)
{
	path = NormalizePath(path.c_str());
	TransformPath(path);
}


// Return the bytes of a PhysFS filehandle
unsigned int M_FileManager::ReadBytes(const char* path, char** buffer) const
{
	uint ret = 0;

	// The reading offset is set to the first byte of the file.
	// Returns a filehandle on success that we will need for the PHYSFS_fileLength

	PHYSFS_file* file = PHYSFS_openRead(path);
	// Check for end-of-file state on a PhysicsFS filehandle.
	if (file != NULL)
	{
		if (!PHYSFS_eof(file))
		{
			// Get total length of a file in bytes
			uint lenght = PHYSFS_fileLength(file);
			*buffer = new char[lenght];

			// Read data from a PhysicsFS firehandle. Returns a number of bytes read.
			uint bytes = PHYSFS_readBytes(file, *buffer, lenght);

			if (bytes != lenght)
			{
				App->editor->AddLog("%s", path, "ERROR: %s", PHYSFS_getLastError());
				delete[] buffer;
			}
			else
				ret = bytes;
		}
		else
			App->editor->AddLog("%s", path, "ERROR: %s", PHYSFS_getLastError());
	}
	else
		App->editor->AddLog("%s", path, "ERROR: %s", PHYSFS_getLastError());

	// Close a PhysicsFS firehandle
	PHYSFS_close(file);

	return ret;
}


void M_FileManager::SplitPath(const char* fullPath, std::string* path, std::string* file, std::string* extension)
{
	if (fullPath != nullptr)
	{
		std::string full(fullPath);
		size_t posSeparator = full.find_last_of("\\/");
		size_t posDot = full.find_last_of(".");

		if (path != nullptr)
		{
			if (posSeparator < full.length())
				*path = full.substr(0, posSeparator + 1);

			else
				path->clear();
		}

		if (file != nullptr)
		{
			if (posSeparator < full.length())
				*file = full.substr(posSeparator + 1, posDot - posSeparator - 1);

			else
				*file = full.substr(0, posDot);
		}

		if (extension != nullptr)
		{
			if (posDot < full.length())
				*extension = full.substr(posDot + 1);

			else
				extension->clear();
		}
	}
}


std::string M_FileManager::NormalizePath(const char* path)
{
	std::string newPath(path);
	for (int i = 0; i < newPath.size(); ++i)
	{
		if (newPath[i] == '\\')
			newPath[i] = '/';
	}
	return newPath;
}


//TODO: this is SOOOOOO dirty but i can't find how to do it any other way, so ill just deal with it later 
void M_FileManager::TransformPath(std::string& path)
{
	int itPos = path.find("Assets");
	path = path.substr(itPos + ASSETS_LENGHT, path.length());
}


/*bool M_FileManager::DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath)
{
	std::string fileStr, extensionStr;
	SplitPath(file, nullptr, &fileStr, &extensionStr);

	relativePath = relativePath.append(dstFolder).append("/") + fileStr.append(".") + extensionStr;
	std::string finalPath = std::string(*PHYSFS_getSearchPath()).append("/") + relativePath;

	std::ifstream source;	//File that we want to copy
	source.open(file, std::ios::binary);

	std::ofstream destiny;	//File where we will copy the file
	destiny.open(finalPath.c_str(), std::ios::binary);

	//Check everything is loaded ok
	bool srcOpen = source.is_open();
	bool dstOpen = destiny.is_open();

	if (srcOpen == true && dstOpen == true)
	{
		destiny << source.rdbuf();

		source.close();
		destiny.close();
		return true;
	}

	else
	{
		source.close();
		destiny.close();

		App->editor->AddLog("ERROR: Could not duplicate the file");
		return false;
	}
}*/


FILE_TYPE M_FileManager::GetFileType(const char* path)
{
	std::string extension;
	SplitPath(path, nullptr, nullptr, &extension);

	if (extension == "FBX" || extension == "fbx")
		return FILE_TYPE::MODEL;

	else if (extension == "PNG" || extension == "png" || extension == "DDS" ||extension == "dds")
		return FILE_TYPE::TEXTURE;

	else
	{
		assert(true, "ERROR: not supported tipe of file");
		return FILE_TYPE::NONE;
	}
}


