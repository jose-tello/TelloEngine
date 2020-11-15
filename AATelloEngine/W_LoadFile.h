#ifndef __W_LOAD_FILE_H__
#define __W_LOAD_FILE_H__

#include "E_Window.h"

class W_LoadFile : public E_Window
{
public:
	W_LoadFile(bool open = true);
	~W_LoadFile();

	bool Draw();

private:
	void DrawLoadWindow();
	void DrawDirectory(const char* directory, const char* filterExtension);
};

#endif // !__W_LOAD_FILE_H__
