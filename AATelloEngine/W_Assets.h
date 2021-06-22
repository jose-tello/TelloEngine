#ifndef __W_ASSETS_H__
#define __W_ASSETS_H__

#include "E_Window.h"

#define BACKSPACE 42

class W_Assets : public E_Window
{
public:
	W_Assets(bool open = true);
	~W_Assets() override;

	bool Draw() override;

private:
	void DrawLoadWindow();
	void DrawDirectory(const char* directory, const char* filterExtension, bool fromResource = false);
};

#endif // !__W_ASSETS_H__
