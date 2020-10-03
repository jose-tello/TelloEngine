#pragma once

#include <vector>
#include "Globals.h"
#include "p2Defs.h"
#include "Timer.h"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleUIManager;


class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleUIManager* uiManager;

private:

	Timer	ms_timer;
	float	dt;
	std::vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	bool Update();
	bool CleanUp();
	bool Reset();




private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};