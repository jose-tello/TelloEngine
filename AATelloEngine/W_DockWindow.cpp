#include "W_DockWindow.h"

#include "Application.h"
#include "M_Scene.h"
#include "M_Editor.h"

#include "imgui/imgui.h"

W_DockWindow::W_DockWindow(bool open) : E_Window(open)
{
}


W_DockWindow::~W_DockWindow()
{
}


bool W_DockWindow::Draw()
{
	CreateDockWindow();
	return true;
}


void W_DockWindow::CreateDockWindow()
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

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("About"))
		{
			//TODO: put some cool text here

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Load shapes"))
		{
			if (ImGui::Button("Cube"))
				App->scene->AddPrimitive(PRIMITIVE_TYPE::CUBE);

			if (ImGui::Button("Piramid"))
				App->scene->AddPrimitive(PRIMITIVE_TYPE::PIRAMID);

			if (ImGui::Button("Sphere"))
				App->scene->AddPrimitive(PRIMITIVE_TYPE::SPHERE);

			if (ImGui::Button("Cilinder"))
				App->scene->AddPrimitive(PRIMITIVE_TYPE::CILINDER);
			
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::Checkbox("App state", &App->editor->GetWindow(E_WINDOW_TYPE::APPLICATION_STATE)->open);
			ImGui::Checkbox("Console", &App->editor->GetWindow(E_WINDOW_TYPE::CONSOLE)->open);
			ImGui::Checkbox("Inspector", &App->editor->GetWindow(E_WINDOW_TYPE::INSPECTOR)->open);
			ImGui::Checkbox("Object hierarchy", &App->editor->GetWindow(E_WINDOW_TYPE::GAME_OBJECTS)->open);
			ImGui::Checkbox("Scene", &App->editor->GetWindow(E_WINDOW_TYPE::SCENE)->open);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();
}