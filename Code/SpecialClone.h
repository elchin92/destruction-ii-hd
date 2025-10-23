#ifndef __SpecialClone__ 
#define __SpecialClone__

#include "Special.h"



class SpecialClone : public Special
{
private:

public:
	SpecialClone(Player * theActivator);
	int Activate();
};
#endif

