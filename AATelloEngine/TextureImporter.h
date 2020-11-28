#ifndef __TEXTURE_IMPORTER_H__
#define __TEXTURE_IMPORTER_H__

class R_Texture;

namespace TextureImporter
{
	void Init();

	void Import(const char* path, R_Texture* resource);
	
	void Load(R_Texture*);
	void Save(R_Texture*);
}
#endif // !__TEXTURE_IMPORTER_H__

