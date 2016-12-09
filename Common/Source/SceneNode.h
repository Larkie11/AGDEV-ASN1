#pragma once

#include "Vector3.h"
#include <vector>
using namespace std;

#include "EntityBase.h"

class CSceneNode
{
public:
	CSceneNode(void);
	virtual ~CSceneNode();

	void Destroy();

	void SetID(const int ID);
	int GetID(void)const;

	bool SetEntity(EntityBase* theEntity);
	//Get Id for this node
	EntityBase* GetEntity(void)const;

	//Set parent to this node
	void SetParent(CSceneNode* theParent);
	//Get parent of this node
	CSceneNode* GetParent(void)const;

	CSceneNode* AddChild(EntityBase* theEntity = NULL);
	bool DeleteChild(EntityBase* theEntity = NULL);
	bool DeleteChild(const int ID);
	bool DeleteAllChildren(void);
	CSceneNode* DetachChild(EntityBase* theEntity = NULL);
	CSceneNode* DetachChild(const int ID);

	//Get entity inside scene graph
	CSceneNode* GetEntity(EntityBase* theEntity);
	//Get entity inside this scene graph using its ID
	CSceneNode* GetEntity(const int ID);

	int GetNumOfChild(void);
	void PrintSelf(const int numTabs = 0);
protected:
	int ID;
	EntityBase* theEntity;
	CSceneNode* theParent;

	vector<CSceneNode*> theChildren;
};