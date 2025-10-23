#include "ActiveMenuItem.h"

#ifndef __SwitchMenuItem__ 
#define __SwitchMenuItem__

enum gState;

class SwitchMenuItem : public ActiveMenuItem
{
private:
	gState nextState;
	int SubMenu;
	void InternalUpdateInput();
public:

	SwitchMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle, gState inextState, int iSubMenu);
	~SwitchMenuItem();

	void InternalBlit(int Selected);


};
#endif