#pragma once
#include "Vector3.h"
#include "Mesh.h"

class CLevelOfDetails
{
public:
	Mesh* modelMesh_HighDetails;
	Mesh* modelMesh_MidDetails;
	Mesh* modelMesh_LowDetails;

	enum DETAIL_LEVEL
	{
		NO_DETAILS = 0,
		HIGH_DETAILS,
		MID_DETAILS,
		LOW_DETAILS,
		NUM_DETAIL_LEVEL,
	};

	CLevelOfDetails();
	virtual ~CLevelOfDetails();

	bool InitLOD(const std::string& _meshName_High,
		const std::string& _meshName_Mid,
		const std::string& _meshName_Low);

	bool DestroyLOD(void);
	void SetLODStatus(const bool bActive);
	bool GetLODStatus(void)const;
	bool SetLODMesh(Mesh* theMesh, const DETAIL_LEVEL theDetailLevel);
	Mesh* GetLODMesh(void) const;
	int GetDetailLevel(void) const;
	bool SetDetailLevel(const DETAIL_LEVEL theDetailLevel);

protected:
	bool m_bActive;
	DETAIL_LEVEL theDetailLevel;
};