#include "ActiveMenuItem.h"

#ifndef __NumberedStaticMenuItemMenuItem__ 
#define __NumberedStaticMenuItemMenuItem__


class NumberedStaticMenuItem : public ActiveMenuItem
{
private:
	void InternalUpdateInput();
	int id;
	char Name[100];
public:

	NumberedStaticMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle, int iid);
	~NumberedStaticMenuItem();

	void InternalBlit(int Selected);

	int GetID();
};
#endif
