#ifndef __E_DOCK_WINDOW_H__
#define __E_DOCK_WINDOW_H__

#include "E_Window.h"

class E_DockWindow : public E_Window
{
public:
	E_DockWindow(bool open = true);
	~E_DockWindow();

	bool Draw() override;
private:
	void CreateDockWindow();
};


#endif // !__E_DOCK_WINDOW_W__

