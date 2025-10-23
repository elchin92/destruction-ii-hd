#include "PassiveMenuItem.h"

#ifndef __SubscriptMenuItemMenuItem__ 
#define __SubscriptMenuItemMenuItem__


class SubscriptMenuItem : public PassiveMenuItem
{
private:

public:

	SubscriptMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle);
	~SubscriptMenuItem();

	void Blit(int Selected);
};
#endif