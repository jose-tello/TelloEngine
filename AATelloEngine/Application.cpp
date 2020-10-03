#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleUIManager.h"


Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this, true);
	uiManager = new ModuleUIManager(this, true);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);

	// Scenes
	AddModule(camera);
	AddModule(uiManager);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	int numModules = list_modules.size();

	for (int i = 0; i < numModules; i++)
	{
		ret = list_modules[i]->Init();

	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	for (int i = 0; i < numModules; i++)
	{
		if (list_modules[i]->IsEnabled())
		{
			ret = list_modules[i]->Start();
		}
	}

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
bool Application::Update()
{
	bool ret = true;
	PrepareUpdate();

	//PreUpdate
	int numModules = list_modules.size();

	for (int i = 0; i < numModules && ret == true; i++)
	{
		ret = list_modules[i]->PreUpdate(dt);
	}

	//Update
	for (int i = 0; i < numModules && ret == true; i++)
	{
		ret = list_modules[i]->Update(dt);
	}

	//PostUpdate
	for (int i = 0; i < numModules && ret == true; i++)
	{
		ret = list_modules[i]->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	int numModules = list_modules.size();

	for (int i = numModules - 1; i >= 0 && ret == true; i--)
	{
		ret = list_modules[i]->CleanUp();
	}
	return ret;
}

bool Application::Reset()
{
	bool ret = true;
	int numModules = list_modules.size();


	for (int i = 0; i < numModules && ret == true; i++)
	{
		ret = list_modules[i]->Reset();
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}
