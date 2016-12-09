#include "CLevelOfDetails.h"
#include "MeshBuilder.h"

/********************************************************************************
Constructor
********************************************************************************/
CLevelOfDetails::CLevelOfDetails(): modelMesh_HighDetails(NULL)
, modelMesh_MidDetails(NULL)
, modelMesh_LowDetails(NULL)
, m_bActive(false)
, theDetailLevel(HIGH_DETAILS)
{

}
/********************************************************************************
Destructor
********************************************************************************/
CLevelOfDetails
::~CLevelOfDetails()
{
	modelMesh_HighDetails = NULL;
	modelMesh_MidDetails =NULL;
	modelMesh_LowDetails =NULL;
}
/********************************************************************************  Initialise the LOD system
********************************************************************************/
bool CLevelOfDetails::InitLOD(const std::string& _meshName_High,
	const std::string& _meshName_Mid, const std::string& _meshName_Low)
{
	//Retrieve meshes from mesh builder's library and assign to the 3 mesh pointers
	modelMesh_HighDetails = MeshBuilder::GetInstance()->GetMesh(_meshName_High);
	modelMesh_MidDetails = MeshBuilder::GetInstance()->GetMesh(_meshName_Mid);
	modelMesh_LowDetails = MeshBuilder::GetInstance()->GetMesh(_meshName_Low);

	//If any not loaded, then return false and avoid setting the lod to active
	if ((modelMesh_HighDetails == nullptr) ||
		(modelMesh_MidDetails == nullptr) ||
		(modelMesh_LowDetails == nullptr))
		return false;

	SetLODStatus(true);
		return true;
}

Mesh* CLevelOfDetails::GetLODMesh(void) const
{
	if (theDetailLevel == HIGH_DETAILS)
		return modelMesh_HighDetails;

	if (theDetailLevel == MID_DETAILS)
		return modelMesh_MidDetails;

	if (theDetailLevel == LOW_DETAILS)
		return modelMesh_LowDetails;
}

int CLevelOfDetails::GetDetailLevel(void) const
{
	return theDetailLevel;
}

bool CLevelOfDetails::SetDetailLevel(const DETAIL_LEVEL theDetailLevel)
{
	if ((theDetailLevel >= NO_DETAILS) && (theDetailLevel < NUM_DETAIL_LEVEL))
	{
		this->theDetailLevel = theDetailLevel;
		return true;
	}
	return false;
}
void CLevelOfDetails::SetLODStatus(const bool bActive)
{
	m_bActive = bActive;
}
bool CLevelOfDetails::GetLODStatus(void) const
{
	return m_bActive;
}