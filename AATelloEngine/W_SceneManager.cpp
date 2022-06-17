#include "W_SceneManager.h"

#include "Application.h"
#include "M_Scene.h"
#include "M_FileManager.h"

#include "imgui/imgui.h"

W_SceneManager::W_SceneManager(bool open) : E_Window(E_WINDOW_TYPE::SCENE_MANAGER, open)
{
}


W_SceneManager::~W_SceneManager()
{
}


bool W_SceneManager::Draw()
{
	ImGui::Begin("Scene manager", &open);
	
	std::string name(App->scene->GetSceneName());
	name.resize(MAX_SCENE_NAME_LENGTH);

	ImGui::InputText("Name: ", &name[0], name.length());
	App->scene->SetSceneName(name.c_str());

	if (ImGui::Button("Save scene"))
	{
		App->scene->SaveScene();
	}

	if (ImGui::BeginMenu("LoadScene"))
	{
		GetSceneNames("/Library/Scenes/");

		int sceneCount = sceneNames.size();
		for (int i = 0; i < sceneCount; ++i)
		{
			if (ImGui::MenuItem(sceneNames[i].c_str()))
			{
				App->scene->LoadScene(sceneNames[i].c_str());
				break;
			}
		}

		sceneNames.clear();
		ImGui::EndMenu();
	}

	ImGui::End();

	return true;
}


void W_SceneManager::GetSceneNames(const char* directory)
{
	std::vector<std::string> files;
	std::vector<std::string> folderDirs;

	std::string dir(directory);

	App->fileManager->ExploreDirectory(dir.c_str(), files, folderDirs);

	int folderDirsCount = folderDirs.size();
	for (int i = 0; i < folderDirsCount; i++)
	{
		GetSceneNames((dir + folderDirs[i]).c_str());
	}

	int filesCount = files.size();
	for (int i = 0; i < filesCount; i++)
	{
		std::string str = files[i];
		std::string extension;
		std::string fileName;
		App->fileManager->SplitPath(files[i].c_str(), nullptr, &fileName, &extension);

		if (extension != "meta")
		{
			sceneNames.push_back(fileName);
		}
	}
}