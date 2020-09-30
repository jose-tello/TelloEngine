#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled), mouseMotionX(0), mouseMotionY(0), mouseX(0), mouseY(0), 
																 mouseXRaw(0), mouseYRaw(0), mouseWheelMotionX(0), mouseWheelMotionY(0)
{
	memset(keyboard, (int)KEY_STATE::KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouseButtons, (int)KEY_STATE::KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
bool ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	mouseMotionX = 0;
	mouseMotionY = 0;
	mouseWheelMotionX = 0;
	mouseWheelMotionY = 0;

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_STATE::KEY_IDLE)
			{
				keyboard[i] = KEY_STATE::KEY_DOWN;
			}
			else
			{
				keyboard[i] = KEY_STATE::KEY_REPEAT;
			}
		}

		else
		{
			if (keyboard[i] == KEY_STATE::KEY_REPEAT || keyboard[i] == KEY_STATE::KEY_DOWN)
			{
				keyboard[i] = KEY_STATE::KEY_UP;
			}
			else
			{
				keyboard[i] = KEY_STATE::KEY_IDLE;
			}
		}
	}


	for (int i = 0; i < MAX_MOUSE_BUTTONS; ++i)
	{
		if (mouseButtons[i] == KEY_STATE::KEY_DOWN)
		{
			mouseButtons[i] = KEY_STATE::KEY_REPEAT;
		}

		if (mouseButtons[i] == KEY_STATE::KEY_UP)
		{
			mouseButtons[i] = KEY_STATE::KEY_IDLE;
		}
	}

	static SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{

		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[(int)EVENT_WINDOW::WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:

			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[(int)EVENT_WINDOW::WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[(int)EVENT_WINDOW::WE_SHOW] = true;
				break;
			}

			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseButtons[event.button.button - 1] = KEY_STATE::KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouseButtons[event.button.button - 1] = KEY_STATE::KEY_UP;
			break;

		case SDL_MOUSEWHEEL:

			mouseWheelMotionX = event.wheel.x;
			mouseWheelMotionY = event.wheel.y;
			break;

		case SDL_MOUSEMOTION:
			mouseMotionX = event.motion.xrel;
			mouseMotionY = event.motion.yrel;

			mouseXRaw = event.motion.x;
			mouseYRaw = event.motion.y;

			break;


		}


	}

	if (GetKey(SDL_SCANCODE_ESCAPE) == KEY_STATE::KEY_DOWN)
	{
		return false;
	}

	return true;
}


// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}


KEY_STATE ModuleInput::GetKey(int id) const
{
	return keyboard[id];
}

KEY_STATE ModuleInput::GetMouseButton(int id) const
{
	return mouseButtons[id - 1];
}


iMPoint ModuleInput::GetMousePosScreen() const
{
	iMPoint ret;
	SDL_GetMouseState(&ret.x, &ret.y);
	return ret;
}


void ModuleInput::GetMouseMotion(int& x, int& y)
{
	x = -mouseMotionX;
	y = -mouseMotionY;
}


void ModuleInput::GetScrollWheelMotion(int& x, int& y)
{
	x = mouseWheelMotionX;
	y = mouseWheelMotionY;
}