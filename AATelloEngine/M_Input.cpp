#include "Globals.h"
#include "Application.h"
#include "M_Input.h"

#include "M_Renderer3D.h"
#include "M_Console.h"

#include "imgui/imgui_impl_sdl.h"

M_Input::M_Input(bool start_enabled) : Module(start_enabled)
{
	memset(keyboard, (int)KEY_STATE::KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouseButtons, (int)KEY_STATE::KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
M_Input::~M_Input()
{
}

// Called before render is available
bool M_Input::Init()
{
	App->console->AddLog("Log: Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		App->console->AddLog("ERROR: SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
UPDATE_STATUS M_Input::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_STATE::KEY_IDLE)
				keyboard[i] = KEY_STATE::KEY_DOWN;
			else
				keyboard[i] = KEY_STATE::KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_STATE::KEY_REPEAT || keyboard[i] == KEY_STATE::KEY_DOWN)
				keyboard[i] = KEY_STATE::KEY_UP;
			else
				keyboard[i] = KEY_STATE::KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < MAX_MOUSE_BUTTONS; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouseButtons[i] == KEY_STATE::KEY_IDLE)
				mouseButtons[i] = KEY_STATE::KEY_DOWN;
			else
				mouseButtons[i] = KEY_STATE::KEY_REPEAT;
		}
		else
		{
			if(mouseButtons[i] == KEY_STATE::KEY_REPEAT || mouseButtons[i] == KEY_STATE::KEY_DOWN)
				mouseButtons[i] = KEY_STATE::KEY_UP;
			else
				mouseButtons[i] = KEY_STATE::KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_STATE::KEY_UP)
		return UPDATE_STATUS::UPDATE_STOP;

	return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS M_Input::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_STATE::KEY_DOWN)
		App->debug = !App->debug;

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_STATE::KEY_DOWN)
		App->renderPrimitives = !App->renderPrimitives;

	return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Called before quitting
bool M_Input::CleanUp()
{
	App->console->AddLog("Log: Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}


KEY_STATE M_Input::GetKey(int id) const
{
	return keyboard[id];
}

KEY_STATE M_Input::GetMouseButton(int id) const
{
	return mouseButtons[id];
}

void M_Input::ReportKeyState(std::vector<std::string>& inputsLog) const
{
	char keyText[20];

	for (int i = 0; i < MAX_KEYS; i++)
	{
		if (keyboard[i] == KEY_STATE::KEY_DOWN)
		{
			sprintf(keyText, "%i KEY_DOWN", i);
			inputsLog.push_back(keyText);
		}

		else if (keyboard[i] == KEY_STATE::KEY_UP)
		{
			sprintf(keyText, "%i KEY_UP", i);
			inputsLog.push_back(keyText);
		}
	}

	for (int i = 0; i < MAX_MOUSE_BUTTONS; i++)
	{
		if (mouseButtons[i] == KEY_STATE::KEY_DOWN)
		{
			sprintf(keyText, "%i MOUSE BUTTON_DOWN", i);
			inputsLog.push_back(keyText);
		}

		if (mouseButtons[i] == KEY_STATE::KEY_UP)
		{
			sprintf(keyText, "%i MOUSE BUTTON_UP", i);
			inputsLog.push_back(keyText);
		}
	}
}

int M_Input::GetMouseX() const
{
	return mouse_x;
}

int M_Input::GetMouseY() const
{
	return mouse_y;
}

int M_Input::GetMouseZ() const
{
	return mouse_z;
}

int M_Input::GetMouseXMotion() const
{
	return mouse_x_motion;
}

int M_Input::GetMouseYMotion() const
{
	return mouse_y_motion;
}

