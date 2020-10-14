#include "Globals.h"
#include "Application.h"
#include "M_Window.h"
#include "M_Editor.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled),
	window(nullptr),
	screenSurface(nullptr),

	fullScreen(false),
	fullScreenDesktop(false)
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	App->editor->AddLog("Log: Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		App->editor->AddLog("ERROR: SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		if (WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			App->editor->AddLog("ERROR: Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	App->editor->AddLog("Log: Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

int ModuleWindow::Width() const
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	return w;
}

int ModuleWindow::Height() const
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	return h;
}


void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}


void ModuleWindow::SetWindowFullScreen(bool fullScreen)
{
	if (fullScreen != this->fullScreen)
	{
		this->fullScreen = fullScreen;

		if (fullScreen == true)
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

		else
		{
			if (fullScreenDesktop == true)
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

			else
				SDL_SetWindowFullscreen(window, 0);
		}
	}
}


void ModuleWindow::SetWindowFullScreenDesktop(bool fullScreenDesktop)
{
	if (fullScreenDesktop != this->fullScreenDesktop)
	{
		this->fullScreenDesktop = fullScreenDesktop;

		if (fullScreenDesktop == true)
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

		else
		{
			if (fullScreen == true)
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

			else
				SDL_SetWindowFullscreen(window, 0);
		}
	}
}


void ModuleWindow::SetWindowBrightness(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
}


void ModuleWindow::SetWindowResizable(bool resizable)
{
	if (resizable == true)
		SDL_SetWindowResizable(window, SDL_TRUE);

	else
		SDL_SetWindowResizable(window, SDL_FALSE);
}


void ModuleWindow::SetWindowBorderless(bool borderless)
{
	if (borderless == true)
		SDL_SetWindowBordered(window, SDL_TRUE);

	else
		SDL_SetWindowBordered(window, SDL_FALSE);
}


void ModuleWindow::GetWindowMeasures(int& width, int& height) const
{
	SDL_GetWindowSize(window, &width, &height);
}


void ModuleWindow::SetWindowMeasures(int width, int height)
{
	SDL_SetWindowSize(window, width, height);
}