#ifndef __IMAGE_IMPORTER_H__
#define __IMAGE_IMPORTER_H__

namespace ImageImporter
{
	void Init();

	unsigned int Load(const char* path, bool pathFromFileManager = true);
}

#endif // !__IMAGE_IMPORTER_H__
