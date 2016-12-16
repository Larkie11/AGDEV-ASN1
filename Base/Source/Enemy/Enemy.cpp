#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MyMath.h"
#include "MeshBuilder.h"



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

	InitLOD("Head_high", "Head_mid", "Head_low");
	//Initialise the collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);
	up.Set(0.0f, 1.0f, 0.0f);
	baseMtx= new CUpdateTransformation();
	ef = new EnemyFollower();
	ez = new EnemyFollower();
	ea = new EnemyFollower();
}

CEnemy::~CEnemy()
{

}
void CEnemy::Init(const Vector3& _position,
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
}

void CEnemy::Init(void)
{
	//Set current values
	//position.Set(Math::RandIntMinMax(0, 10), 0.0f, Math::RandIntMinMax(0, 10));
	//target.Set(10.0f, 0.0f, 450.f);
	//moveto.Set(Math::RandIntMinMax(100, 200), 0.0f, Math::RandIntMinMax(100, 200));
	//Set boundary
	//enemyHand = new CEnemy();
	CSceneNode* newNode = CSceneGraph::GetInstance()->AddNode(ez);
	enemyNode = newNode->AddChild(this);
	ef->Init(position, target, m_dSpeed - 0.5, m_pTerrain, "Android_high", "Android_mid", "Android_low");
	ez->Init(position, target, m_dSpeed - 0.5, m_pTerrain, "Body_high", "Body_mid", "Body_low");
	ez->SetAABB(Vector3(5, 5, 5), Vector3(-5, -5, -5));
	ea->Init(position, target, m_dSpeed - 0.5, m_pTerrain, "robot1_high", "robot1_mid", "robot1_mid");
	node = enemyNode->AddChild(ef);
	node = enemyNode->AddChild(ea);
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
	if (target != position)
	{
		Vector3 viewVector = (target - position).Normalized();

		position += viewVector * (float)m_dSpeed * (float)dt;
	}
	//enemyHand->SetPos(position - 10);
	Constrain();	
	distance = (position - target).LengthSquared();
	ez->SetPos(Vector3(position.x, position.y - 5.5, position.z));
	ez->SetCollider(true);

	if (follow == 1)
	{
		ef->SetScale(Vector3(1, 1, 1));
		ef->SetCollider(true);
	}
	else
	{
		ef->SetScale(Vector3(0, 0, 0));
		ef->SetCollider(false);
	}

	if (follow >= 2)
	{
		ea->SetScale(Vector3(1, 1, 1));
		ea->SetCollider(true);
	}
	else
	{
		ea->SetScale(Vector3(0, 0, 0));
		ea->SetCollider(false);
	}

	ef->SetPos(Vector3(position.x - 10, position.y - 4, position.z - 10));
	ea->SetPos(Vector3(position.x + 10, position.y, position.z));

}
void CEnemy::SetNumOfFollowers(int followers)
{
	follow = followers;
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
		//target.Set(Math::RandIntMinMax(100, 200), 0.0f, Math::RandIntMinMax(100, 200));
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
	result->SetAABB(Vector3(2.5f, 1.5f, 2.5f), Vector3(-2.5f, -1.5f, -2.5f));
	result->SetSpeed(m_fSpeed);
	result->IsEnemy(true);
	result->SetTerrain(m_pTerrain);
	result->Init();
	EntityManager::GetInstance()->AddEntity(result, true);
	return result;
}