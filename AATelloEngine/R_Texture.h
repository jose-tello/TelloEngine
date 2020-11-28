#ifndef __R_TEXTURE_H__
#define __R_TEXTURE_H__

#include "Resource.h"

class R_Texture : public Resource
{
public:
	R_Texture(int uid, const char* assetPath, RESOURCE_TYPE type);
	~R_Texture() override;

	void Load() override;
	void UnLoad() override;

	unsigned int GetTextureId() const;
	void SetTextureId(unsigned int texId);
	void GetTextureSize(int& w, int& h) const;

private:
	void InitTextureSize();

private:
	unsigned int textureId;

public:
	int textureWidth;
	int textureHeight;
};
#endif // !__R_TEXTURE_H__
