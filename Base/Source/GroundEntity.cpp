#include "GroundEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

GroundEntity::GroundEntity(Mesh* _modelMesh1, Mesh* _modelMesh2) 
	: position(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f)
	, size(1.0f, 1.0f, 1.0f)
	, grids(10.0f, 0.0f, 10.0f)
	, maxBoundary(1.0f, 1.0f, 1.0f)
	, minBoundary(1.0f, 1.0f, 1.0f)
	, m_bMaxBoundaryDefined(false)
	, m_bMinBoundaryDefined(false)
{
	modelMesh[0] = _modelMesh1;
	modelMesh[1] = _modelMesh2;
}

GroundEntity::~GroundEntity()
{
}

void GroundEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void GroundEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	// the y- and z- components are swapped because of the way that MVP is calculated inside RenderMesh
	modelStack.Translate(position.x, position.z, position.y);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);

	// Since the ground textures are displayed with the centre as the reference point,
	// we need to offset the tiles by half of the tile size
	modelStack.Translate(size.x / 2, size.z / 2, 0.0f);
	for (int x = 0; x < (int)grids.x; x++)
	{
		for (int z = 0; z < (int)grids.z; z++)
		{
			modelStack.PushMatrix();
			// the y- and z- components are swapped because of the way that MVP is calculated inside RenderMesh
			modelStack.Translate(x - (grids.x * size.x) / 2, z - (grids.z * size.z) / 2, 0.0f);
			if (((x * (int)(grids.x - 1) + z) % 2) == 0)
				RenderHelper::RenderMesh(modelMesh[0]);
			else
				RenderHelper::RenderMesh(modelMesh[1]);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
}

float GroundEntity::GetTerrainHeight(Vector3 position)
{
	// If a contour map is loaded into this ground entity, then get the height of the contour for position.x and position.z
	return 0.0f;
}

Vector3 GroundEntity::GetMaxBoundary(void)
{ 
	if (!m_bMaxBoundaryDefined)
	{
		maxBoundary = Vector3(	position.x + (grids.x*size.x*scale.x) / 2.0f, 
								position.y + (grids.y*size.y*scale.y) / 2.0f, 
								position.z + (grids.z*size.z*scale.z) / 2.0f);
		m_bMaxBoundaryDefined = true;
	}
	return maxBoundary;
};
Vector3 GroundEntity::GetMinBoundary(void)
{ 
	if (!m_bMinBoundaryDefined)
	{
		minBoundary = Vector3(	position.x - (grids.x*size.x*scale.x) / 2.0f, 
								position.y - (grids.y*size.y*scale.y) / 2.0f, 
								position.z - (grids.z*size.z*scale.z) / 2.0f);
		m_bMinBoundaryDefined = true;
	}
	return minBoundary;
};

GroundEntity* Create::Ground(const std::string& _meshName1, const std::string& _meshName2)
{
	Mesh* modelMesh1 = MeshBuilder::GetInstance()->GetMesh(_meshName1);
	if (modelMesh1 == nullptr)
		return nullptr;

	Mesh* modelMesh2 = MeshBuilder::GetInstance()->GetMesh(_meshName2);
	if (modelMesh2 == nullptr)
		return nullptr;

	GroundEntity* result = new GroundEntity(modelMesh1, modelMesh2);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}
