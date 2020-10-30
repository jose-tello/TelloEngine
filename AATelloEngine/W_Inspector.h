#ifndef __E_INSPECTOR__
#define __E_INSPECTOR__

#include "E_Window.h"

struct GameObject;
class C_Transform;
class C_Mesh;
class C_Material;

#define BACKSPACE 42

class W_Inspector : public E_Window
{
public:
	W_Inspector(bool open = true);
	~W_Inspector() override;

	bool Draw() override;

	GameObject* GetFocusedGameObject() const;
	void SetFocusedObject(GameObject* obj);
	void QuitFocusedObject();
	bool DeleteFocusedObject();

private:
	void DrawGameObject(GameObject* obj);
	void DrawTransformComp(C_Transform*);
	void DrawMeshComp(C_Mesh*);
	void DrawMaterialComp(C_Material*);

private:
	GameObject* focusedObject = nullptr;
};


#endif // !__E_INSPECTOR__

