#ifndef __SpecialAirStrike__ 
#define __SpecialAirStrike__

#include "Special.h"


class SpecialAirStrike : public Special
{
private:
	__int64 AirStart;
	int AirDropPos[2], AirFreePos[2], AirPos[2], AirStartPos, AirBombs;
public:
	SpecialAirStrike(Player * theActivator);
	int * GetPos();
	int IsActive();
	int Activate();
	void InActivate();
	void Fire();
	__int64 GetTime();
};
#endif

