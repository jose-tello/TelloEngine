#include "W_Inspector.h"

#include "GameObject.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"
#include "C_PointLight.h"
#include "C_ProceduralMesh.h"

#include "Application.h"
#include "M_Input.h"
#include "M_Editor.h"

#include "UniformHandle.h"

#include "imgui/imgui.h"

W_Inspector::W_Inspector(bool open) : E_Window(E_WINDOW_TYPE::INSPECTOR, open),
	focusedObject(nullptr),
	drawAddMenu(true)
{
}


W_Inspector::~W_Inspector()
{
	focusedObject = nullptr;
}


bool W_Inspector::Draw()
{
	ImGui::Begin("Inspector", &open);

	hovered = ImGui::IsWindowHovered();
	focused = ImGui::IsWindowFocused();

	if (focusedObject != nullptr)
	{
		DrawGameObject(focusedObject);

		std::vector<Component*> componentsVec;
		focusedObject->GetAllComponents(componentsVec);

		int objCount = componentsVec.size();
		for (int i = 0; i < objCount; i++)
		{
			switch (componentsVec[i]->GetType())
			{
			case COMPONENT_TYPE::TRANSFORM:
				DrawTransformComp(static_cast<C_Transform*>(componentsVec[i]));
				break;

			case COMPONENT_TYPE::MESH:
				DrawMeshComp(static_cast<C_Mesh*>(componentsVec[i]));
				break;

			case COMPONENT_TYPE::MATERIAL:
				DrawMaterialComp(static_cast<C_Material*>(componentsVec[i]));
				break;

			case COMPONENT_TYPE::CAMERA:
				DrawCameraComp(static_cast<C_Camera*>(componentsVec[i]));
				break;

			case COMPONENT_TYPE::PROCEDURAL_MESH:
				DrawProceduralMeshComp(static_cast<C_ProceduralMesh*>(componentsVec[i]));
				break;

			case COMPONENT_TYPE::POINT_LIGHT:
				DrawPointLightComponent(static_cast<C_PointLight*>(componentsVec[i]));
				break;

			default:
				assert("Forgot to add component");
				break;
			}
		}

		ImVec2 size = ImGui::GetWindowSize();
		size.y = ADD_HEIGHT;
		if (ImGui::Button("Add", size))
			drawAddMenu = true;

		if (drawAddMenu == true)
			DrawAddMenu(focusedObject);
	}
	else
		drawAddMenu = false;

	ImGui::End();

	return true;
}


GameObject* W_Inspector::GetFocusedGameObject() const
{
	return focusedObject;
}


bool W_Inspector::GetFocusedGameObjectPos(float& x, float& y, float& z) const
{
	if (focusedObject == nullptr)
		return false;

	focusedObject->transform.GetGlobalPos(x, y, z);
	return true;
}



void W_Inspector::SetFocusedObject(GameObject* obj)
{
	focusedObject = obj;
	drawAddMenu = false;
}


void W_Inspector::QuitFocusedObject()
{
	focusedObject = nullptr;
}


bool W_Inspector::DeleteFocusedObject()
{
	if (focusedObject != nullptr)
		focusedObject->toDelete = true;

	focusedObject = nullptr;

	return true;
}


void W_Inspector::DrawGameObject(GameObject* obj)
{
	if (ImGui::CollapsingHeader("GameObject"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		std::string name(obj->GetName());
		name.resize(MAX_GO_NAME_LENGTH);

		ImGui::InputText("Name: ", &name[0], name.length());
		obj->SetName(name.c_str());
	}

	ImGui::NewLine();
}


void W_Inspector::DrawTransformComp(C_Transform* transform)
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		DrawTransformPos(transform);

		DrawTransformRot(transform);

		DrawTransformScale(transform);
	}

	ImGui::NewLine();
}


