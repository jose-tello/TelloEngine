#include "E_AppState.h"

#include "Application.h"
#include "M_Input.h"
#include "M_Window.h"

#include "imgui/imgui.h"

E_AppState::E_AppState(bool open) :
	E_Window(open),

	//Window
	winFullScreen(false),
	winFullScreenDesktop(false),
	winResizable(false),
	winBorderless(true),

	brightness(100),
	winWidth(0),
	winHeight(0),

	//Cpu
	cpuCores(0),
	maxRamMemory(0),

	has3DNow(false),
	hasAVX(false),
	hasAVX2(false),
	hasAltiVec(false),
	hasMMX(false),
	hasRDTSC(false),
	hasSSE(false),
	hasSSE2(false),
	hasSSE3(false),
	hasSSE41(false),
	hasSSE42(false)
{

}


E_AppState::~E_AppState()
{
	frameRateLog.clear();
	inputsLog.clear();
}

bool E_AppState::Start()
{
	App->window->GetWindowMeasures(winWidth, winHeight);

	cpuCores = SDL_GetCPUCount();
	maxRamMemory = SDL_GetSystemRAM();

	has3DNow = SDL_Has3DNow();
	hasAVX = SDL_HasAVX();
	hasAVX2 = SDL_HasAVX2();
	hasAltiVec = SDL_HasAltiVec();
	hasMMX = SDL_HasMMX();
	hasRDTSC = SDL_HasRDTSC();
	hasSSE = SDL_HasSSE();
	hasSSE2 = SDL_HasSSE2();
	hasSSE3 = SDL_HasSSE3();
	hasSSE41 = SDL_HasSSE41();
	hasSSE42 = SDL_HasSSE42();

	return true;
}

bool E_AppState::Update()
{
	return true;
}


bool E_AppState::Draw()
{
	ImGui::Begin("Application state", &open);

	CreateBmHelp();
	CreateChApplicationState();
	CreateChInput();
	CreateChWindow();
	CreateChHardware();

	ImGui::End();

	return true;
}


void E_AppState::CreateBmHelp()
{
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Documentation"))
			ShellExecuteA(NULL, "open", "https://github.com/jose-tello/TelloEngine/wiki", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("Download latest version"))
			ShellExecuteA(NULL, "open", "https://github.com/jose-tello/TelloEngine/releases", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("Report a bug"))
			ShellExecuteA(NULL, "open", "https://github.com/jose-tello/TelloEngine/issues", NULL, NULL, SW_SHOWNORMAL);

		ImGui::End();
	}
}


void E_AppState::CreateChApplicationState()
{
	if (ImGui::CollapsingHeader("Application state"))
	{
		frameRateLog.push_back(ImGui::GetIO().Framerate);
		if (frameRateLog.size() > MAX_LOG_SIZE)
			frameRateLog.erase(frameRateLog.begin());

		ImGui::PlotHistogram("##framerate", &frameRateLog.front(), frameRateLog.size(), 0, "FrameRate", 0.0f, 100.0f, ImVec2(310, 100));
	}
}


void E_AppState::CreateChInput()
{
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse position: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i x,	%i y", App->input->GetMouseX(), App->input->GetMouseY());

		ImGui::Text("Mouse motion: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i x,	%i y", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());

		ImGui::Separator();
		ImGui::Text("Key and mouse inputs: \n");
		ImGui::NewLine();

		App->input->ReportKeyState(inputsLog);

		if (inputsLog.size() > MAX_LOG_SIZE / 4)
		{
			int elementsToErase = inputsLog.size() - MAX_LOG_SIZE / 4;
			for (int i = 0; i < elementsToErase; i++)
			{
				inputsLog.erase(inputsLog.begin());
			}
		}

		if (inputsLog.empty() == false)
		{
			int logSize = inputsLog.size();

			for (int i = 0; i < logSize; i++)
			{
				ImGui::Text(inputsLog[i].c_str());
			}
		}
	}
}


void E_AppState::CreateChWindow()
{
	if (ImGui::CollapsingHeader("Window"))
	{
		int width = winWidth;
		int height = winHeight;

		ImGui::Checkbox("Full screen", &winFullScreen);
		ImGui::SameLine();
		ImGui::Checkbox("Full screen desktop", &winFullScreenDesktop);
		ImGui::Checkbox("Resizable", &winResizable);
		ImGui::SameLine();
		ImGui::Checkbox("Borderless", &winBorderless);


		ImGui::SliderInt("Brightness", &brightness, 1, 100);
		ImGui::SliderInt("Width", &width, 100, MAX_RESOLUTION_WIDTH);
		ImGui::SliderInt("Height", &height, 100, MAX_RESOLUTION_HEIGHT);

		App->window->SetWindowFullScreen(winFullScreen);
		App->window->SetWindowFullScreenDesktop(winFullScreenDesktop);
		App->window->SetWindowResizable(winResizable);
		App->window->SetWindowBorderless(winBorderless);

		float bright = brightness * 0.01;
		App->window->SetWindowBrightness(bright);

		if (winWidth != width || winHeight != height)
		{
			winWidth = width;
			winHeight = height;
			App->window->SetWindowMeasures(winWidth, winHeight);
		}

	}
}


void E_AppState::CreateChHardware()
{
	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("CPU cache used: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i MB", SDL_GetCPUCacheLineSize());
		//ImGui::SameLine();
		//ImGui::TextColored(ImVec4(249, 215, 28, 1) 
		ImGui::Text("CPU cores: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", cpuCores);

		ImGui::Text("Caps: ");

		if (has3DNow == true)
			ImGui::BulletText("3DNow");

		if (hasAVX == true)
			ImGui::BulletText("AVX");

		if (hasAVX2 == true)
			ImGui::BulletText("AVX2");

		if (hasAltiVec == true)
			ImGui::BulletText("AltiVec");

		if (hasMMX == true)
			ImGui::BulletText("MMX");

		if (hasRDTSC == true)
			ImGui::BulletText("RDTSC");

		if (hasSSE == true)
			ImGui::BulletText("SSE");

		if (hasSSE2 == true)
			ImGui::BulletText("SSE2");

		if (hasSSE3 == true)
			ImGui::BulletText("SSE3");

		if (hasSSE41 == true)
			ImGui::BulletText("SSE41");

		if (hasSSE42 == true)
			ImGui::BulletText("SSE42");
	}
}