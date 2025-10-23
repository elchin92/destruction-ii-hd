#ifndef __SpecialClusterBomb__ 
#define __SpecialClusterBomb__

#include "Special.h"

class SpecialClusterBomb : public Special
{
private:
	__int64 ClusterStart;
	int ClusterPos[2],Booms,Ms;

public:

	SpecialClusterBomb(Player * theActivator);
	int IsActive();
	void Fire(long TheFrame);
	int Activate();
	void InActivate();
	int * GetPos();
	__int64 GetTime();
};
#endif

