#ifndef GROUND_ENTITY_H
#define GROUND_ENTITY_H

#include "EntityBase.h"
#include "Vector3.h"
#include <string>

class Mesh;

class GroundEntity : public EntityBase
{
public:
	GroundEntity(Mesh* _modelMesh1, Mesh* _modelMesh2);
	virtual ~GroundEntity();

	virtual void Update(double _dt);
	virtual void Render();

	inline void SetPosition(const Vector3& _value){ position = _value; };
	inline Vector3 GetPosition(){ return position; };

	float GetTerrainHeight(Vector3 position);

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

	// size is included for completeness. we assume that the mesh has a size of 1.0X1.0 square units
	// use scale to change the ground texture sizes
	inline void SetSize(const Vector3& _value) { size = _value; };
	inline Vector3 GetSize(){ return size; };

	inline void SetGrids(const Vector3& _value) { grids = _value; };
	inline Vector3 GetGrids(){ return grids; };

	Vector3 GetMaxBoundary(void);
	Vector3 GetMinBoundary(void);

private:
	Vector3 position;
	Vector3 scale;
	Vector3 size;
	Vector3 grids;
	Vector3 maxBoundary;
	Vector3 minBoundary;
	bool m_bMaxBoundaryDefined;
	bool m_bMinBoundaryDefined;
	Mesh* modelMesh[2];
};

namespace Create
{
	GroundEntity* Ground(const std::string& _meshName1, const std::string& _meshName2);
};

#endif // GROUND_ENTITY_H