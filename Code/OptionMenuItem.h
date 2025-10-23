#include "ActiveMenuItem.h"

#ifndef __OptionMenuItem__ 
#define __OptionMenuItem__

class OptionMenuItemOption;

class OptionMenuItem : public ActiveMenuItem
{
private:
	OptionMenuItemOption * CurrentOption;
	OptionMenuItemOption * FirstOption; // Only used for building structure

	int LastId;

	void InternalUpdateInput();
	void SwitchOption();

public:

	OptionMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle);
	~OptionMenuItem();

	void AddOption(const char * iName, int StandardOption);
	void InternalBlit(int Selected);
	void SetOptionSelection(int newSelection);

	int IsOption();
};
#endif