void W_Inspector::DrawTransformPos(C_Transform* transform)
{
	float x, y, z;
	transform->GetPos(x, y, z);

	float pos[] = { x, y, z };

	if (ImGui::InputFloat3("Position", pos, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		transform->SetPos(pos[0], pos[1], pos[2]);
	
	ImGui::NewLine();
	ImGui::Separator();
}


void W_Inspector::DrawTransformRot(C_Transform* transform)
{
	float x, y, z;

	transform->GetEulerAngles(x, y, z);
	float rotation[] = { x, y, z };

	if (ImGui::InputFloat3("Rotation", rotation, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		transform->SetEulerAngles(rotation[0], rotation[1], rotation[2]);
	

	ImGui::NewLine();
	ImGui::Separator();
}


void W_Inspector::DrawTransformScale(C_Transform* transform)
{
	float x, y, z;

	transform->GetEscale(x, y, z);
	float escale[] = { x, y, z };

	if (ImGui::InputFloat3("Scale", escale, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		transform->SetEscale(escale[0], escale[1], escale[2]);
	
	ImGui::NewLine();
}


void W_Inspector::DrawMeshComp(C_Mesh* mesh)
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		unsigned int verSize, normSize, indSize;
		mesh->GetAllVectorsSize(verSize, normSize, indSize);

		ImGui::Text("Vertices: %i", verSize);
		ImGui::Text("Normals: %i", normSize);
		ImGui::Text("Indices: %i", indSize);

		ImGui::NewLine();
		ImGui::Separator();

		ImGui::Checkbox("Draw vertex normals", &mesh->drawVertexNormals);
		ImGui::Checkbox("Draw face normals", &mesh->drawFaceNormals);
	}
	ImGui::NewLine();
}


void W_Inspector::DrawMaterialComp(C_Material* material)
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Texture Path: %s", material->GetTexturePath().c_str());
		ImGui::Image((ImTextureID)material->GetTextureId(), ImVec2(104, 104));
		int width, height;
		material->GetTextureSize(width, height);
		ImGui::Text("Tex width: %i", width);
		ImGui::Text("Tex height: %i", height);

		ImGui::Spacing();
		ImGui::Separator();

		bool texEnabled = material->GetTextureEnabled();
		bool colEnabled = material->GetColorEnabled();
		bool checkEnabled = material->GetCheckerTextureEnabled();

		Color col = material->GetColor();
		ImGui::ColorPicker4("Color", &col.r);
		material->SetColor(col.r, col.g, col.b, col.a);

		ImGui::Checkbox("Texture", &texEnabled);
		ImGui::Checkbox("Color enabled", &colEnabled);
		ImGui::Checkbox("Checker tex", &checkEnabled);

		material->SetTextureEnable(texEnabled);
		material->SetColorEnable(colEnabled);
		material->SetCheckerTextureEnable(checkEnabled);

		ImGui::Spacing();
		ImGui::Separator();

		DrawShaderInfo(material);
	}

	ImGui::NewLine();
}


void W_Inspector::DrawShaderInfo(C_Material* material)
{
	ImGui::Text("Shader: %s", material->GetShaderName().c_str());
	if (ImGui::Button("Edit shader"))
		App->editor->OpenShaderEditor(material->GetShader(), material->GetShaderName().c_str());

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::Text("Shader uniforms");
	ImGui::NewLine();

	std::vector<UniformHandle> uniformVector = material->GetUniformVector();

	int uniformCount = uniformVector.size();
	for (int i = 0; i < uniformCount; i++)
	{
		if (IsDefaultUniform(uniformVector[i].GetName()) == false)
			DrawShaderUniform(uniformVector[i]);
		else
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), uniformVector[i].GetName());
	}

	material->SetUniformVector(uniformVector);
}


