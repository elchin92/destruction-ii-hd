// BorderMenuItem.cpp: implementation of the BorderMenuItem class.
//
//////////////////////////////////////////////////////////////////////
#include "MenuStyle.h"
#include "BorderMenuItem.h"
#include "Game.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BorderMenuItem::BorderMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle, int ixplacement){
	PassiveMenuItem::Initialize();
	TheMenuStyle=iTheMenuStyle;
	Title=iTitle;  xplacement=ixplacement;
}

void BorderMenuItem::SetEndPos(int iex, int iey){

	ex=iex; ey=iey; 
}

BorderMenuItem::~BorderMenuItem()
{

}

void BorderMenuItem::Blit(int Selected){

	TheMenuStyle->SetBorderStyle();
	RoundRect(TheGame->hdc,x,y, ex,ey, TheMenuStyle->GetBorderRound(),TheMenuStyle->GetBorderRound());
	DrawTitle();

}

void BorderMenuItem::DrawTitle(){

	if(TRUE||!strcmp(Title,"")){
		return;
		}

	TheMenuStyle->SetBorderTitleStyle();
	TheGame->Text(x+xplacement,y, Title);
}
int BorderMenuItem::IsBorder(){return 1;}