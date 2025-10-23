#include "ActiveMenuItem.h"

#ifndef __MarkableMenuItemMenuItem__ 
#define __MarkableMenuItemMenuItem__

class Menu;
class MarkableMenuItem : public ActiveMenuItem
{
private:
	int xm, ym, MARKED, id;
	Menu * MotherMenu;
	LPDIRECTDRAWSURFACE7 * MarkedSurface;
	void InternalUpdateInput();
public:

	MarkableMenuItem(Menu * iMotherMenu, const char * iTitle, int ixm, int iym, int iid, LPDIRECTDRAWSURFACE7 * mSurface);
	~MarkableMenuItem();

	void InternalBlit(int Selected);
	void Unmark();
	void Mark();

	int IsMarked();
	int GetID();

	MarkableMenuItem * nextMarked;
};
#endif