void W_Inspector::DrawShaderUniform(UniformHandle& uniform)
{
	std::string name = uniform.GetName();
	switch (uniform.GetVartiableType())
	{
	case VARIABLE_TYPE::NONE:
		break;


	case VARIABLE_TYPE::BOOL:
	{
		bool value = uniform.GetBool();
		ImGui::Checkbox(name.c_str(), &value);

		uniform.SetBool(value);
	}
	break;


	case VARIABLE_TYPE::UINT:
	{
		int value = uniform.GetUint();
		ImGui::InputInt(name.c_str(), &value);	//Imgui doesn't support uint input...

		uniform.SetUint(value);
	}
	break;


	case VARIABLE_TYPE::UINT_VEC2:
	{
		int value[2];
		uniform.GetUintVec2((unsigned int*)value);

		ImGui::InputInt2(name.c_str(), value);

		uniform.SetUintVec2((unsigned int*)value);
	}
	break;


	case VARIABLE_TYPE::UINT_VEC3:
	{
		int value[3];
		uniform.GetUintVec3((unsigned int*)value);

		ImGui::InputInt3(name.c_str(), value);

		uniform.SetUintVec3((unsigned int*)value);
	}
	break;


	case VARIABLE_TYPE::UINT_VEC4:
	{
		int value[4];
		uniform.GetUintVec4((unsigned int*)value);

		ImGui::InputInt4(name.c_str(), value);

		uniform.SetUintVec4((unsigned int*)value);
	}
	break;


	case VARIABLE_TYPE::INT:
	{
		int value = uniform.GetInt();
		ImGui::InputInt(name.c_str(), &value);

		uniform.SetInt(value);
	}
	break;


	case VARIABLE_TYPE::INT_VEC2:
	{
		int value[2];
		uniform.GetIntVec2(value);

		ImGui::InputInt2(name.c_str(), value);
		uniform.SetIntVec2(value);
	}
	break;


	case VARIABLE_TYPE::INT_VEC3:
	{
		int value[3];
		uniform.GetIntVec3(value);

		ImGui::InputInt3(name.c_str(), value);
		uniform.SetIntVec3(value);
	}
	break;


	case VARIABLE_TYPE::INT_VEC4:
	{
		int value[4];
		uniform.GetIntVec4(value);

		ImGui::InputInt4(name.c_str(), value);
		uniform.SetIntVec4(value);
	}
	break;


	case VARIABLE_TYPE::FLOAT:
	{
		float value = uniform.GetFloat();

		ImGui::InputFloat(name.c_str(), &value, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		uniform.SetFloat(value);
	}
	break;


	case VARIABLE_TYPE::FLOAT_VEC2:
	{
		float value[2];
		uniform.GetFloatVec2(value);

		ImGui::InputFloat2(name.c_str(), value, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		uniform.SetFloatVec2(value);
	}
	break;


	case VARIABLE_TYPE::FLOAT_VEC3:
	{
		float value[3];
		uniform.GetFloatVec3(value);

		ImGui::InputFloat3(name.c_str(), value, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		uniform.SetFloatVec3(value);
	}
	break;


	case VARIABLE_TYPE::FLOAT_VEC4:
	{
		float value[4];
		uniform.GetFloatVec4(value);

		ImGui::InputFloat4(name.c_str(), value, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		uniform.SetFloatVec4(value);
	}
	break;


	case VARIABLE_TYPE::FLOAT_MAT2:
	{
		float value[4];
		std::string id = name + " "; //Imgui doesn't support elements with the same tag
		uniform.GetMat2(value);
		
		ImGui::InputFloat2(name.c_str(), &value[0], 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		
		ImGui::InputFloat2(id.c_str(), &value[2], 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		uniform.SetMat2(value);
	}
	break;


	case VARIABLE_TYPE::FLOAT_MAT3:
	{
		float value[9];
		std::string id = name + " ";
		uniform.GetMat3(value);

		ImGui::InputFloat3(name.c_str(), &value[0], 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::InputFloat3(id.c_str(), &value[3], 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		id += " ";
		ImGui::InputFloat3(id.c_str(), &value[6], 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		uniform.SetMat3(value);
	}
	break;


	case VARIABLE_TYPE::FLOAT_MAT4:
	{
		float value[16];
		std::string id = name + " ";
		uniform.GetMat4(value);

		ImGui::InputFloat4(name.c_str(), &value[0], 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::InputFloat4(id.c_str(), &value[4], 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		id += " ";
		ImGui::InputFloat4(id.c_str(), &value[8], 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		id += " ";
		ImGui::InputFloat4(id.c_str(), &value[12], 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		uniform.SetMat4(value);
	}
		break;

	default:
		break;
	}
}


void W_Inspector::DrawCameraComp(C_Camera* camera)
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float aspectRatio = camera->GetAspectRatio();
		float verticalFov = camera->GetVerticalFov();
		float farDst = camera->GetFarPlaneDst();
		float nearDst = camera->GetNearPlaneDst();

		if (ImGui::InputFloat("Aspect ratio", &aspectRatio, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			camera->SetAspectRatio(aspectRatio);

		if (ImGui::InputFloat("Vertical fov", &verticalFov, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			camera->SetVerticalFov(verticalFov);

		if (ImGui::InputFloat("Far plane distance", &farDst, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			camera->SetFarPlaneDst(farDst);

		if (ImGui::InputFloat("Near plane distance", &nearDst, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			camera->SetNearPlaneDst(nearDst);
	}

	ImGui::NewLine();
}


void W_Inspector::DrawPointLightComponent(C_PointLight* pointLight)
{
	if (ImGui::CollapsingHeader("Point light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::NewLine();

		float lightColor[3];
		pointLight->GetLightColor(lightColor);
		
		ImGui::ColorPicker3("Light color", lightColor, 3);
		pointLight->SetLightColor(lightColor);

		float ambientColor[3];
		pointLight->GetAmbientColor(ambientColor);

		ImGui::ColorPicker3("Ambient color", ambientColor, 3);
		pointLight->SetAmbientColor(ambientColor);
		
		float lightIntensity = pointLight->GetLightIntensity();

		ImGui::DragFloat("Light intensity", &lightIntensity);
		pointLight->SetLightIntensity(lightIntensity);

		float specular = pointLight->GetSpecular();

		ImGui::DragFloat("Specular", &specular);
		pointLight->SetSpecular(specular);
	}

	ImGui::NewLine();
}


void W_Inspector::DrawProceduralMeshComp(C_ProceduralMesh* pMesh)
{
	if (ImGui::CollapsingHeader("Procedural mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::NewLine();

		int rows = pMesh->GetRows();
		int columns = pMesh->GetColumns();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Rows: %i", rows);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Columns: %i", columns);

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		ImGui::DragInt("New rows", &pMesh->inspectorRows, 0.2f, 0, 600);
		ImGui::DragInt("New columns", &pMesh->inspectorColumns, 0.2f, 0, 600);

		ImGui::NewLine();

		if (ImGui::Button("Generate mesh", ImVec2(280, 30)));
			pMesh->RecalculateMesh();
	}

	ImGui::NewLine();
}


bool W_Inspector::IsDefaultUniform(const char* uniformName) const
{
	for (int i = 0; i < sizeof(defaultUniforms) / sizeof(defaultUniforms[0]); ++i)
	{
		if (std::strcmp(uniformName, defaultUniforms[i]) == 0)
			return true;
	}

	for (int i = 0; i < sizeof(defaultStructUniforms) / sizeof(defaultStructUniforms[0]); ++i)
	{
		std::string uName = uniformName;

		if (uName.find(defaultStructUniforms[i]) != std::string::npos)
			return true;
	}

	return false;
}

//TODO: add components
void W_Inspector::DrawAddMenu(GameObject* gameObject)
{
	if (ImGui::Button("Add child"))
	{
		std::string name(gameObject->GetName());
		name.append(" child");

		GameObject* obj = new GameObject(name, gameObject);
		gameObject->childs.push_back(obj);

		drawAddMenu = false;
	}
}