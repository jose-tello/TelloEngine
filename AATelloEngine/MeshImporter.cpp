#include "MeshImporter.h"
#include "ImageImporter.h"

#include "Application.h"
#include "M_Editor.h"

#include "GameObject.h"
#include "C_Mesh.h"
#include "Mesh.h"
#include "C_Material.h"

#include "M_Scene.h"
#include "M_FileManager.h"

#include "glmath.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include <stack>

void ModelImporter::InitDebuggerOptions()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}


bool ModelImporter::Import(char* buffer, unsigned int bytes)
{
	std::stack<GameObject*> objStack;
	GameObject* root = new GameObject(nullptr);
	GameObject* obj = nullptr;

	std::stack<aiNode*> nodeStack;
	aiNode* node = nullptr;

	const aiScene* scene = aiImportFileFromMemory(buffer, bytes, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	if (scene != nullptr && scene->HasMeshes())
	{
		nodeStack.push(scene->mRootNode);
		objStack.push(root);

		while (nodeStack.empty() == false)
		{
			node = nodeStack.top();
			nodeStack.pop();

			obj = objStack.top();
			objStack.pop();

			SetObjName(obj, node);
			InitTransformComponent(obj, node);

			for (int i = 0; i < node->mNumMeshes; i++)
			{
				unsigned int meshIterator = node->mMeshes[i];

				aiMesh* mesh = scene->mMeshes[meshIterator];
				InitMeshComponent(obj, mesh);
				
				aiMaterial* material = scene->mMaterials[scene->mMeshes[meshIterator]->mMaterialIndex];
				InitMaterialComponent(obj, material);

				if (node->mNumMeshes > 1) // if there is more than one mesh, create a sibiling
				{
					GameObject* it = new GameObject(obj->parent);
					obj->parent->childs.push_back(it);
					obj = it;
					SetObjName(obj, node);
					InitTransformComponent(obj, node);
				}
			}

			for (int i = 0; i < node->mNumChildren; i++)
			{
				nodeStack.push(node->mChildren[i]);

				obj->childs.push_back(new GameObject(obj));
				objStack.push(obj->childs[i]);
			}
		}
		App->scene->AddGameObject(root);
		aiReleaseImport(scene);
	}

	else
	{
		App->editor->AddLog("Error loading scene");
		return false;
	}

	return true;
}


void ModelImporter::SetObjName(GameObject* object, aiNode* node)
{
	std::string str(node->mName.C_Str());

	str = str.substr(0, str.find_first_of("$"));
	object->SetName(str.c_str());
}


void ModelImporter::InitTransformComponent(GameObject* object, aiNode* node)
{
	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;

	node->mTransformation.Decompose(scale, rotation, position);

	Quat quat(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 pos = {position.x, position.y, position.z};
	float3 scl = { scale.x, scale.y, scale.z };

	float4x4 rot = float4x4::FromTRS(pos, quat, scl);

	object->transform.AddTransform(rot);
}


void ModelImporter::InitMeshComponent(GameObject* object, aiMesh* mesh)
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
				App->editor->AddLog("WARNING, geometry face with != 3 indices!");

			else
				memcpy(&indices[j * 3], mesh->mFaces[j].mIndices, 3 * sizeof(unsigned int));
		}
	}

	C_Mesh* meshComponent = new C_Mesh(vertices, normals, texCoords, indices);
	Save(meshComponent->GetMesh(), object->GetName());
	//Load(meshComponent->GetMesh(), );

	vertices.clear();
	normals.clear();
	texCoords.clear();
	indices.clear();

	object->AddComponent(meshComponent);
}


void ModelImporter::InitMaterialComponent(GameObject* gameObject, aiMaterial* mat)
{
	aiColor4D color;

	bool hasTextures = mat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) > 0;
	bool hasColor = aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color) == aiReturn_SUCCESS;

	if (hasTextures || hasColor)
	{
		C_Material* material = new C_Material();

		if (hasTextures)
		{
			aiString texPath;
			mat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texPath);

			material->SetTexture(ImageImporter::Import(texPath.C_Str(), false));
			material->texturePath = texPath.C_Str();
		}

		if (hasColor)
			material->SetColor(Color(color.r, color.g, color.b));

		gameObject->AddComponent(material);
	}	
}


//TODO: Use only two vectors
void ModelImporter::Load(Mesh* mesh, char* buffer)
{
	std::vector<float> vertices, normals, texCoords;
	std::vector<unsigned int> indices;
	char* pointer = buffer;

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
	bytes = sizeof(float) * ranges[1];
	normals.resize(ranges[1]);
	memcpy(&normals[0], pointer, bytes);

	pointer += bytes;

	//Texture coords
	bytes = sizeof(float) * ranges[2];
	texCoords.resize(ranges[2]);
	memcpy(&texCoords[0], pointer, bytes);

	pointer += bytes;

	//Indices
	bytes = sizeof(unsigned int) * ranges[3];
	indices.resize(ranges[3]);
	memcpy(&indices[0], pointer, bytes);

	mesh->InitVertexBuffer(&vertices[0], vertices.size());
	
	if (normals.empty() == false)
		mesh->InitNormalBuffer(&normals[0], normals.size());

	if (texCoords.empty() == false)
		mesh->InitTexCoordBuffer(&texCoords[0], texCoords.size());

	mesh->InitIndexBuffer(&indices[0], indices.size());
}


void ModelImporter::Save(Mesh* mesh, const char* fileName)
{
	std::string filePath(MESH_LIBRARY);
	filePath.append( "/");
	filePath.append(fileName);

	std::vector<float> vertices, normals, texCoords;
	std::vector<unsigned int> indices;

	mesh->GetAllVertexData(vertices, normals, texCoords, indices);

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
	bytes = sizeof(float) * normals.size();
	memcpy(pointer, &normals[0], bytes);
	pointer += bytes;

	//Store indices
	bytes = sizeof(unsigned int) * indices.size();
	memcpy(pointer, &indices[0], bytes);
	pointer += bytes;

	App->fileManager->Save(filePath.c_str(), fileBuffer, size);
}


void ModelImporter::CleanUp()
{
	aiDetachAllLogStreams();
}