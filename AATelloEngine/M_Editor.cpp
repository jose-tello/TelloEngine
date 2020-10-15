
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

#include "E_DockWindow.h"
#include "E_AppState.h"
#include "E_Console.h"
#include "E_Scene.h"


M_Editor::M_Editor(bool start_enabled) : Module(start_enabled)
{
	E_Window* win;

	win = new E_DockWindow(true);
	windowsVec.push_back(win);

	win = new E_AppState(true);
	windowsVec.push_back(win);

	win = new E_Console(true);
	windowsVec.push_back(win);

	win = new E_Scene(true);
	windowsVec.push_back(win);
}


M_Editor::~M_Editor()
{
	int size = windowsVec.size();

	for (int i = 0; i < size; i++)
	{
		delete windowsVec[i];
		windowsVec[i] = nullptr;
	}

	windowsVec.clear();
}


bool M_Editor::Init()
{
	bool ret = true;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init("#version 130");

	App->editor->AddLog("Log: ImGui initialized correctlly");


	return ret;
}


bool M_Editor::Start()
{
	for (int i = 0; i < (int)E_WINDOW_TYPE::MAX; i++)
		windowsVec[i]->Start();

	return true;
}


UPDATE_STATUS M_Editor::Update(float dt)
{
	// Start the Dear ImGui frame

	for (int i = 0; i < (int)E_WINDOW_TYPE::MAX; i++)
		windowsVec[i]->Update();
	
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
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	CreateDockingWindow();

	for (int i = 0; i < (int)E_WINDOW_TYPE::MAX; i++)
	{
		if (windowsVec[i]->open == false)
			continue;

		windowsVec[i]->Draw();
	}

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


void M_Editor::AddLog(const char* fmt, ...) IM_FMTARGS(2)
{
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	
	E_Console* console = (E_Console*)windowsVec[(int)E_WINDOW_TYPE::CONSOLE];
	console->AddLog(buf);
}


void M_Editor::OpenWindow(E_WINDOW_TYPE type)
{
	windowsVec[(int)type]->open = true;
}


void M_Editor::CreateDockingWindow()
{
	
}
