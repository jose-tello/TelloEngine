#ifndef __APLICATION_H__
#define __APLICATION_H__

#include "Globals.h"
#include "TimeManager.h"

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
class M_Resources;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	UPDATE_STATUS Update();
	bool CleanUp();

	void CloseApplication();
	
	TimeManager* GetTimeManager();

private:
	void AddModule(Module* mod);
	void PrepareUpdate();

public:
	M_Window* window = nullptr;
	M_Input* input = nullptr;
	M_Renderer3D* renderer3D = nullptr;
	M_Editor* editor = nullptr;
	M_Camera3D* camera = nullptr;
	M_FileManager* fileManager = nullptr;
	M_Scene* scene = nullptr;
	M_Resources* resourceManager = nullptr;

private:
	TimeManager	timeManager;
	float	dt;
	std::vector<Module*> modulesVec;

	bool closeApplication;
};

#endif // !__APLICATION_H__