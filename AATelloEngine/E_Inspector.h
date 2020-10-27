#ifndef __E_INSPECTOR__
#define __E_INSPECTOR__

#include "E_Window.h"

struct GameObject;
class C_Transform;
class C_Mesh;
class C_Material;

class E_Inspector : public E_Window
{
public:
	E_Inspector(bool open = true);
	~E_Inspector();

	bool Draw() override;

	void SetFocusedObject(GameObject* obj);
	void QuitFocusedObject();
	bool DeleteFocusedObject(); //TODO: Implement this (not necessary for now)

private:
	void DrawGameObject(GameObject* obj);
	void DrawTransformComp(C_Transform*);
	void DrawMeshComp(C_Mesh*);
	void DrawMaterialComp(C_Material*);

private:
	GameObject* focusedObject = nullptr;
};


#endif // !__E_INSPECTOR__

