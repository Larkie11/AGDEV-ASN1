#include "Shotgun.h"


CShotGun::CShotGun()
{
}


CShotGun::~CShotGun()
{
}

// Initialise this instance to default values
void CShotGun::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 5;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 5;
	// The current total number of rounds currently carried by this player
	totalRounds = 20;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 20;

	// The time between shots
	timeBetweenShots = 1.4;
	// The elapsed time (between shots)
	elapsedTime = 1.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}
