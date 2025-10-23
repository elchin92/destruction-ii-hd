#include "ActiveMenuItem.h"

#ifndef __SelectKeyMenuItem__ 
#define __SelectKeyMenuItem__



class SelectKeyMenuItem : public ActiveMenuItem
{
private:
	__int64 BlinkStart;
	int		id, side, xAdjust;

	void InternalUpdateInput();
public:

	SelectKeyMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle, int iid, int iside);
	~SelectKeyMenuItem();

	void InternalBlit(int Selected);

	void CatchKey();
	void AbortCatchKey();

	int GetID();
};
#endif