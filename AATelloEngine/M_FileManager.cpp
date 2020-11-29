#include "M_FileManager.h"

#include "Application.h"
#include "M_Editor.h"
#include "W_Inspector.h"

#include "GameObject.h"
#include "C_Material.h"

#include "M_Resources.h"
#include "Resource.h"

#include "ModelImporter.h"
#include "MaterialImporter.h"
#include "TextureImporter.h"

//#include <fstream>
//#include <filesystem>
#include <assert.h>

#include "PhysFS/include/physfs.h"
#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

M_FileManager::M_FileManager(bool startEnabled) : Module(startEnabled)
{
	PHYSFS_init(nullptr);

	PHYSFS_setWriteDir(".");
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
		App->editor->AddLog("[ERROR]: Failed loading Asset Manager");
		return false;
	}

	PHYSFS_permitSymbolicLinks(2);
	PHYSFS_mount("Assets", nullptr, 1);
	PHYSFS_mount("Library", nullptr, 2);

	return true;
}

bool M_FileManager::Start()
{
	TextureImporter::Init();
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
	RESOURCE_TYPE type = GetFileType(path);

	switch (type)
	{
	case RESOURCE_TYPE::MODEL:
	{
		App->resourceManager->DragAndDropImport(path, nullptr);
		App->editor->AddLog("Log: Added a model");
	}
	break;

	case RESOURCE_TYPE::TEXTURE:
		GameObject* focusedGO = App->editor->GetFocusedGameObject();
		if (focusedGO != nullptr)
		{
			App->resourceManager->DragAndDropImport(path, focusedGO);
			App->editor->AddLog("Log: Added a texture");
		}
		else
			App->editor->AddLog("[WARNING]: No focused game object");
		
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
			App->editor->AddLog("[ERROR]: Error while writting file %s: %s", fileName, PHYSFS_getLastError());
		
		if (PHYSFS_close(file) == 0)
			App->editor->AddLog("[ERROR]: Error while closing file %s: %s", fileName, PHYSFS_getLastError());
	}

	else
		App->editor->AddLog("[ERROR]: Error while opening file %s: %s", fileName, PHYSFS_getLastError());
}


unsigned int M_FileManager::Load(const char* fileName, char** buffer) const
{
	PHYSFS_File* file = PHYSFS_openRead(fileName);

	if (file != nullptr)
	{
		PHYSFS_uint64 size = PHYSFS_fileLength(file);

		if (size != 0)
		{
			*buffer = new char[size + 1];

			PHYSFS_readBytes(file, *buffer, size);

			if (PHYSFS_close(file) == 0)
				App->editor->AddLog("[ERROR]: Error while closing file %s: %s", fileName, PHYSFS_getLastError());

			(*buffer)[size] = '\0'; //End of file signal
			return size;
		}
	}

	else
		App->editor->AddLog("[ERROR]: Error while opening file %s: %s", fileName, PHYSFS_getLastError());

	return 0;
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
				App->editor->AddLog("%s", path, "[ERROR]: %s", PHYSFS_getLastError());
				delete[] buffer;
			}
			else
				ret = bytes;
		}
		else
			App->editor->AddLog("%s", path, "[ERROR]: %s", PHYSFS_getLastError());
	}
	else
		App->editor->AddLog("%s", path, "[ERROR]: %s", PHYSFS_getLastError());

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


std::string M_FileManager::RemoveExtension(const char* path)
{
	std::string ret(path);
	return ret.substr(0, ret.find_last_of("."));
}


void M_FileManager::ExploreDirectory(const char* directory, std::vector<std::string>& filesVec, std::vector<std::string>& dirVec) const
{
	char** files = PHYSFS_enumerateFiles(directory);
	char** iterator;

	std::string dir(directory);

	for (iterator = files; *iterator != nullptr; iterator++)
	{
		if (PHYSFS_isDirectory((dir + *iterator).c_str()))
		{
			dirVec.push_back(*iterator);
			dirVec.back() += "/";
		}
		else
			filesVec.push_back(*iterator);
	}

	PHYSFS_freeList(files);
}


unsigned __int64 M_FileManager::GetLastModTime(const char* file) const
{
	return PHYSFS_getLastModTime(file);
}


RESOURCE_TYPE M_FileManager::GetFileType(const char* path)
{
	std::string extension;
	SplitPath(path, nullptr, nullptr, &extension);

	if (extension == "FBX" || extension == "fbx")
		return RESOURCE_TYPE::MODEL;

	else if (extension == "PNG" || extension == "png" || extension == "DDS" || extension == "dds" || extension == "jpg" || extension == "tga")
		return RESOURCE_TYPE::TEXTURE;

	else
	{
		assert(true, "ERROR: not supported type of file");
		return RESOURCE_TYPE::NONE;
	}
}


bool M_FileManager::FileExists(const char* file)
{
	return PHYSFS_exists(file);
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



