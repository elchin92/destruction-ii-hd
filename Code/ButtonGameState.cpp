// ButtonGameState.cpp: implementation of the ButtonGameState class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "Menu.h"
#include "ButtonGameState.h"
#include "ButtonMenuItem.h"
#include "InfoBox.h"
#include "Debugger.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void ButtonGameState::Blit(){

	BlitButtonMenu();
	InternalBlit();
}

void ButtonGameState::Initialize(){
	FadingGameState::Initialize();
	ButtonMenu=NULL;

}
void ButtonGameState::BlitButtonMenu(){
	if(!ButtonMenu)
		return;
	Beacon(2143124);
	TheGame->BlitOntoBack(BackgroundSurface);
	Beacon(15315234);
	for(ActiveMenuItem * ami=ButtonMenu->GetActiveMenuItem(0);ami;ami=ami->nextActiveMenuItem){
		ami->Blit(ami==ButtonMenu->GetChoice());
	}
	Beacon(1345132);
}

ButtonGameState::~ButtonGameState(){
	SAFE_DELETE(ButtonMenu);
	// NOTE: UserMsgBox is deleted by base class GameState::~GameState()
	// Deleting it here causes double-free crash!
}

