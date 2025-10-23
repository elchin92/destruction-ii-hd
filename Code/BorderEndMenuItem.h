#include "PassiveMenuItem.h"

#ifndef __BorderEndMenuItem__ 
#define __BorderEndMenuItem__


class BorderMenuItem;

class BorderEndMenuItem : public PassiveMenuItem
{
private:
	BorderMenuItem * TheBorder;

public:

	BorderEndMenuItem(BorderMenuItem * iTheBorder);
	~BorderEndMenuItem();

	void Blit(int Selected);
	void SetBottomRightCorner(int ix, int iy);
	int IsBorder();
	int IsEndBorder();
};
#endif