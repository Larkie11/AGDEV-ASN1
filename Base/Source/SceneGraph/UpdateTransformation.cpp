#include "UpdateTransformation.h"


CUpdateTransformation::CUpdateTransformation()
	: curSteps(0)
	, deltaSteps(1)
	, minSteps(0)
	, maxSteps(0)
{
	Update_Mtx.SetToIdentity();
	Update_Mtx_REVERSED.SetToIdentity();
}


CUpdateTransformation::~CUpdateTransformation()
{
}

// Reset the transformation matrix to identity matrix
void CUpdateTransformation::Reset(void)
{
	Update_Mtx.SetToIdentity();
	Update_Mtx_REVERSED.SetToIdentity();
}

// Update the steps
void CUpdateTransformation::Update(void)
{
	curSteps += deltaSteps;
	if ((curSteps >= maxSteps) || (curSteps <= minSteps))
	{
		deltaSteps *= -1;
	}
}

// Apply a translation to the Update Transformation Matrix
void CUpdateTransformation::ApplyUpdate(const float dx, const float dy, const float dz)
{
	Update_Mtx.SetToTranslation(dx, dy, dz);
	Update_Mtx_REVERSED.SetToTranslation(-dx, -dy, -dz);
}
// Apply a rotation to the Update Transformation Matrix
void CUpdateTransformation::ApplyUpdate(const float angle, const float rx, const float ry, const float rz)
{
	Update_Mtx.SetToRotation(angle, rx, ry, rz);
	Update_Mtx_REVERSED.SetToRotation(-angle, rx, ry, rz);
}

// Set the minSteps and maxSteps
void CUpdateTransformation::SetSteps(const int minSteps, const int maxSteps)
{
	this->minSteps = minSteps;
	this->maxSteps = maxSteps;
}
// Get the minSteps and maxSteps
void CUpdateTransformation::GetSteps(int& minSteps, int& maxSteps)
{
	minSteps = this->minSteps;
	maxSteps = this->maxSteps;
}
// Get the direction of update
bool CUpdateTransformation::GetDirection(void) const
{
	if (deltaSteps == -1)
		return false;
	return true;
}
// Get the Update_Mtx
Mtx44 CUpdateTransformation::GetUpdateTransformation(void)
{
	if (deltaSteps == -1)
		return Update_Mtx_REVERSED;
	return Update_Mtx;
}
