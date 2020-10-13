
//The order is relevant from here...
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL/include/SDL.h"
#include "Glew/include/glew.h"
//...to here
#include "Globals.h"
#include "Application.h"
#include "M_Editor.h"

#include "M_Window.h"
#include "M_Renderer3D.h"
#include "M_Input.h"
#include "M_Console.h"


M_Editor::M_Editor(bool start_enabled) : Module(start_enabled),
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


M_Editor::~M_Editor()
{
}


bool M_Editor::Init()
{
	bool ret = true;


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiBackendFlags_RendererHasViewports;
																//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init("#version 130"); //TODO: this is hardcoded. Deal with it.

	App->console->AddLog("Log: ImGui initialized correctlly");

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

	return ret;
}


UPDATE_STATUS M_Editor::Update(float dt)
{

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	CreateDockingWindow();

	ImGui::Begin("Test", nullptr, ImGuiWindowFlags_NoBackground);
	ImGui::End();

	ImGui::Begin("Menu");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

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

	if (ImGui::CollapsingHeader("Application state"))
	{
		frameRateLog.push_back(ImGui::GetIO().Framerate);
		if (frameRateLog.size() > MAX_LOG_SIZE)
			frameRateLog.erase(frameRateLog.begin());

		ImGui::PlotHistogram("##framerate", &frameRateLog.front(), frameRateLog.size(), 0, "FrameRate", 0.0f, 100.0f, ImVec2(310, 100));
	}

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

	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Checkbox("Full screen", &winFullScreen);
		ImGui::SameLine();
		ImGui::Checkbox("Full screen desktop", &winFullScreenDesktop);
		ImGui::Checkbox("Resizable", &winResizable);
		ImGui::SameLine();
		ImGui::Checkbox("Borderless", &winBorderless);


		ImGui::SliderInt("Brightness", &brightness, 1, 100);
		ImGui::SliderInt("Width", &winWidth, 1, MAX_RESOLUTION_WIDTH);
		ImGui::SliderInt("Height", &winHeight, 1, MAX_RESOLUTION_HEIGHT);

		App->window->SetWindowFullScreen(winFullScreen);
		App->window->SetWindowFullScreenDesktop(winFullScreenDesktop);
		App->window->SetWindowResizable(winResizable);
		App->window->SetWindowBorderless(winBorderless);

		float bright = brightness * 0.01;
		App->window->SetWindowBrightness(bright);
		App->window->SetWindowMeasures(winWidth, winHeight);
	}

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

	ImGui::End();

	return UPDATE_STATUS::UPDATE_CONTINUE;
}


UPDATE_STATUS M_Editor::PostUpdate(float dt)
{
	//Draw();
	//SDL_GL_SwapWindow(App->window->window);

	return UPDATE_STATUS::UPDATE_CONTINUE;
}


bool M_Editor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}


void M_Editor::Draw()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}


void M_Editor::CreateDockingWindow()
{

	ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, windowFlags);
	ImGui::PopStyleVar();

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}

	ImGui::End();
}