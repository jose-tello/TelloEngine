#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL/include/SDL.h"
#include "Glew/include/glew.h"

#include "Globals.h"
#include "Application.h"
#include "M_Editor.h"

#include "M_Window.h"
#include "M_Renderer3D.h"

#include "W_DockWindow.h"
#include "W_AppState.h"
#include "W_Console.h"
#include "W_CameraView.h"
#include "W_ObjectHierarchy.h"
#include "W_Inspector.h"
#include "W_Assets.h"
#include "W_Scene.h"
#include "W_ShaderEditor.h"


M_Editor::M_Editor(bool startEnabled) : Module(startEnabled)
{
	E_Window* win;

	win = new W_DockWindow(true);
	windowsVec.push_back(win);

	win = new W_AppState(true);
	windowsVec.push_back(win);

	win = new W_Console(true);
	windowsVec.push_back(win);

	win = new W_ObjectHierarchy(true);
	windowsVec.push_back(win);

	win = new W_Inspector(true);
	windowsVec.push_back(win);

	win = new W_Assets(true);
	windowsVec.push_back(win);

	win = new W_ShaderEditor();
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
	ImGui_ImplOpenGL3_Init("#version 330");

	App->editor->AddLog("Log: ImGui initialized correctlly");


	return ret;
}


bool M_Editor::Start()
{
	int numWindows = windowsVec.size();
	for (int i = 0; i < numWindows; i++)
		windowsVec[i]->Start();

	return true;
}


UPDATE_STATUS M_Editor::Update(float dt)
{
	int numWindows = windowsVec.size();
	for (int i = 0; i < numWindows; i++)
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

	int numWindows = windowsVec.size();
	for (int i = 0; i < numWindows; i++)
	{
		if (windowsVec[i]->open == true)
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


void M_Editor::AddLog(const char* fmt, ...)
{
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	
	W_Console* console = (W_Console*)windowsVec[(int)E_WINDOW_TYPE::CONSOLE];
	console->AddLog(buf);
}


void M_Editor::OpenShaderEditor(int shaderId, const char* shaderName) const
{
	W_ShaderEditor* shaderEditor = (W_ShaderEditor*)windowsVec[(int)E_WINDOW_TYPE::EDIT_SHADER];
	shaderEditor->OpenShaderCode(shaderId, shaderName);
}


void M_Editor::OpenWindow(int open)
{
	windowsVec[open]->open = true;
}


void M_Editor::DeleteWindow(E_Window* toDelete)
{
	int numWindows = windowsVec.size();
	for (int i = 0; i < numWindows; i++)
	{
		if (windowsVec[i] == toDelete)
		{
			delete windowsVec[i];
			windowsVec.erase(windowsVec.begin() + i);
			break;
		}
	}
}


E_Window* M_Editor::AddSceneWindow(C_Camera* camera)
{
	W_Scene* sceneWin = new W_Scene(true, camera);
	windowsVec.push_back(sceneWin);

	return sceneWin;
}


E_Window* M_Editor::AddCameraWindow(C_Camera* camera)
{
	W_CameraView* sceneWin = new W_CameraView(true, camera);
	windowsVec.push_back(sceneWin);

	return sceneWin;
}


std::vector<E_Window*> M_Editor::GetWindowsVector()
{
	return windowsVec;
}


E_Window* M_Editor::GetWindow(int win)
{
	return windowsVec[win];
}


void M_Editor::GetCameraWindowSize(E_Window* win, int& x, int& y, float& mouseX, float& mouseY)
{
	if (win->type == E_WINDOW_TYPE::SCENE_CAMERA)
	{
		W_CameraView* sceneWin = (W_CameraView*)win;
		sceneWin->GetWindowMeasures(x, y);
		sceneWin->GetMousePosInWindow(mouseX, mouseY);
	}
	else
		assert("Invalid window type");
	
}


bool M_Editor::IsWindowHovered(E_WINDOW_TYPE win) const
{
	return windowsVec[(int)win]->IsHovered();
}


bool M_Editor::IsWindowFocused(E_WINDOW_TYPE win) const
{
	return windowsVec[(int)win]->IsFocused();
}


GameObject* M_Editor::GetFocusedGameObject() const
{
	W_Inspector* inspector = (W_Inspector *)windowsVec[(int)E_WINDOW_TYPE::INSPECTOR];
	return inspector->GetFocusedGameObject();
}


void M_Editor::SetFocusedGameObject(GameObject* gameObject)
{
	W_Inspector* inspector = (W_Inspector*)windowsVec[(int)E_WINDOW_TYPE::INSPECTOR];
	inspector->SetFocusedObject(gameObject);
}


bool M_Editor::GetFocusedGameObjectPos(float& x, float& y, float& z) const
{
	W_Inspector* inspector = (W_Inspector*)windowsVec[(int)E_WINDOW_TYPE::INSPECTOR];
	return inspector->GetFocusedGameObjectPos(x, y, z);
}


void M_Editor::QuitFocusedGameObject()
{
	W_Inspector* inspector = (W_Inspector*)windowsVec[(int)E_WINDOW_TYPE::INSPECTOR];
	inspector->QuitFocusedObject();
}


void M_Editor::DeleteFocusedObject()
{
	W_Inspector* inspector = (W_Inspector*)windowsVec[(int)E_WINDOW_TYPE::INSPECTOR];
	inspector->DeleteFocusedObject();
}