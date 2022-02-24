#ifndef __M_SCEME_H__
#define __M_SCENE_H__

#include "Module.h"
#include "MathGeoLib/src/MathGeoLibFwd.h"
#include <vector>

struct GameObject;

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
	void AddEmpty();
	void AddCamera();
	GameObject* AddLight();
	void AddProceduralMesh();

	void NotifyShaderWasUpdated(int updatedShader);

	GameObject* GetGameObject(int uid);
	void GetGameObjectVector(std::vector<GameObject*>&);

	void CullGameObjects(std::vector<GameObject*>& objVector);
	void TestRayCollision(LineSegment& ray);

	void LoadScene();
	void SaveScene();

	//TODO: this should be an event
	void OnGameStart();
	void OnGameEnd();

	void GetAllGameObjects(std::vector<GameObject*>& vector) const;

private:
	void UpdateGameObjects(float dt);

	void CheckObjectsToDelete();

	void DeleteAllGameObjects();

private:
	std::vector<GameObject*> gameObjects; //Game objects without parent
};

#endif // !__M_SCEME_H__

