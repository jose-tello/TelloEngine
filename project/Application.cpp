#include "Application.h"

Application::Application() :
	timer(),
	dt(0.0f),

	window(nullptr),
	input(nullptr),
	audio(nullptr),
	scene_intro(nullptr),
	renderer3D(nullptr),
	camera(nullptr)
	
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	int modulesNum = modulesVector.size();

	for (int i = modulesNum - 1; i >= 0; i--)
	{
		delete modulesVector[i];
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	int modulesNum = modulesVector.size();

	for(int i = 0; i < modulesNum && ret == true; i++)
	{
		ret = modulesVector[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");;

	for (int i = 0; i < modulesNum && ret == true; i++)
	{
		ret = modulesVector[i]->Start();
	}
	
	timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)timer.Read() / 1000.0f;
	timer.Start();
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
	
	int modulesNum = modulesVector.size();

	for (int i = 0; i < modulesNum && ret == true; i++)
	{
		ret = modulesVector[i]->PreUpdate(dt);
	}


	for (int i = 0; i < modulesNum && ret == true; i++)
	{
		ret = modulesVector[i]->Update(dt);
		
	}


	for (int i = 0; i < modulesNum && ret == true; i++)
	{
		ret = modulesVector[i]->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}


bool Application::CleanUp()
{
	bool ret = true;
	int modulesNum = modulesVector.size();

	for (int i = modulesNum - 1; i >= 0; i--)
	{
		ret = modulesVector[i]->CleanUp();
	}
	return ret;
}


void Application::AddModule(Module* mod)
{
	modulesVector.push_back(mod);
}