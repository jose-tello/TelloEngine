#ifndef __M_SCEME_H__
#define __M_SCENE_H__

#include "Module.h"

#include <vector>

struct GameObject;

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

	//TODO: Considering using pointers to functions, ask Marc about it
	void UpdateGameObjects(float dt);
	void PostUpdateGameObjects(float dt);

	void DrawGameObjects(bool drawVertexNormals, bool drawFaceNormals, bool black);
	void DrawObject(GameObject*, bool drawVertexNormals, bool drawFaceNormals, bool black);
private:

	std::vector<GameObject*> gameObjects; //Game objects without parent
};

#endif // !__M_SCEME_H__

