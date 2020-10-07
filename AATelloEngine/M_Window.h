#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	int Width() const;
	int Height() const;

	void SetTitle(const char* title);
	void SetWindowFullScreen(bool fullScreen);
	void SetWindowFullScreenDesktop(bool fullScreenDesktop);
	void SetWindowBrightness(float brightness);
	void SetWindowResizable(bool resizable);
	void SetWindowBorderless(bool borderless);

	void GetWindowMeasures(int& width, int& height) const;
	void SetWindowMeasures(int width, int height);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;

private:
	bool fullScreen;
	bool fullScreenDesktop;
};

#endif // __ModuleWindow_H__