#include "Globals.h"
#include "Application.h"
#include "M_Input.h"

#include "M_Renderer3D.h"

#include "imgui/imgui_impl_sdl.h"

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	memset(keyboard, (int)KEY_STATE::KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, (int)KEY_STATE::KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
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
UPDATE_STATUS ModuleInput::PreUpdate(float dt)
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
			if(mouse_buttons[i] == KEY_STATE::KEY_IDLE)
				mouse_buttons[i] = KEY_STATE::KEY_DOWN;
			else
				mouse_buttons[i] = KEY_STATE::KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_STATE::KEY_REPEAT || mouse_buttons[i] == KEY_STATE::KEY_DOWN)
				mouse_buttons[i] = KEY_STATE::KEY_UP;
			else
				mouse_buttons[i] = KEY_STATE::KEY_IDLE;
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

UPDATE_STATUS ModuleInput::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_STATE::KEY_DOWN)
		App->debug = !App->debug;

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_STATE::KEY_DOWN)
		App->renderPrimitives = !App->renderPrimitives;

	return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}