#pragma once

#include "WeaponInfo.h"

class CShotGun :
	public CWeaponInfo
{
public:
	CShotGun();
	virtual ~CShotGun();

	// Initialise this instance to default values
	void Init(void);
};

