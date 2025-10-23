// NumberedStaticMenuItem.cpp: implementation of the NumberedStaticMenuItem class.
//
//////////////////////////////////////////////////////////////////////
#include "MenuStyle.h"
#include "Game.h"
#include "NumberedStaticMenuItem.h"
#include "Debugger.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NumberedStaticMenuItem::NumberedStaticMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle, int iid){
	ActiveMenuItem::Initialize();
	TheMenuStyle=iTheMenuStyle;
	strcpy(Name,iTitle);
	id=iid;
}

NumberedStaticMenuItem::~NumberedStaticMenuItem()
{

}

void NumberedStaticMenuItem::InternalBlit(int Selected){
	Beacon(635434);
	TheMenuStyle->SetUnselectedStyle();
	TheGame->Text(x,y, Name);
}

int NumberedStaticMenuItem::GetID(){
	return id;
}

void NumberedStaticMenuItem::InternalUpdateInput(){}