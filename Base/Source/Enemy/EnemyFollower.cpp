#include "EnemyFollower.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MyMath.h"
#include "MeshBuilder.h"



EnemyFollower::EnemyFollower() : GenericEntity(NULL)
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

	this->InitLOD("Android_high", "Android_mid", "Android_mid");
	//Initialise the collider
	this->SetCollider(true);
	this->SetAABB(Vector3(3, 3, 3), Vector3(-3, -3, -3));
	this->IsEnemy(true);
	position.SetZero();
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);
	up.Set(0.0f, 1.0f, 0.0f);
	baseMtx= new CUpdateTransformation();
}

EnemyFollower::~EnemyFollower()
{

}
void EnemyFollower::Init(const Vector3& _position,
	const Vector3& _target,
	const float m_fSpeed, GroundEntity* m_pTerrain, string High, string mid, string low)
{
	SetPos(_position);
	SetTarget(_target);
	SetCollider(true);
	SetSpeed(m_fSpeed);
	SetTerrain(m_pTerrain);
	IsEnemy(true);
	InitLOD(High, mid, low);
	EntityManager::GetInstance()->AddEntity(this, true);
}

void EnemyFollower::Init(void)
{
	//Set current values
	//position.Set(Math::RandIntMinMax(0, 10), 0.0f, Math::RandIntMinMax(0, 10));
	//target.Set(10.0f, 0.0f, 450.f);
	//moveto.Set(Math::RandIntMinMax(100, 200), 0.0f, Math::RandIntMinMax(100, 200));
	//Set boundary

	//enemyNode= CSceneGraph::GetInstance()->AddNode(this);
	//enemyHand->SetPos(position-10);
	//enemyHand->SetTarget(this->target);
	//enemyHand->SetCollider(true);
	//enemyHand->IsEnemy(true);
	//enemyHand->SetAABB(Vector3(2.f, 2.f, 1.f), Vector3(-2.f, -2.f, -2.f));
	//enemyHand->SetSpeed(this->m_dSpeed);
	//enemyHand->SetTerrain(this->m_pTerrain);
	//enemyHand->InitLOD("Android_high", "Android_mid", "Android_mid");
	//CSceneNode* node = enemyNode->AddChild(enemyHand);
	//EntityManager::GetInstance()->AddEntity(enemyHand, true);

	//bCube = Create::Entity("Hand", Vector3(0, 0, position.z));
	//bCube->SetCollider(true);
	//bCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	//node = enemyNode->AddChild(bCube);
	//aRotateMtx = new CUpdateTransformation();
	//aRotateMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	//aRotateMtx->SetSteps(-200, 10);
	////node->SetUpdateTransformation(aRotateMtx);
	//bCube->InitLOD("Hand", "lightball", "cubeSG");
}


void EnemyFollower::Reset(void)
{
	//Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void EnemyFollower::SetPos(const Vector3& pos)
{
	position = pos;
}

void EnemyFollower::SetTarget(const Vector3& target)
{
	this->target = target;
}

void EnemyFollower::SetUp(const Vector3& up)
{
	this->up = up;
}
void EnemyFollower::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
void EnemyFollower::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;
		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}
Vector3 EnemyFollower::GetPos(void) const
{
	return position;
}
Vector3 EnemyFollower::GetTarget(void) const
{
	return target;
}
Vector3 EnemyFollower::GetUp(void) const
{
	return up;
}
GroundEntity* EnemyFollower::GetTerrain(void)
{
	return m_pTerrain;
}
void EnemyFollower::Update(double dt)
{
	//Vector3 viewVector = (target - position).Normalized();
	//position += viewVector * (float)m_dSpeed * (float)dt;

	//enemyHand->SetPos(position - 10);
	//Constrain();	
	//distance = (position - target).LengthSquared();
	
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
void EnemyFollower::Constrain(void)
{
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x - 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	//if (position.y != m_pTerrain->GetTerrainHeight(position))
		//position.y = m_pTerrain->GetTerrainHeight(position);

	if (distance < 100)
	{
		target.Set(Math::RandIntMinMax(100, 200), 0.0f, Math::RandIntMinMax(100, 200));
	}
}
void EnemyFollower::Render(void)
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
void EnemyFollower::SetSpeed(float speed)
{
	m_dSpeed = speed;
}