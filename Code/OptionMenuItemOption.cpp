// OptionMenuItemOption.cpp: implementation of the OptionMenuItemOption class.
//
//////////////////////////////////////////////////////////////////////

#include "OptionMenuItemOption.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OptionMenuItemOption::OptionMenuItemOption(const char * iName, int iID)
{
	Name=iName;ID=iID;
	nextOption=0;lastOption=0;
}

OptionMenuItemOption::~OptionMenuItemOption()
{

}

const char * OptionMenuItemOption::GetName(){return Name;}
int OptionMenuItemOption::GetID(){return ID;}

