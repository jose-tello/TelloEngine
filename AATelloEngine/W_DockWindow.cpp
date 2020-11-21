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
		if (ImGui::BeginMenu("Menu"))
		{
			DrawAboutMenu();

			DrawWindowsMenu();
			

			if (ImGui::MenuItem("Exit"))
				App->CloseApplication();

			ImGui::EndMenu();
		}

		DrawFileMenu();
		DrawAddMenu();

		ImGui::EndMenuBar();
	}
	ImGui::End();
}


void W_DockWindow::DrawAboutMenu()
{
	if (ImGui::BeginMenu("About"))
	{
		if (ImGui::MenuItem("Documentation"))
			ShellExecuteA(NULL, "open", "https://github.com/jose-tello/TelloEngine/wiki", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("Download latest version"))
			ShellExecuteA(NULL, "open", "https://github.com/jose-tello/TelloEngine/releases", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("Report a bug"))
			ShellExecuteA(NULL, "open", "https://github.com/jose-tello/TelloEngine/issues", NULL, NULL, SW_SHOWNORMAL);

		ImGui::EndMenu();
	}
}


void W_DockWindow::DrawWindowsMenu()
{
	if (ImGui::BeginMenu("Windows"))
	{
		ImGui::Checkbox("App state", &App->editor->GetWindow(E_WINDOW_TYPE::APPLICATION_STATE)->open);
		ImGui::Checkbox("Console", &App->editor->GetWindow(E_WINDOW_TYPE::CONSOLE)->open);
		ImGui::Checkbox("Inspector", &App->editor->GetWindow(E_WINDOW_TYPE::INSPECTOR)->open);
		ImGui::Checkbox("Object hierarchy", &App->editor->GetWindow(E_WINDOW_TYPE::GAME_OBJECTS)->open);
		ImGui::Checkbox("Scene", &App->editor->GetWindow(E_WINDOW_TYPE::SCENE)->open);

		ImGui::EndMenu();
	}
}


void W_DockWindow::DrawFileMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save"))
			App->scene->SaveScene();

		if (ImGui::MenuItem("Load"))
			App->editor->OpenWindow(E_WINDOW_TYPE::LOAD_FILE);

		if (ImGui::MenuItem("Load Scene"))
			App->scene->LoadScene();
			

			ImGui::EndMenu();
	}
}


void W_DockWindow::DrawAddMenu()
{
	if (ImGui::BeginMenu("Add"))
	{
		if (ImGui::Button("Empty"))
			App->scene->AddEmpty();

		if (ImGui::Button("Camera"))
			App->scene->AddCamera();
		
		DrawShapesMenu();
		ImGui::EndMenu();
	}
}


void W_DockWindow::DrawShapesMenu()
{
	if (ImGui::BeginMenu("Add shape"))
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
}