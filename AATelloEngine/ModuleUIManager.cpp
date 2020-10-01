#include "Globals.h"
#include "Application.h"
#include "ModuleUIManager.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"

#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"


#pragma comment (lib, "Glew/libs/glew/glew32.lib") /* link Microsoft OpenGL lib   */
ModuleUIManager::ModuleUIManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleUIManager::~ModuleUIManager()
{}

// Called before render is available
bool ModuleUIManager::Init()
{

    
	
	return true;
}


bool ModuleUIManager::Start()
{
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
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->GetContext());
    ImGui_ImplOpenGL3_Init("#version 130");

    return true;
}


bool ModuleUIManager::PreUpdate(float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

    
    
    //Ui creation
    ImGui::Begin("Hello Imgui");
    
    ImGui::Text("It seems it works.");
    ImGui::Text("Click to close me");
    ImGui::SameLine();
    if (ImGui::Button("Button"))
        return false;

    ImGui::End();

    ImGui::ShowDemoWindow();

	return true;
}

bool ModuleUIManager::PostUpdate(float dt)
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
    return true;
}

// Called before quitting
bool ModuleUIManager::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
	return true;
}


	