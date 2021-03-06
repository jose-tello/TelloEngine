#include "Application.h"

#include "M_Input.h"
#include "M_Renderer3D.h"
#include "M_Editor.h"

#include "M_FileManager.h"

#include "imgui/imgui_impl_sdl.h"

M_Input::M_Input(bool startEnabled) : Module(startEnabled)
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
	App->editor->AddLog("Log: Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		App->editor->AddLog("[ERROR]: SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
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

	Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

	mouseX /= SCREEN_SIZE;
	mouseY /= SCREEN_SIZE;
	mouseZ = 0;

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

	xMouseMotion = yMouseMotion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch(e.type)
		{
		case (SDL_DROPFILE): { 
			char* droppedFileDir = e.drop.file;
			App->fileManager->LoadFromExporter(droppedFileDir);

			SDL_free(droppedFileDir);
			break;
		}
			case SDL_MOUSEWHEEL:
			mouseZ = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouseX = e.motion.x / SCREEN_SIZE;
			mouseY = e.motion.y / SCREEN_SIZE;

			xMouseMotion = e.motion.xrel / SCREEN_SIZE;
			yMouseMotion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if (e.window.event == SDL_WINDOWEVENT_RESIZED);
					
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_STATE::KEY_UP)
		return UPDATE_STATUS::UPDATE_STOP;

	return UPDATE_STATUS::UPDATE_CONTINUE;
}


// Called before quitting
bool M_Input::CleanUp()
{
	App->editor->AddLog("Log: Quitting SDL input event subsystem");
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
	return mouseX;
}

int M_Input::GetMouseY() const
{
	return mouseY;
}

int M_Input::GetMouseZ() const
{
	return mouseZ;
}

int M_Input::GetMouseXMotion() const
{
	return xMouseMotion;
}

int M_Input::GetMouseYMotion() const
{
	return yMouseMotion;
}

