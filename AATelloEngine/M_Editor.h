#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include "imgui/imgui.h"
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
	void Draw();

	bool CleanUp();
	
	void AddLog(const char* fmt, ...) IM_FMTARGS(2);

private:
	void CreateDockingWindow();

private:
	std::vector<E_Window*> windowsVec;
}; 
