#ifndef __M_SCEME_H__
#define __M_SCENE_H__

#include "Module.h"

#include <vector>

struct GameObject;

enum class PRIMITIVE_TYPE : int
{
	NONE = -1,
	CUBE,
	PIRAMID,
	SPHERE,
	CILINDER
};


class M_Scene : public Module
{
public:
	M_Scene(bool start_enabled = true);
	~M_Scene();

	bool Start() override;
	UPDATE_STATUS Update(float dt) override;
	UPDATE_STATUS PostUpdate(float dt) override;

	bool CleanUp() override;

	void AddGameObject(GameObject*);
	void AddPrimitive(PRIMITIVE_TYPE);

	void GetGameObjectVector(std::vector<GameObject*>&);

	void DrawGameObjects(bool blackWireframe);

	void SaveScene();

private:
	//TODO: Considering using pointers to functions, ask Marc about it
	void UpdateGameObjects(float dt);
	void PostUpdateGameObjects(float dt);

	void CheckObjectsToDelete();

	void DrawObject(GameObject*, bool blackWireframe);

private:
	std::vector<GameObject*> gameObjects; //Game objects without parent
};

#endif // !__M_SCEME_H__

