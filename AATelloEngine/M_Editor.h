#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include <vector>

#define MAX_LOG_SIZE 80
#define MAX_RESOLUTION_WIDTH 1920
#define MAX_RESOLUTION_HEIGHT 1080

class E_Window;

class M_Editor : public Module
{
public:
	M_Editor(bool start_enabled = true);
	~M_Editor();

	bool Init();
	bool Start();
	
	UPDATE_STATUS Update(float dt);

	bool CleanUp();

	void Draw();

private:
	void CreateDockingWindow();

private:
	std::vector<E_Window*> windowsVec;
}; 
