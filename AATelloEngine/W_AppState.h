#include "E_Window.h"

#include <vector>
#include <string>

#define MAX_LOG_SIZE 80
#define MAX_RESOLUTION_WIDTH 1920
#define MAX_RESOLUTION_HEIGHT 1080

class W_AppState : public E_Window
{
public:
	W_AppState(bool open = true);
	~W_AppState() override;

	bool Start() override;
	bool Update() override; 
	bool Draw() override;


private:
	void UpdateChApplicationState();
	void UpdateChInput();

	void DrawChApplicationState();
	void DrawChWindow();
	void DrawChRenderOptions();
	void DrawCameraOptions();
	void DrawChInput();
	void DrawChHardware();

private:

	//App State
	std::vector<float> frameRateLog;

	//Window
	bool winFullScreen;
	bool winFullScreenDesktop;
	bool winResizable;
	bool winBorderless;

	int  brightness;

	//Render
	bool depthTestEnabled;
	bool cullFaceEnabled;
	bool lightingEnabled;
	bool colorMatEnabled;
	bool texture2DEnabled;
	bool fillModeEnabled;
	bool wireframeModeEnabled;

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
