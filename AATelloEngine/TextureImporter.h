#ifndef __TEXTURE_IMPORTER_H__
#define __TEXTURE_IMPORTER_H__

class R_Texture;

namespace TextureImporter
{
	void Init();

	void Import(const char* path, R_Texture* texture);
	
	void Load(R_Texture* texture);
	void Save(R_Texture* texture);
}
#endif // !__TEXTURE_IMPORTER_H__

