#ifndef __MODEL_IMPORTER_H__
#define __MODEL_IMPORTER_H__

#define MAX_TEX_COORDS 8

namespace ModelImporter
{
	void InitDebuggerOptions();

	bool Load(char* buffer, unsigned int bytes);

	void CleanUp();
}
#endif // !__MODEL_IMPORTER_H__

