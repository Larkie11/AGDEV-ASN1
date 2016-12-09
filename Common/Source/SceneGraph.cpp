#include "SceneGraph.h"
#include "MeshBuilder.h"
#include "../../Base/Source/EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
CSceneGraph::CSceneGraph(void)
	: ID(0)
	, theRoot(NULL)
{
	theRoot = new CSceneNode();
	// Assign the first ID to the root. Default is 0
	theRoot->SetID(this->GenerateID());
}
CSceneGraph::~CSceneGraph()
{
}
void CSceneGraph::Destroy() 
{
	if (theRoot)
	{
		theRoot->DeleteAllChildren();
		delete theRoot;
	}
	Singleton<CSceneGraph>::Destroy();
}
CSceneNode* CSceneGraph::GetRoot()
{
	return theRoot;
}
// Add a Node to this Scene Graph
CSceneNode* CSceneGraph::AddNode(EntityBase* theEntity)
{
	CSceneNode* aNewSceneNode = theRoot->AddChild(theEntity);
	// aNewSceneNode->SetID(this->GenerateID());
	return aNewSceneNode;
}
// Delete a Node from this Scene Graph using the pointer to the node
bool CSceneGraph::DeleteNode(EntityBase* theEntity)
{
	return theRoot->DeleteChild(theEntity);
}
// Delete a Node from this Scene Graph using its ID
bool CSceneGraph::DeleteNode(const int ID)
{
	return theRoot->DeleteChild(ID);
}
// Generate an ID for a Scene Node
int CSceneGraph::GenerateID(void)
{
	// Return the ID and increment the value by 1
	return ID++;
}
// PrintSelf for debug purposes
void CSceneGraph::PrintSelf(void)
{
	theRoot->PrintSelf();
}