#ifndef __SpecialTank__ 
#define __SpecialTank__

#include "Special.h"


class SpecialTank : public Special
{
private:

public:
	SpecialTank(Player * theActivator);
	int Activate();
	int ActivateIt(int Type);
};
#endif

