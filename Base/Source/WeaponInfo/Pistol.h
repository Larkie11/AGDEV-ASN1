#pragma once

#include "WeaponInfo.h"

class CPistol :
	public CWeaponInfo
{
public:
	CPistol();
	virtual ~CPistol();

	// Initialise this instance to default values
	void Init(void);
};

