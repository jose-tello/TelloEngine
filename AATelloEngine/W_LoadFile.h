#ifndef __W_LOAD_FILE_H__
#define __W_LOAD_FILE_H__

#include "E_Window.h"

#define BACKSPACE 42

class W_LoadFile : public E_Window
{
public:
	W_LoadFile(bool open = true);
	~W_LoadFile() override;

	bool Draw() override;

private:
	void DrawLoadWindow();
	void DrawDirectory(const char* directory, const char* filterExtension, bool fromResource = false);
};

#endif // !__W_LOAD_FILE_H__
