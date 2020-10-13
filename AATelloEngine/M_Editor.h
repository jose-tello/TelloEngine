#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include <vector>
#include <string>

#define MAX_LOG_SIZE 80
#define MAX_RESOLUTION_WIDTH 1920
#define MAX_RESOLUTION_HEIGHT 1080

class M_Editor : public Module
{
public:
	M_Editor(bool start_enabled = true);
	~M_Editor();

	bool Init();
	UPDATE_STATUS Update(float dt);
	UPDATE_STATUS PostUpdate(float dt);

	bool CleanUp();

	void Draw();

private:
	void CreateDockingWindow();

public:

	//App State
	std::vector<float> frameRateLog;
	
	//Window
	bool winFullScreen;
	bool winFullScreenDesktop;
	bool winResizable;
	bool winBorderless;
	int  brightness;

	int winWidth;
	int winHeight;

	//Hardware
	int cpuCores;

	int maxRamMemory;
	bool has3DNow;
	bool hasAVX;
	bool hasAVX2;
	bool hasAltiVec;
	bool hasMMX;
	bool hasRDTSC;
	bool hasSSE;
	bool hasSSE2;
	bool hasSSE3;
	bool hasSSE41;
	bool hasSSE42;

	//Inputs
	std::vector<std::string> inputsLog;
}; 
