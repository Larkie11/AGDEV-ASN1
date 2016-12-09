#include "Collider.h"


CCollider::CCollider()
	: maxAABB(Vector3( 0, 0, 0))
	, minAABB(Vector3( 0, 0, 0))
{
}


CCollider::~CCollider()
{
}

// Set the maxAABB and minAABB
void CCollider::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}
// Set the maxAABB
void CCollider::SetMaxAABB(Vector3 maxAABB)
{
	this->maxAABB = maxAABB;
}
// Get the maxAABB
Vector3 CCollider::GetMaxAABB(void)
{
	return maxAABB;
}
// Set the minAABB
void CCollider::SetMinAABB(Vector3 minAABB)
{
	this->minAABB = minAABB;
}
// Get the minAABB
Vector3 CCollider::GetMinAABB(void)
{
	return minAABB;
}
