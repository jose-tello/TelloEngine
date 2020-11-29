#include "Resource.h"

Resource::Resource(int uid, const char* assetPath, RESOURCE_TYPE type) :
	uid(uid),
	assetPath(assetPath),
	type(type),

	isLoaded(false),
	referenceCount(0)
{
}


Resource::~Resource()
{
}


bool Resource::IsLoaded() const
{
	return isLoaded;
}


void Resource::Load()
{
}


void Resource::UnLoad()
{
}


int Resource::GetUid() const
{
	return uid;
}


unsigned int Resource::GetReferenceCount() const
{
	return referenceCount;
}


std::string Resource::GetAssetPath() const
{
	return assetPath;
}


RESOURCE_TYPE Resource::GetType() const
{
	return type;
}