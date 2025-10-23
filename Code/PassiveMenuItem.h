#include "MenuItem.h"

#ifndef __PassiveMenuItemMenuItem__ 
#define __PassiveMenuItemMenuItem__


class PassiveMenuItem : public MenuItem
{
private:

public:

	void Initialize();
	virtual ~PassiveMenuItem();
	int IsActive();


};
#endif