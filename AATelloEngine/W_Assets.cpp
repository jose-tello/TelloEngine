#include "W_Assets.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Editor.h"
#include "M_Resources.h"
#include "M_Input.h"

#include "imgui/imgui.h"

#include <vector>
#include <string>

W_Assets::W_Assets(bool open) : E_Window(E_WINDOW_TYPE::LOAD_FILE, open)
{
}


W_Assets::~W_Assets()
{
}


bool W_Assets::Draw()
{
	ImGui::Begin(("Assets"), &open);
	
	if (ImGui::Button("Update state"))
		App->resourceManager->UpdateAllAssets();

	DrawLoadWindow();
	ImGui::End();
	
	return true;
}


void W_Assets::DrawLoadWindow()
{
	ImGui::BeginChild("File bowser");

	DrawDirectory("/Assets/", "");

	/*ImGui::Separator();

	if (ImGui::CollapsingHeader("Materials"))
		DrawDirectory(MATERIAL_LIBRARY, "", true);

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Meshes"))
		DrawDirectory(MESH_LIBRARY, "", true);

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Shaders"))
		DrawDirectory(SHADER_LIBRARY, "", true);*/

	ImGui::EndChild();
}


void W_Assets::DrawDirectory(const char* directory, const char* filterExtension, bool fromResource)
{
	std::vector<std::string> files;
	std::vector<std::string> folderDirs;

	std::string dir(directory);

	App->fileManager->ExploreDirectory(dir.c_str(), files, folderDirs);

	int folderDirsCount = folderDirs.size();
	for (int i = 0; i < folderDirsCount; i++)
	{
		if (ImGui::TreeNodeEx((dir + folderDirs[i]).c_str(), 0, "%s", folderDirs[i].c_str()))
		{
			DrawDirectory((dir + folderDirs[i]).c_str(), filterExtension, fromResource);
			ImGui::TreePop();
		}
	}

	int filesCount = files.size();
	for (int i = 0; i < filesCount; i++)
	{
		std::string str = files[i];
		std::string extension;
		App->fileManager->SplitPath(files[i].c_str(), nullptr, nullptr, &extension);

		if (extension != "meta")
		{
			if (ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_Leaf))
			{
				if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
				{
					if (fromResource == true)
					{
						int id = std::stoi(files[i]);
						App->resourceManager->WindowLoad(id, App->editor->GetFocusedGameObject());
					}
					else
						App->resourceManager->DragAndDropImport((dir += files[i]).c_str(), App->editor->GetFocusedGameObject());
				}

				if (fromResource == false)
				{
					if (ImGui::IsItemHovered() && App->input->GetKey(BACKSPACE) == KEY_STATE::KEY_DOWN)
						App->fileManager->RemoveFile((dir += files[i]).c_str());
				}

				ImGui::TreePop();
			}
		}
	}
}