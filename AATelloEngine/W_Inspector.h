#ifndef __E_INSPECTOR__
#define __E_INSPECTOR__

#include "E_Window.h"

struct GameObject;
struct UniformHandle;

class C_Transform;
class C_Mesh;
class C_Material;
class C_Camera;

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
	void DrawTransformComp(C_Transform*);

	void DrawTransformPos(C_Transform* transform);
	void DrawTransformRot(C_Transform* transform);
	void DrawTransformScale(C_Transform* transform);

	void DrawMeshComp(C_Mesh*);
	void DrawMaterialComp(C_Material*);
	void DrawShaderInfo(C_Material*);
	void DrawShaderUniform(UniformHandle&);
	void DrawCameraComp(C_Camera*);

	void DrawAddMenu(GameObject*);

private:
	GameObject* focusedObject = nullptr;
	bool drawAddMenu;
};


#endif // !__E_INSPECTOR__

