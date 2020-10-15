#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"

class ModuleWindow;
class M_Input;
class M_Renderer3D;
class M_Editor;
class M_Console;
class ModuleCamera3D;

#include <vector>

class Application
{
public:
	ModuleWindow* window;
	M_Input* input;
	M_Renderer3D* renderer3D;
	M_Editor* editor;
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