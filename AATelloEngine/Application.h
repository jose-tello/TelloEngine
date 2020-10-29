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
class M_FileManager;
class M_Scene;

class Application
{
public:
	M_Window* window = nullptr;
	M_Input* input = nullptr;
	M_Renderer3D* renderer3D = nullptr;
	M_Editor* editor = nullptr;
	M_Camera3D* camera = nullptr;
	M_FileManager* fileManager = nullptr;
	M_Scene* scene = nullptr;

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