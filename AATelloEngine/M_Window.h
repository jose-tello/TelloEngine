#ifndef __M_WINDOW_H__
#define __M_WINDOW_H__

#include "Module.h"

class Application;

struct SDL_Window;
struct SDL_Surface;

class M_Window : public Module
{
public:

	M_Window(bool start_enabled = true);

	// Destructor
	virtual ~M_Window();

	bool Init() override;
	bool CleanUp() override;

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

#endif // __M_WINDOW_H__