#ifndef __MESH_LOADER_H__
#define __MESH_LOADER_H__

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Application.h"
#include "Mesh.h"
#include "M_Editor.h"
#include "M_AssetManager.h"


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
		int numTexCoords = 0;
		int numIndices = 0;

		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> texCoords;
		std::vector <unsigned int> indices;

		/*char* buffer = nullptr;
		unsigned int lenght = App->assetManager->Load(filename.c_str(), buffer);
		const aiScene* scene = aiImportFileFromMemory(buffer, lenght, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);*/

		const aiScene* scene = aiImportFile(filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
		if (scene != nullptr && scene->HasMeshes())
		{
			// Use scene->mNumMeshes to iterate on scene->mMeshes array

			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				numVertices = scene->mMeshes[i]->mNumVertices;
				vertices.resize(numVertices * 3);

				memcpy(&vertices[0], scene->mMeshes[i]->mVertices, sizeof(float) * numVertices * 3);

				if (scene->mMeshes[i]->HasNormals())
				{
					normals.resize(numVertices * 3);

					memcpy(&normals[0], scene->mMeshes[i]->mNormals, sizeof(float) * numVertices * 3);
				}

				//TODO make this check the 8 tex coords abaliable
				if (scene->mMeshes[i]->HasTextureCoords(0))
				{
					texCoords.resize(numVertices * 2);

					memcpy(&texCoords[0], scene->mMeshes[i]->mTextureCoords[0], sizeof(float) * numVertices * 2);
				}

				if (scene->mMeshes[i]->HasFaces())
				{
					numIndices = scene->mMeshes[i]->mNumFaces * 3;
					indices.resize(numIndices);

					for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
					{
						if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
							App->editor->AddLog("WARNING, geometry face with != 3 indices!");

						else
							memcpy(&indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(unsigned int));
					}
				}

				meshArray.push_back(MeshEntry());
				meshArray[i].InitVertexBuffer(&vertices[0], numVertices * 3 * sizeof(float));
				
				if (normals.empty() == false)
					meshArray[i].InitNormalBuffer(&normals[0], numVertices * 3 * sizeof(float));
				
				if (texCoords.empty() == false)
					meshArray[i].InitTexCoordBuffer(&texCoords[0], numVertices * 2 * sizeof(float));

				if (indices.empty() == false)
					meshArray[i].InitIndexBuffer(&indices[0], numIndices * sizeof(unsigned int));

				vertices.clear();
				normals.clear();
				texCoords.clear();
				indices.clear();
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

