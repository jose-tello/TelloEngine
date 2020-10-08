#include "Application.h"

#include "M_Window.h"
#include "M_Input.h"
#include "M_Renderer3D.h"
#include "M_UI.h"
#include "M_Camera3D.h"
#include "M_Console.h"

Application::Application() : debug(false), renderPrimitives(true), dt(0.16f)
{
	window = new ModuleWindow();
	input = new ModuleInput();
	renderer3D = new ModuleRenderer3D();
	ui = new M_UI();
	camera = new ModuleCamera3D();
	console = new M_Console();

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(camera);
	
	// Renderer last!
	AddModule(console);
	
	AddModule(renderer3D);
	AddModule(ui);
}

Application::~Application()
{
	for (int i = modulesVec.size() - 1; i >= 0; i--)
	{
		delete modulesVec[i];
		modulesVec[i] = nullptr;
	}
}

bool Application::Init()
{
	bool ret = true;

	App = this;

	console->AddLog("Application Init --------------");

	// Call Init() in all modules
	int numModules = modulesVec.size();

	for (int i = 0; i < numModules && ret == true; i++)
	{
		ret = modulesVec[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	console->AddLog("Application Start --------------");

	for (int i = 0; i < numModules && ret == true; i++)
	{
		ret = modulesVec[i]->Start();
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
UPDATE_STATUS Application::Update()
{
	UPDATE_STATUS ret = UPDATE_STATUS::UPDATE_CONTINUE;
	PrepareUpdate();

	int numModules = modulesVec.size();

	for (int i = 0; i < numModules && ret == UPDATE_STATUS::UPDATE_CONTINUE; i++)
	{
		ret = modulesVec[i]->PreUpdate(dt);
	}

	for (int i = 0; i < numModules && ret == UPDATE_STATUS::UPDATE_CONTINUE; i++)
	{
		ret = modulesVec[i]->Update(dt);
	}

	for (int i = 0; i < numModules && ret == UPDATE_STATUS::UPDATE_CONTINUE; i++)
	{
		ret = modulesVec[i]->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	console->AddLog("Application clean up --------------");

	for (int i = modulesVec.size() - 1; i >= 0 && ret == true; i--)
	{
		ret = modulesVec[i]->CleanUp();
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	modulesVec.push_back(mod);
}

Application* App = nullptr;
