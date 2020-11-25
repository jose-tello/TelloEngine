#ifndef __M_SCEME_H__
#define __M_SCENE_H__

#include "Module.h"
#include "MathGeoLib/src/MathGeoLibFwd.h"
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
	~M_Scene() override;

	bool Start() override;
	UPDATE_STATUS Update(float dt) override;
	UPDATE_STATUS PostUpdate(float dt) override;

	bool CleanUp() override;

	void AddGameObject(GameObject*);
	void AddPrimitive(PRIMITIVE_TYPE);
	void AddEmpty();
	void AddCamera();

	GameObject* GetGameObject(int uid);
	void GetGameObjectVector(std::vector<GameObject*>&);

	void CullGameObjects(std::vector<GameObject*>& objVector);
	void DrawGameObjects(std::vector<GameObject*>& objVector, bool blackWireframe);	//Called from render
	void TestRayCollision(LineSegment& ray);

	void LoadScene();
	void SaveScene();

private:
	void UpdateGameObjects(float dt);

	void CheckObjectsToDelete();

	void DrawObject(GameObject*, bool blackWireframe);

	void GetAllGameObjects(std::vector<GameObject*>& vector) const;
	void DeleteAllGameObjects();

private:
	std::vector<GameObject*> gameObjects; //Game objects without parent
};

#endif // !__M_SCEME_H__

