#ifndef __TEXTURE_IMPORTER_H__
#define __TEXTURE_IMPORTER_H__

namespace TextureImporter
{
	void Init();

	unsigned int Import(const char* path, bool pathFromFileManager = true);
	
	unsigned int Load(const char* path);
	void Save(const char* path);
}
#endif // !__TEXTURE_IMPORTER_H__

