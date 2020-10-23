#include "MeshImporter.h"

#include "Application.h"
#include "Mesh.h"
#include "M_Editor.h"

#include "M_Renderer3D.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

void ModelImporter::InitDebuggerOptions()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}


bool ModelImporter::Load(char* buffer, unsigned int bytes)
{
	std::vector<MeshEntry> meshArray;

	int numVertices = 0;
	int numTexCoords = 0;
	int numIndices = 0;

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector <unsigned int> indices;

	const aiScene* scene = aiImportFileFromMemory(buffer, bytes, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

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
			for (int j = 0; j < MAX_TEX_COORDS; j++)
			{
				if (scene->mMeshes[i]->HasTextureCoords(j))
				{
					for (int k = 0; k < scene->mMeshes[i]->mNumVertices; k++)
					{
						texCoords.push_back(scene->mMeshes[i]->mTextureCoords[j][k].x);
						texCoords.push_back(scene->mMeshes[i]->mTextureCoords[j][k].y);
					}
				}
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

		App->renderer3D->AddMesh(meshArray);
		aiReleaseImport(scene);
	}
	else
		App->editor->AddLog("Error loading scene");

	return true;
}


void ModelImporter::CleanUp()
{
	aiDetachAllLogStreams();
}