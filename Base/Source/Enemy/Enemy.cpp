#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MyMath.h"
#include "MeshBuilder.h"
#include "SceneNode.h"
#include "SceneGraph.h"

CEnemy::CEnemy() : GenericEntity(NULL)
, defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
, target(Vector3(0.0f, 0.0f, 0.0f))
, up(Vector3(0.0f, 0.0f, 0.0f))
, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
, m_pTerrain(NULL)
, distance(0)
{
	defaultPosition.SetZero();
	defaultTarget.SetZero();
	defaultUp.Set(0, 1, 0);

	InitLOD("m24r", "sphere", "cube");
	//Initialise the collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);
	up.Set(0.0f, 1.0f, 0.0f);

}

CEnemy::~CEnemy()
{

}
void CEnemy::Init(void)
{
	//Set current values
	//position.Set(Math::RandIntMinMax(0, 10), 0.0f, Math::RandIntMinMax(0, 10));
	//target.Set(10.0f, 0.0f, 450.f);
	//moveto.Set(Math::RandIntMinMax(100, 200), 0.0f, Math::RandIntMinMax(100, 200));
	//Set boundary
	CSceneNode* enemyNode = CSceneGraph::GetInstance()->AddNode(this);

	bCube = Create::Entity("cube", Vector3(position.x, position.y - 5, position.z));
	bCube->SetCollider(true);
	bCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	bCube->InitLOD("cubeSG", "lightball", "cubeSG");
	CSceneNode* node = enemyNode->AddChild(bCube);
}

void CEnemy::Reset(void)
{
	//Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void CEnemy::SetPos(const Vector3& pos)
{
	position = pos;
}

void CEnemy::SetTarget(const Vector3& target)
{
	this->target = target;
}

void CEnemy::SetUp(const Vector3& up)
{
	this->up = up;
}
void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
void CEnemy::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;
		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}
Vector3 CEnemy::GetPos(void) const
{
	return position;
}
Vector3 CEnemy::GetTarget(void) const
{
	return target;
}
Vector3 CEnemy::GetUp(void) const
{
	return up;
}
GroundEntity* CEnemy::GetTerrain(void)
{
	return m_pTerrain;
}
void CEnemy::Update(double dt)
{
	Vector3 viewVector = (target - position).Normalized();
	position += viewVector * (float)m_dSpeed * (float)dt;

	Constrain();

	/*if (position.z > 400.0f)
		target.z = position.z* -1;
	else if (position.z < -400.0f)
		target.z = position.z * -1;
		*/		
	 distance = (position - target).LengthSquared();
	 
	 bCube->SetPosition(Vector3(position.x, position.y - 5, position.z));

	/*if (distance > 100)
	{
		Vector3 direction = (moveto - position).Normalize();
		position = position + direction* (float)m_dSpeed * (float)dt;
	}
	else
	{
		arrived = true;
	}*/
}
void CEnemy::Constrain(void)
{
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x - 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);

	if (distance < 100)
	{
		target.Set(Math::RandIntMinMax(100, 200), 0.0f, Math::RandIntMinMax(100, 200));
	}
}
void CEnemy::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}
void CEnemy::SetSpeed(float speed)
{
	m_dSpeed = speed;
}
CEnemy* Create::Enemy(
	const Vector3& _position,
	const Vector3& _target,
	const float m_fSpeed, GroundEntity* m_pTerrain)
{
	CEnemy* result = new CEnemy();
	result->SetPos(_position);
	result->SetTarget(_target);
	result->SetCollider(true);
	result->SetSpeed(m_fSpeed);
	result->SetTerrain(m_pTerrain);
	result->Init();
	EntityManager::GetInstance()->AddEntity(result, true);
	return result;
}