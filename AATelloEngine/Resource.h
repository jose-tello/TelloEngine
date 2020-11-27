#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>

enum class RESOURCE_TYPE : int
{
	NONE = -1,
	MESH,
	MODEL,
	MATERIAL,
	MAX
};


class Resource
{
public:
	Resource(int uid, const char*, RESOURCE_TYPE type = RESOURCE_TYPE::NONE);
	virtual ~Resource();

	bool IsLoaded() const;
	virtual void Load();
	virtual void UnLoad();

	unsigned int GetReferenceCount() const;

private:
	int uid;
	std::string assetPath;
	RESOURCE_TYPE type;

	bool isLoaded;
	unsigned int referenceCount;
};

#endif // !__RESOURCE_H__

