#ifndef __E_INSPECTOR__
#define __E_INSPECTOR__

#include "E_Window.h"

struct GameObject;
struct UniformHandle;

class C_Transform;
class C_Mesh;
class C_Material;
class C_Camera;
class C_ProceduralMesh;
class C_PointLight;

#define BACKSPACE 42
#define ADD_HEIGHT 20

#define MAX_GO_NAME_LENGTH 100

class W_Inspector : public E_Window
{
public:
	W_Inspector(bool open = true);
	~W_Inspector() override;

	bool Draw() override;

	GameObject* GetFocusedGameObject() const;
	bool GetFocusedGameObjectPos(float& x, float& y, float& z) const;
	void SetFocusedObject(GameObject* obj);
	void QuitFocusedObject();
	bool DeleteFocusedObject();

private:
	void DrawGameObject(GameObject* obj);
	void DrawTransformComp(C_Transform* trans);

	void DrawTransformPos(C_Transform* transform);
	void DrawTransformRot(C_Transform* transform);
	void DrawTransformScale(C_Transform* transform);

	void DrawMeshComp(C_Mesh* mesh);
	void DrawMaterialComp(C_Material* mat);
	void DrawShaderInfo(C_Material* mat);
	void DrawShaderUniform(UniformHandle& uniform);
	void DrawCameraComp(C_Camera* cam);
	void DrawPointLightComponent(C_PointLight* areaLight);
	void DrawProceduralMeshComp(C_ProceduralMesh* pMesh);


	bool IsDefaultUniform(const char* uniformName) const;

	void DrawAddMenu(GameObject* go);

private:
	GameObject* focusedObject = nullptr;
	bool drawAddMenu;
};

static const char* defaultUniforms[] = { "model_matrix", "projection", "view", "has_texture", "light_position",
										 "material_color", "timer", "light_color", "ambient_color", "light_intensity",
										 "light_number"};


#endif // !__E_INSPECTOR__

