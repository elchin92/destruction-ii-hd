#ifndef __OptionMenuItemOption__ 
#define __OptionMenuItemOption__


class OptionMenuItemOption
{
private:

	const char * Name;
	int ID;
public:

	OptionMenuItemOption(const char * iName, int iID);
	~OptionMenuItemOption();

	const char * GetName();
	int GetID();

	OptionMenuItemOption * nextOption;
	OptionMenuItemOption * lastOption;
};
#endif