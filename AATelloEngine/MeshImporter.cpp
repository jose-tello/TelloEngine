#include "MeshImporter.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Editor.h"
#include "M_Resources.h"

#include "R_Mesh.h"

#include "Assimp/include/mesh.h"
#include "MathGeoLib/src/Algorithm/Random/LCG.h"

int MeshImporter::Import(aiMesh* mesh, const char* assetPath)
{
	int numVertices = 0;
	int numTexCoords = 0;
	int numIndices = 0;

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector <unsigned int> indices;

	numVertices = mesh->mNumVertices;
	vertices.resize(numVertices * 3);

	memcpy(&vertices[0], mesh->mVertices, sizeof(float) * numVertices * 3);

	if (mesh->HasNormals())
	{
		normals.resize(numVertices * 3);

		memcpy(&normals[0], mesh->mNormals, sizeof(float) * numVertices * 3);
	}

	for (int j = 0; j < MAX_TEX_COORDS; j++)
	{
		if (mesh->HasTextureCoords(j))
		{
			texCoords.reserve(numVertices * 2);

			for (int k = 0; k < numVertices; k++)
			{
				texCoords.push_back(mesh->mTextureCoords[j][k].x);
				texCoords.push_back(mesh->mTextureCoords[j][k].y);
			}
		}
	}

	if (mesh->HasFaces())
	{
		numIndices = mesh->mNumFaces * 3;
		indices.resize(numIndices);

		for (int j = 0; j < mesh->mNumFaces; j++)
		{
			if (mesh->mFaces[j].mNumIndices != 3)
				App->editor->AddLog("[WARNING], geometry face with != 3 indices!");

			else
				memcpy(&indices[j * 3], mesh->mFaces[j].mIndices, 3 * sizeof(unsigned int));
		}
	}

	LCG random;
	R_Mesh* resourceMesh = new R_Mesh(random.IntFast(), assetPath, RESOURCE_TYPE::MESH);

	Save(vertices, normals, texCoords, indices, resourceMesh->GetUid());
	App->resourceManager->PushResource(resourceMesh, resourceMesh->GetUid());

	vertices.clear();
	normals.clear();
	texCoords.clear();
	indices.clear();

	return resourceMesh->GetUid();
}


void MeshImporter::Load(R_Mesh* mesh)
{
	std::vector<float> vertices, normals, texCoords;
	std::vector<unsigned int> indices;
	char* fileBuffer = nullptr;

	std::string path(MESH_LIBRARY);
	path.append(std::to_string(mesh->GetUid()));

	unsigned int size = App->fileManager->Load(path.c_str(), &fileBuffer);
	char* pointer = fileBuffer;

	if (size > 0)
	{
		//Ranges
		unsigned int ranges[4];
		unsigned int bytes = sizeof(ranges);
		memcpy(ranges, pointer, bytes);

		pointer += bytes;

		//Vertices
		bytes = sizeof(float) * ranges[0];
		vertices.resize(ranges[0]);
		memcpy(&vertices[0], pointer, bytes);

		pointer += bytes;

		//Normals
		if (ranges[1] != 0)
		{
			bytes = sizeof(float) * ranges[1];
			normals.resize(ranges[1]);
			memcpy(&normals[0], pointer, bytes);

			pointer += bytes;
		}

		//Texture coords
		if (ranges[2] != 0)
		{
			bytes = sizeof(float) * ranges[2];
			texCoords.resize(ranges[2]);
			memcpy(&texCoords[0], pointer, bytes);

			pointer += bytes;
		}

		//Indices
		bytes = sizeof(unsigned int) * ranges[3];
		indices.resize(ranges[3]);
		memcpy(&indices[0], pointer, bytes);

		float* normalsPointer = nullptr;
		float* texCoordsPointer = nullptr;

		if (normals.empty() == false)
			normalsPointer = &normals[0];

		if (texCoords.empty() == false)
			texCoordsPointer = &texCoords[0];

		mesh->InitVAO(&vertices[0], vertices.size() * sizeof(float), &indices[0], indices.size() * sizeof(unsigned int),
			normalsPointer, normals.size() * sizeof(float), texCoordsPointer, texCoords.size() * sizeof(float));

		delete[] fileBuffer;
		fileBuffer = nullptr;
		pointer = nullptr;
	}
	else
		App->editor->AddLog("[WARNING], mesh not loaded correctlly from: %s", path.c_str());
}


void MeshImporter::Save(std::vector<float>& vertices, std::vector<float>& normals, std::vector<float>& texCoords,
	std::vector<unsigned int>& indices, int uid)
{
	std::string filePath(MESH_LIBRARY);
	filePath.append(std::to_string(uid));

	unsigned int ranges[4] = { vertices.size(), normals.size(), texCoords.size(), indices.size() };

	unsigned int size = sizeof(ranges) + sizeof(float) * vertices.size() + sizeof(float) * normals.size() +
		sizeof(float) * texCoords.size() + sizeof(unsigned int) * indices.size();

	char* fileBuffer = new char[size];
	char* pointer = fileBuffer;

	//Store ranges
	unsigned int bytes = sizeof(ranges);
	memcpy(pointer, ranges, bytes);
	pointer += bytes;

	//Store vertices
	bytes = sizeof(float) * vertices.size();
	memcpy(pointer, &vertices[0], bytes);
	pointer += bytes;

	//Store normals
	if (normals.empty() == false)
	{
		bytes = sizeof(float) * normals.size();
		memcpy(pointer, &normals[0], bytes);
		pointer += bytes;
	}

	//Store tex coords
	if (texCoords.empty() == false)
	{
		bytes = sizeof(float) * texCoords.size();
		memcpy(pointer, &texCoords[0], bytes);
		pointer += bytes;
	}

	//Store indices
	bytes = sizeof(unsigned int) * indices.size();
	memcpy(pointer, &indices[0], bytes);

	App->fileManager->Save(filePath.c_str(), fileBuffer, size);

	delete[] fileBuffer;
	fileBuffer = nullptr;
	pointer = nullptr;
}