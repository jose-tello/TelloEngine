#include "Application.h"

#include "Module.h"
#include "M_Window.h"
#include "M_Input.h"
#include "M_Renderer3D.h"
#include "M_Editor.h"
#include "M_Camera3D.h"
#include "M_FileManager.h"
#include "M_Scene.h"


Application* App = nullptr; //TODO: Dont know how and why this works, ask marc, should use an extern?


Application::Application() : 
	closeApplication(false),
	dt(0.16f)
{
	window = new M_Window();
	input = new M_Input();
	renderer3D = new M_Renderer3D();
	editor = new M_Editor();
	camera = new M_Camera3D();
	fileManager = new M_FileManager();
	scene = new M_Scene();

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(camera);
	AddModule(fileManager);

	AddModule(scene);

	// Renderer
	AddModule(renderer3D);
	AddModule(editor);
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

	editor->AddLog("Application Init --------------");

	// Call Init() in all modules
	int numModules = modulesVec.size();

	for (int i = 0; i < numModules && ret == true; i++)
	{
		ret = modulesVec[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	editor->AddLog("Application Start --------------");

	for (int i = 0; i < numModules && ret == true; i++)
	{
		ret = modulesVec[i]->Start();
	}

	return ret;
}


// ---------------------------------------------
void Application::PrepareUpdate()
{
	timeManager.Update();
	dt = timeManager.GetDt();
}


// Call PreUpdate, Update and PostUpdate on all modules
UPDATE_STATUS Application::Update()
{
	UPDATE_STATUS ret = UPDATE_STATUS::UPDATE_CONTINUE;

	if (closeApplication == true)
		return UPDATE_STATUS::UPDATE_STOP;

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

	return ret;
}


bool Application::CleanUp()
{
	bool ret = true;
	editor->AddLog("Application clean up --------------");

	for (int i = modulesVec.size() - 1; i >= 0 && ret == true; i--)
	{
		ret = modulesVec[i]->CleanUp();
	}
	return ret;
}


void Application::CloseApplication()
{
	editor->AddLog("Closing Application --------------");
	closeApplication = true;
}


TimeManager* Application::GetTimeManager()
{
	return &timeManager;
}


void Application::AddModule(Module* mod)
{
	modulesVec.push_back(mod);
}



