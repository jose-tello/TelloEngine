#include "Application.h"
#include "M_AssetManager.h"

#include "M_Editor.h"

#include "PhysFS/include/physfs.h"
#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

M_AssetManager::M_AssetManager(bool start_enabled) : Module(start_enabled)
{
	PHYSFS_init(nullptr);

	// We only need this when compiling in debug. In Release we don't need it.
	PHYSFS_mount(".", nullptr, 1);
	PHYSFS_mount("Assets.zip", nullptr, 1);
}


M_AssetManager::~M_AssetManager()
{
	PHYSFS_deinit();
}


bool M_AssetManager::Init()
{
	if (PHYSFS_isInit())
		App->editor->AddLog("Log: Asset Manager is succefully loaded");
	else
		App->editor->AddLog("ERROR: Failed loading Asset Manager");

	PHYSFS_permitSymbolicLinks(1);
	PHYSFS_mount("Assets", nullptr, 1);

	return true;
}


bool M_AssetManager::CleanUp()
{
	pathVec.clear();
	bufferVec.clear();
	bytesVec.clear();

	return false;
}


// Return the bytes of a PhysFS filehandle
unsigned int M_AssetManager::ReadBytes(const char* path, char** buffer) const
{
	uint ret = 0;

	// The reading offset is set to the first byte of the file.
	// Returns a filehandle on success that we will need for the PHYSFS_fileLength
	std::string newPath(path);
	for (int i = 0; i < newPath.size(); ++i)
	{
		if (newPath[i] == '\\')
			newPath[i] = '/';
	}
	PHYSFS_getSearchPath();
	PHYSFS_file* file = PHYSFS_openRead(newPath.c_str());
	// Check for end-of-file state on a PhysicsFS filehandle.
	if (!PHYSFS_eof(file))
	{
		// Get total length of a file in bytes
		uint lenght = PHYSFS_fileLength(file);
		*buffer = new char[lenght];

		// Read data from a PhysicsFS firehandle. Returns a number of bytes read.
		uint bytes = PHYSFS_readBytes(file, *buffer, lenght);

		if (bytes != lenght)
		{
			LOG("%s", path, "ERROR: %s", PHYSFS_getLastError());
			delete[] buffer;
		}
		else
			ret = bytes;
	}
	else
		LOG("%s", path, "ERROR: %s", PHYSFS_getLastError());


	// Close a PhysicsFS firehandle
	PHYSFS_close(file);

	return ret;
}


unsigned int M_AssetManager::Load(const char* path, char* buffer)
{
	uint bytes;

	 int check = CheckPath(path);
	if (check == -1)
	{
		bytes = ReadBytes(path, &buffer);

		bufferVec.push_back(buffer);
		bytesVec.push_back(bytes);
	}

	else
	{
		buffer = bufferVec[check];
		bytes =	bytesVec[check];
	}

	return bytes;
}



int M_AssetManager::CheckPath(const char* path)
{
	std::string string(path);

	if (pathVec.empty() == true)
	{
		pathVec.push_back(string);
		return -1;
	}

	int numBuffers = pathVec.size();
	for (int i = 0; i < numBuffers; i++)
	{
		if (strcmp(pathVec[i].c_str(), path) == 0)
		{
			return i;
		}
	}

	pathVec.push_back(string);
	return -1;
}