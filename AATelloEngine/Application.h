#ifndef __APLICATION_H__
#define __APLICATION_H__

#include "Globals.h"
#include "Timer.h"

#include <vector>

class Module;
class M_Window;
class M_Input;
class M_Renderer3D;
class M_Editor;
class M_Console;
class M_Camera3D;
class M_AssetManager;

class Application
{
public:
	M_Window* window;
	M_Input* input;
	M_Renderer3D* renderer3D;
	M_Editor* editor;
	M_Camera3D* camera;
	M_AssetManager* assetManager;
	bool debug;
	bool renderPrimitives;

private:

	Timer	msTimer;
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

#endif // !__APLICATION_H__