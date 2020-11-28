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

	int GetUid() const;
	unsigned int GetReferenceCount() const;
	std::string GetAssetPath() const;

protected:
	int uid;

	bool isLoaded;

private:
	std::string assetPath;
	RESOURCE_TYPE type;

	unsigned int referenceCount;
};

#endif // !__RESOURCE_H__

