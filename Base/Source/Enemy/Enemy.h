#pragma once

#include "../GenericEntity.h"
#include "../GroundEntity.h"

class Mesh;

class CEnemy :
	public GenericEntity
{
protected:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;
	Vector3 moveto;
	bool arrived;
	float distance;

public:
	CEnemy(void);
	virtual ~CEnemy();
	void Init(void);
	void Reset(void);

	//Set position
	void SetPos(const Vector3& pos);
	void SetTarget(const Vector3& target);
	//Set up
	void SetUp(const Vector3& up);
	void SetBoundary(Vector3 max, Vector3 min);
	void SetTerrain(GroundEntity* m_pTerrain);
	void SetSpeed(float speed);
	//Get position
	Vector3 GetPos(void) const;
	//Get target
	Vector3 GetTarget(void) const;
	//Get up
	Vector3 GetUp(void) const;
	//Get terrain for player info
	GroundEntity* GetTerrain(void);
	GenericEntity* bCube;
	//Update
	void Update(double dt = 0.0333f);
	//Constrain position within borders
	void Constrain(void);
	void Render(void);
};

namespace Create
{
	CEnemy* Enemy(
		const Vector3& _position,
		const Vector3& _target,
		const float m_fSpeed, GroundEntity* m_pTerrain);
};