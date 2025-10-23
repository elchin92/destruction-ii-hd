#include "PassiveMenuItem.h"

#ifndef __BorderMenuItem__ 
#define __BorderMenuItem__


class BorderMenuItem : public PassiveMenuItem
{
private:
	int ex,ey, bround, xplacement;

	void SetBorderPos(int iwidth, int iheight);
public:

	BorderMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle=0, int ixplacement=0);
	~BorderMenuItem();

	void SetEndPos(int iex, int iey);
	void Blit(int Selected);
	void DrawTitle();
	int IsBorder();

};
#endif