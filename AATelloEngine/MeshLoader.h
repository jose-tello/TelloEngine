#ifndef __MESH_LOADER_H__
#define __MESH_LOADER_H__

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Application.h"
#include "Mesh.h"
#include "M_Editor.h"


namespace MeshLoader
{
	void InitDebuggerOptions()
	{
		struct aiLogStream stream;
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
		aiAttachLogStream(&stream);
	}


	bool Load(std::string& filename, std::vector<MeshEntry>& meshArray)
	{
		int numVertices = 0;
		int numIndices = 0;
		float* vertices = nullptr;
		unsigned int* indices = nullptr;

		const aiScene* scene = aiImportFile(filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
		if (scene != nullptr && scene->HasMeshes())
		{
			// Use scene->mNumMeshes to iterate on scene->mMeshes array
			

			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				meshArray.push_back(MeshEntry());
			}
			

			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				numVertices = scene->mMeshes[i]->mNumVertices;
				vertices = new float[numVertices * 3];
			
				memcpy(vertices, scene->mMeshes[i]->mVertices, sizeof(float) * numVertices * 3);

				if (scene->mMeshes[i]->HasFaces())
				{
					numIndices = scene->mMeshes[i]->mNumFaces * 3;
					indices = new unsigned int[numIndices];

					for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
					{
						if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
							App->editor->AddLog("WARNING, geometry face with != 3 indices!");

						else
							memcpy(&indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(unsigned int));
					}
				}

				meshArray[i].Init(vertices, numVertices * 3 * sizeof(float), indices, numIndices * sizeof(unsigned int));
				delete vertices;
				delete indices;
			}

			aiReleaseImport(scene);
		}
		else
			App->editor->AddLog("Error loading scene % s", filename);

		return true;
	}


	void CleanUp()
	{
		aiDetachAllLogStreams();
	}
}
#endif // !__MESH_LOADER_H__

