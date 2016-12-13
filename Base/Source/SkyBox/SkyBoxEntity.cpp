#include "SkyBoxEntity.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

SkyBoxEntity::SkyBoxEntity(void)
	: size(1000.0f, 1000.0f, 1000.0f)
	, m_bBoundaryDefined(false)
{
}

SkyBoxEntity::~SkyBoxEntity()
{
}

void SkyBoxEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void SkyBoxEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();

		// Front
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -size.z / 2);
		modelStack.Scale(size.x, size.y, size.z);
			RenderHelper::RenderMesh(modelMesh[FRONT]);
		modelStack.PopMatrix();

		// Back
		modelStack.PushMatrix();
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Translate(0, 0, -size.z / 2);
		modelStack.Scale(size.x, size.y, size.z);
			RenderHelper::RenderMesh(modelMesh[BACK]);
		modelStack.PopMatrix();

		// Left
		modelStack.PushMatrix();
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Translate(0, 0, -size.z / 2);
		modelStack.Scale(size.x, size.y, size.z);
			RenderHelper::RenderMesh(modelMesh[LEFT]);
		modelStack.PopMatrix();

		// Right
		modelStack.PushMatrix();
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(0, 0, -size.z / 2);
		modelStack.Scale(size.x, size.y, size.z);
			RenderHelper::RenderMesh(modelMesh[RIGHT]);
		modelStack.PopMatrix();

		// Top
		modelStack.PushMatrix();
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Translate(0, 0, -size.z / 2);
		modelStack.Rotate(-90, 0, 0, 1);
		modelStack.Scale(size.x, size.y, size.z);
			RenderHelper::RenderMesh(modelMesh[TOP]);
		modelStack.PopMatrix();

		// Bottom
		modelStack.PushMatrix();
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Translate(0, 0, -size.z / 2);
		modelStack.Rotate(90, 0, 0, 1);
		modelStack.Scale(size.x, size.y, size.z);
			RenderHelper::RenderMesh(modelMesh[BOTTOM]);
		modelStack.PopMatrix();

	modelStack.PopMatrix();
}

// Set a mesh to this class
void SkyBoxEntity::SetMesh(const int _side, Mesh* _modelMesh)
{
	modelMesh[_side] = _modelMesh;
}

Vector3 SkyBoxEntity::GetBoundary(void)
{
	if (!m_bBoundaryDefined)
	{
		boundary = Vector3(	position.x - (size.x*scale.x) / 2.0f,
							position.y - (size.y*scale.y) / 2.0f,
							position.z - (size.z*scale.z) / 2.0f);
		m_bBoundaryDefined = true;
	}
	return boundary;
};

SkyBoxEntity* Create::SkyBox(	const std::string& _meshName0, 
								const std::string& _meshName1,
								const std::string& _meshName2,
								const std::string& _meshName3,
								const std::string& _meshName4,
								const std::string& _meshName5)
{
	Mesh* modelMesh0 = MeshBuilder::GetInstance()->GetMesh(_meshName0);
	if (modelMesh0 == nullptr)
		return nullptr;

	Mesh* modelMesh1 = MeshBuilder::GetInstance()->GetMesh(_meshName1);
	if (modelMesh1 == nullptr)
		return nullptr;

	Mesh* modelMesh2 = MeshBuilder::GetInstance()->GetMesh(_meshName2);
	if (modelMesh2 == nullptr)
		return nullptr;

	Mesh* modelMesh3 = MeshBuilder::GetInstance()->GetMesh(_meshName3);
	if (modelMesh3 == nullptr)
		return nullptr;

	Mesh* modelMesh4 = MeshBuilder::GetInstance()->GetMesh(_meshName4);
	if (modelMesh4 == nullptr)
		return nullptr;

	Mesh* modelMesh5 = MeshBuilder::GetInstance()->GetMesh(_meshName5);
	if (modelMesh5 == nullptr)
		return nullptr;

	SkyBoxEntity* result = new SkyBoxEntity();
	result->SetMesh(SkyBoxEntity::FRONT, modelMesh0);
	result->SetMesh(SkyBoxEntity::BACK, modelMesh1);
	result->SetMesh(SkyBoxEntity::LEFT, modelMesh2);
	result->SetMesh(SkyBoxEntity::RIGHT, modelMesh3);
	result->SetMesh(SkyBoxEntity::TOP, modelMesh4);
	result->SetMesh(SkyBoxEntity::BOTTOM, modelMesh5);

	EntityManager::GetInstance()->AddEntity(result);
	return result;
}
