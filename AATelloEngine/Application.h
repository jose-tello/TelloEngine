#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"

class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class M_UI;
class M_Console;
class ModuleCamera3D;

#include <vector>

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	M_UI* ui;
	M_Console* console;
	ModuleCamera3D* camera;
	bool debug;
	bool renderPrimitives;
private:

	Timer	ms_timer;
	float	dt;
	std::vector<Module*> modulesVec;

public:

	Application();
	~Application();

	bool Init();
	UPDATE_STATUS Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};