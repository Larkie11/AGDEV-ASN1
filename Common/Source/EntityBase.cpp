#include "EntityBase.h"

EntityBase::EntityBase() 
	: position(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f)
	, rotation(0.0f, 0.0f, 0.0f)
	, isDone(false)
	, m_bCollider(false)
	, bLaser(false)
{
}

EntityBase::~EntityBase()
{
}
bool EntityBase::GetIsEnemy()
{
	return isEnemy;
}
void EntityBase::IsEnemy(bool enemy)
{
	isEnemy = enemy;
}
void EntityBase::Update(double _dt)
{
}

void EntityBase::Render()
{
}

void EntityBase::RenderUI()
{
}

bool EntityBase::IsDone()
{
	return isDone;
}

void EntityBase::SetIsDone(bool _value)
{
	isDone = _value;
}

// Check if this entity has a collider class parent
bool EntityBase::HasCollider(void) const
{
	return m_bCollider;
}
std::string EntityBase::GetIsCamp()
{
	return isCamp;
}
void EntityBase::IsCamp(std::string iscamp)
{
	isCamp = iscamp;
}
// Set the flag to indicate if this entity has a collider class parent
void EntityBase::SetCollider(const bool _value)
{
	m_bCollider = _value;
}

// Set the flag, bLaser
void EntityBase::SetIsLaser(const bool bLaser)
{
	this->bLaser = bLaser;
}

// Get the flag, bLaser
bool EntityBase::GetIsLaser(void) const
{
	return bLaser;
}