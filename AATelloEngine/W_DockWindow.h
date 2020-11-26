#ifndef __E_DOCK_WINDOW_H__
#define __E_DOCK_WINDOW_H__

#include "E_Window.h"

class W_DockWindow : public E_Window
{
public:
	W_DockWindow(bool open = true);
	~W_DockWindow() override;

	bool Draw() override;

private:

	void CreateDockWindow();
	void DrawAboutMenu();
	void DrawWindowsMenu();
	void DrawFileMenu();

	void DrawAddMenu();
	void DrawShapesMenu();

	void DrawStateMenu();
};


#endif // !__E_DOCK_WINDOW_W__

