#include "MeshImporter.h"
#include "ImageImporter.h"

#include "Application.h"
#include "M_Editor.h"

#include "GameObject.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include "M_Scene.h"

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


bool ModelImporter::Load(char* buffer, unsigned int bytes)
{
	std::stack<GameObject*> objStack;
	GameObject* root = new GameObject(nullptr);
	GameObject* obj = nullptr;

	std::stack<aiNode*> nodeStack;
	aiNode* node = nullptr;

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
				//TODO: i hate this if
				if (i > 0)
				{
					GameObject* it = new GameObject(obj->parent);
					obj->parent->childs.push_back(it);
					obj = it;
					SetObjName(obj, node);
					InitTransformComponent(obj, node);
				}
				unsigned int mesh = node->mMeshes[i];

				numVertices = scene->mMeshes[mesh]->mNumVertices;
				vertices.resize(numVertices * 3);

				memcpy(&vertices[0], scene->mMeshes[mesh]->mVertices, sizeof(float) * numVertices * 3);

				if (scene->mMeshes[mesh]->HasNormals())
				{
					normals.resize(numVertices * 3);

					memcpy(&normals[0], scene->mMeshes[mesh]->mNormals, sizeof(float) * numVertices * 3);
				}

				for (int j = 0; j < MAX_TEX_COORDS; j++)
				{
					if (scene->mMeshes[mesh]->HasTextureCoords(j))
					{
						texCoords.reserve(numVertices * 2);

						for (int k = 0; k < numVertices; k++)
						{
							texCoords.push_back(scene->mMeshes[mesh]->mTextureCoords[j][k].x);
							texCoords.push_back(scene->mMeshes[mesh]->mTextureCoords[j][k].y);
						}
					}
				}

				if (scene->mMeshes[mesh]->HasFaces())
				{
					numIndices = scene->mMeshes[mesh]->mNumFaces * 3;
					indices.resize(numIndices);

					for (int j = 0; j < scene->mMeshes[mesh]->mNumFaces; j++)
					{
						if (scene->mMeshes[mesh]->mFaces[j].mNumIndices != 3)
							App->editor->AddLog("WARNING, geometry face with != 3 indices!");

						else
							memcpy(&indices[j * 3], scene->mMeshes[mesh]->mFaces[j].mIndices, 3 * sizeof(unsigned int));
					}
				}

				InitMeshComponent(obj, vertices, normals, texCoords, indices);
				
				aiMaterial* material = scene->mMaterials[scene->mMeshes[mesh]->mMaterialIndex];

				if (material->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) > 0)
					InitMaterialComponent(obj, material);			
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
	mat4x4 mat;

	node->mTransformation.Decompose(scale, rotation, position);
	Quat quat(rotation.x, rotation.y, rotation.z, rotation.w);

	object->transform.SetEscale(scale.x, scale.y, scale.z);
	mat.rotate(quat.Angle() * RADTODEG, vec3(quat.Axis().x, quat.Axis().y, quat.Axis().z));
	object->transform.AddTransform(mat);

	object->transform.SetPos(position.x, position.y, position.z);
}


void ModelImporter::InitMeshComponent(GameObject* object, std::vector<float>& vertices, std::vector<float>& normals, std::vector<float>& texCoords, std::vector<unsigned int>& indices)
{
	C_Mesh* meshComponent = new C_Mesh(object);
	meshComponent->InitVertexBuffer(&vertices[0], vertices.size() * sizeof(float));

	if (normals.empty() == false)
		meshComponent->InitNormalBuffer(&normals[0], normals.size() * sizeof(float));

	if (texCoords.empty() == false)
		meshComponent->InitTexCoordBuffer(&texCoords[0], texCoords.size() * sizeof(float));

	if (indices.empty() == false)
		meshComponent->InitIndexBuffer(&indices[0], indices.size() * sizeof(unsigned int));

	vertices.clear();
	normals.clear();
	texCoords.clear();
	indices.clear();

	object->AddComponent(meshComponent);
}


void ModelImporter::InitMaterialComponent(GameObject* object, aiMaterial* mat)
{
	C_Material* material = new C_Material(object);

	aiString texPath;
	mat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texPath);

	material->SetTexture(ImageImporter::Load(texPath.C_Str(), false));

	object->AddComponent(material);
}


void ModelImporter::CleanUp()
{
	aiDetachAllLogStreams();
}