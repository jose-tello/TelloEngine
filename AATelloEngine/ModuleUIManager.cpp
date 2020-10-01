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

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
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


	