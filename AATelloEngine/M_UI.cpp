
//The order is relevant from here...
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL/include/SDL.h"
#include "Glew/include/glew.h"
//...to here
#include "Globals.h"
#include "Application.h"
#include "M_UI.h"

#include "M_Window.h"
#include "M_Renderer3D.h"
#include "M_Console.h"


M_UI::M_UI(bool start_enabled) : Module(start_enabled),
	winFullScreen(false),
	winFullScreenDesktop(false),
	winResizable(false),
	winBorderless(true),

	brightness(100),
	winWidth(0),
	winHeight(0)
{
}


M_UI::~M_UI()
{
}


bool M_UI::Init()
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

	return ret;
}


UPDATE_STATUS M_UI::PreUpdate(float dt)
{

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();


	ImGui::Begin("Menu");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::BeginMenu("Help")) 
	{
		if (ImGui::MenuItem("Documentation"))
			ShellExecuteA(NULL, "open", "https://github.com/jose-tello/TelloEngine/wiki", NULL, NULL, SW_SHOWNORMAL);

		if(ImGui::MenuItem("Download latest version"))
			ShellExecuteA(NULL, "open", "https://github.com/jose-tello/TelloEngine/releases", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("Report a bug"))
			ShellExecuteA(NULL, "open", "https://github.com/jose-tello/TelloEngine/issues", NULL, NULL, SW_SHOWNORMAL);

		ImGui::End();
	}

	if (ImGui::CollapsingHeader("Application state"))
	{
		char text[25];

		frameRateLog.push_back(ImGui::GetIO().Framerate);
		if (frameRateLog.size() > MAX_LOG_SIZE)
			frameRateLog.erase(frameRateLog.begin());
		
		ImGui::PlotHistogram("##framerate", &frameRateLog.front(), frameRateLog.size(), 0, "FrameRate", 0.0f, 100.0f, ImVec2(310, 100));
	
		sprintf(text, "CPU cache used: %i MB", SDL_GetCPUCacheLineSize());
		ImGui::Text(text);
		
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

	}
	
	ImGui::End();


	return UPDATE_STATUS::UPDATE_CONTINUE;
}


UPDATE_STATUS M_UI::PostUpdate(float dt)
{
	// Rendering
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

	return UPDATE_STATUS::UPDATE_CONTINUE;
}


bool M_UI::CleanUp()
{
	bool ret = true;

	//TODO: Void functions, no return, no check possible. FIX!
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();


	return ret;
}

