#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "SceneGraph\SceneGraph.h"
#include "SpatialPartition\SpatialPartition.h"

#include <iostream>
using namespace std;

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

SceneText::SceneText(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("Start", this);
}

SceneText::~SceneText()
{
	CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	// Tell the shader program to store these uniform locations
	t0 = clock();
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");
	
	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);
	
	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sgbullet", Color(1, 0, 0), 18, 36, 0.2f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//grass_darkgreen.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass_lightgreen.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GROUND")->textureID = LoadTGA("Image//ground.tga");
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);
	//MeshBuilder::GetInstance()->GenerateRectangular("hand", Color(1.0f, 0.64f, 0.0f), 1.0f,5.0f,1.0f);
	////LOD objs for assignment 1
	MeshBuilder::GetInstance()->GenerateOBJ("m24r", "Obj//M24R.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Camp", "Obj//Camp.obj");
	MeshBuilder::GetInstance()->GetMesh("Camp")->textureID = LoadTGA("Image//wood.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Head", "Obj//Head.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Body", "Obj//Body.obj");

	MeshBuilder::GetInstance()->GenerateOBJ("Hand", "Obj//Hand.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("robot1_high", "Obj//robot1_high.obj"); //model 1
	MeshBuilder::GetInstance()->GetMesh("robot1_high")->textureID = LoadTGA("Image//ROBOT1_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("robot1_mid", "Obj//robot1_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("robot1_mid")->textureID = LoadTGA("Image//ROBOT1_mid.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("robot1_low", "Obj//robot1_low.obj");
	MeshBuilder::GetInstance()->GetMesh("robot1_low")->textureID = LoadTGA("Image//ROBOT1_low.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Android_high", "Obj//android_high.obj"); //model 2
	MeshBuilder::GetInstance()->GetMesh("Android_high")->textureID = LoadTGA("Image//android_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Android_mid", "Obj//android_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Android_mid")->textureID = LoadTGA("Image//android_mid.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Android_low", "Obj//android_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Android_low")->textureID = LoadTGA("Image//android_low.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Dog_high", "Obj//dog_high.obj"); //model 3
	MeshBuilder::GetInstance()->GetMesh("Dog_high")->textureID = LoadTGA("Image//dog_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Dog_mid", "Obj//dog_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Dog_mid")->textureID = LoadTGA("Image//dog_mid.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Dog_low", "Obj//dog_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Dog_low")->textureID = LoadTGA("Image//dog_low.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("House_high", "Obj//house_high.obj"); //model 4
	MeshBuilder::GetInstance()->GetMesh("House_high")->textureID = LoadTGA("Image//house_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("House_mid", "Obj//house_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("House_mid")->textureID = LoadTGA("Image//house_mid.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("House_low", "Obj//house_low.obj");
	MeshBuilder::GetInstance()->GetMesh("House_low")->textureID = LoadTGA("Image//house_low.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Tree_high", "Obj//tree_high.obj"); //model 5
	MeshBuilder::GetInstance()->GetMesh("Tree_high")->textureID = LoadTGA("Image//tree_high.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Tree_mid", "Obj//tree_mid.obj");
	MeshBuilder::GetInstance()->GetMesh("Tree_mid")->textureID = LoadTGA("Image//tree_mid.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Tree_low", "Obj//tree_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Tree_low")->textureID = LoadTGA("Image//tree_low.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("pistolB", "Obj//pistolbullet.obj");
	MeshBuilder::GetInstance()->GetMesh("pistolB")->textureID = LoadTGA("Image//pistolbullet.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("shotgunB", "Obj//shotgunbullet.obj");
	MeshBuilder::GetInstance()->GetMesh("shotgunB")->textureID = LoadTGA("Image//shotgunbullet.tga");
	
	//MeshBuilder::GetInstance()->GetMesh("m24r")->textureID = LoadTGA("Image//M24R.tga");
	//MeshBuilder::GetInstance()->GenerateOBJ("robothead", "Obj//RobotHead.obj");

	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 1, 1), 10.f);

	// Set up the Spatial Partition and pass it to the EntityManager to manage
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDistance(40000,160000.f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball

	/*GenericEntity* android = Create::Entity("Android_high", Vector3(100.0f, 0.0f, -100.0f));
	android->SetCollider(true);
	android->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	android->InitLOD("Android_high", "Android_mid", "Android_low");*/

	/*GenericEntity* dog = Create::Entity("Dog_high", Vector3(-100.0f, 0.0f, 100.0f));
	dog->SetCollider(true);
	dog->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	dog->InitLOD("Dog_high", "Dog_mid", "Dog_low");*/

	/*GenericEntity* house = Create::Entity("House_high", Vector3(-300.0f, -5.0f, 0.0f));
	house->SetCollider(true);
	house->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	house->InitLOD("House_high", "House_mid", "House_low");*/

	/*GenericEntity* tree = Create::Entity("Tree_high", Vector3(400.0f, -5.0f, 0.0f));
	tree->SetCollider(true);
	tree->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	tree->InitLOD("Tree_high", "Tree_mid", "Tree_low");*/

	//GenericEntity* aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, -20.0f));
	//aCube->SetCollider(true);
	//aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	//aCube->InitLOD("cube", "lightball", "cubeSG");

	//// Add the pointer to this new entity to the Scene Graph
	//CSceneNode* theNode = CSceneGraph::GetInstance()->AddNode(aCube);
	//if (theNode == NULL)
	//{
	//	cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	//}

	//GenericEntity* anotherCube = Create::Entity("cube", Vector3(-20.0f, 1.1f, -20.0f));
	//anotherCube->SetCollider(true);
	//anotherCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	//anotherCube->InitLOD("cube", "lightball", "lightball");
	//CSceneNode* anotherNode = theNode->AddChild(anotherCube);
	//if (anotherNode == NULL)
	//{
	//	cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	//}

	GenericEntity* anotherCube = Create::Entity("Camp", Vector3(0.0f, -5.f, 0.0f));
	anotherCube->SetCollider(true);
	anotherCube->SetAABB(Vector3(10.f, 10.f, 10.f), Vector3(-10.f, -10.f, -10.f));
	anotherCube->InitLOD("Camp", "lightball", "lightball");
	anotherCube->IsEnemy(false);
	anotherCube->IsCamp("Camp");

	//GenericEntity* bCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, -10.0f));
	//bCube->SetCollider(true);
	//bCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	//bCube->InitLOD("cubeSG", "lightball", "cubeSG");
	//CSceneNode* node = theNode->AddChild(bCube);
	////
	//GenericEntity* baseCube = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(baseCube);

	//baseCube->InitLOD("cubeSG", "lightball", "lightball");

	//CUpdateTransformation* baseMtx = new CUpdateTransformation();
	//baseMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	//baseMtx->SetSteps(-60, 60);
	//baseNode->SetUpdateTransformation(baseMtx);

	//GenericEntity* childCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* childNode = baseNode->AddChild(childCube);
	//childNode->ApplyTranslate(0.0f, 1.0f, 0.0f);
	//childCube->InitLOD("cubeSG", "lightball", "cube");

	//GenericEntity* grandchildCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	//CSceneNode* grandchildNode = childNode->AddChild(grandchildCube);
	//grandchildNode->ApplyTranslate(0.0f, 0.0f, 1.0f);
	//CUpdateTransformation* aRotateMtx = new CUpdateTransformation();
	//aRotateMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	//aRotateMtx->SetSteps(-120, 60);
	//grandchildNode->SetUpdateTransformation(aRotateMtx);
	//grandchildCube->InitLOD("cubeSG", "lightball", "cubeSG");

	groundEntity = Create::Ground("GROUND", "GROUND");
//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);
	for (int i = 0; i < 3; i++)
	{
		theEnemy= Create::Enemy(Vector3(Math::RandIntMinMax(-250, -200), 0.0f, Math::RandIntMinMax(-200, -200)),Vector3(Math::RandIntMinMax(-50, 50), 0.0f, Math::RandIntMinMax(-50, 50)), Math::RandFloatMinMax(5.f,15.f), groundEntity);
	}
/*for (int i = 0; i < 10;)
	{
		theEnemy = new CEnemy();
		theEnemy->Init();
		theEnemy->SetTerrain(groundEntity);
		theEnemy->SetPosition(Vector3(Math::RandIntMinMax(-100, 100), 0.0f, Math::RandIntMinMax(0, 100)));
		theEnemy->SetTarget(Vector3(Math::RandIntMinMax(-10, 10), 0.0f, Math::RandIntMinMax(-10, 10)));
		i++;
	}*/
	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;

	for (int i = 0; i < 10; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	}
	textObj[0]->SetText("HELLO WORLD");
}

void SceneText::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);
	elasped = (clock() - t0) /CLOCKS_PER_SEC;
	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}
	if (elasped == 50 && waveNo == "1")
	{
		waveNo = "2";
	}
	if (elasped == 100 && waveNo == "2.1")
	{
		waveNo = "3";
		currWaveEnemy = 0;
	}
	if (waveNo == "2")
	{
		for (currWaveEnemy; currWaveEnemy < 3;)
		{
			theEnemy = Create::Enemy(Vector3(Math::RandIntMinMax(250, 200), 0.0f, Math::RandIntMinMax(-200, -200)), Vector3(Math::RandIntMinMax(-50, 50), 0.0f, Math::RandIntMinMax(-50, 50)), Math::RandFloatMinMax(5.f, 15.f), groundEntity);
			currWaveEnemy++;
		}
		if (currWaveEnemy > 1)
			waveNo = "2.1";
	}
	if (waveNo == "3")
	{
		for (currWaveEnemy; currWaveEnemy < 3;)
		{
			theEnemy = Create::Enemy(Vector3(Math::RandIntMinMax(250, 200), 0.0f, Math::RandIntMinMax(-200, -200)), Vector3(Math::RandIntMinMax(-50, 50), 0.0f, Math::RandIntMinMax(-50, 50)), Math::RandFloatMinMax(5.f, 15.f), groundEntity);
			currWaveEnemy++;
		}
		if (currWaveEnemy > 1)
			waveNo = "3.1";
	}
	if(KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		CSceneNode* theNode = CSceneGraph::GetInstance()->GetNode(1);
		Vector3 pos = theNode->GetEntity()->GetPosition();
		theNode->GetEntity()->SetPosition(Vector3(pos.x + 40.0f, pos.y, pos.z + 40.0f));
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('N'))
	{
		CSpatialPartition::GetInstance()->PrintSelf();
	}

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);
	playerInfo->Render("m24r");
	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[1]->SetText(ss.str());



	std::ostringstream ss1;
	ss1.precision(10);
	//ss1 << "Player:" << playerInfo->GetPos();
	//textObj[2]->SetText(ss1.str());
	ss1.str("");
	ss1 << "TIME: " << elasped;
	textObj[2]->SetText(ss1.str());

	ss1.str("");
	ss1 << "Ammo: " << playerInfo->GetMagRound();
	textObj[3]->SetText(ss1.str()); 

	ss1.str("");
	ss1 << "Player Health: "<<  playerInfo->playerHealth;
	textObj[4]->SetText(ss1.str());

	ss1.str("");
	ss1 << "Weapon: " << playerInfo->GetWeaponName();
	textObj[5]->SetText(ss1.str());
	/*ss1.str("");
	ss1 << "Enemies: " << theEnemy->GetEnemyCount();
	textObj[5]->SetText(ss1.str());*/

}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}
