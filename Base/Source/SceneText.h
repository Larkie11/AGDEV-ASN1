#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph/UpdateTransformation.h"
#include "Enemy\Enemy.h"
#include "Sound.h"
#include <ctime>


class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager
	
	Sound sound;
	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[13];
	Light* lights[2];
	GenericEntity* theCube[5];
	unsigned t0;
	string waveNo = "1";
	int currWaveEnemy = 0;
	unsigned  elasped = 0;
	int wave1 = 40;
	int wave2 = 60;
	int wave3 = 90;
	int wave4 = 140;
	int survive = 200;
	CEnemy* theEnemy;
	static SceneText* sInstance; // The pointer to the object that gets registered
};

#endif