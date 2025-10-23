// SubscriptMenuItem.cpp: implementation of the SubscriptMenuItem class.
//
//////////////////////////////////////////////////////////////////////
#include "MenuStyle.h"
#include "Game.h"
#include "SubscriptMenuItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SubscriptMenuItem::SubscriptMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle){
	PassiveMenuItem::Initialize();
	TheMenuStyle=iTheMenuStyle;
	Title=iTitle;
}

SubscriptMenuItem::~SubscriptMenuItem()
{

}

void SubscriptMenuItem::Blit(int Selected){
	TheMenuStyle->SetDiscreteStyle();
	TheGame->Text(x,y, Title);
}