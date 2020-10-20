#ifndef __M_ASSET_MANAGER_H__
#define __M_ASSET_MANAGER_H__

#include "Module.h"

#include <vector>
#include <string>

class M_AssetManager : public Module
{
public:
	M_AssetManager(bool start_enabled = true);
	~M_AssetManager();

	bool Init() override;

	bool CleanUp() override;

	// Return the bytes of a PhysFS filehandle
	unsigned int ReadBytes(const char* path, char** buffer) const;
	unsigned int Load(const char* path, char* buffer);

private:
	int CheckPath(const char*);

private:
	std::vector<std::string> pathVec;
	std::vector<char*> bufferVec;
	std::vector<unsigned int> bytesVec;
};

#endif // __M_ASSET_MANAGER_H__
