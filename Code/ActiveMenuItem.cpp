// ActiveMenuItem.cpp: implementation of the ActiveMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#include "OptionMenuItem.h"
#include "InputEngine.h"
#include "Game.h"
#include "MenuStyle.h"
#include "Debugger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void ActiveMenuItem::Initialize()
{
	MenuItem::Initialize();
	StatusBits=0;
	ReturnNext=FALSE;
	InternalSelection=0;
	ExternalSelection=0;
	MoveToNeighbours=TRUE;
	CatchingEscape=FALSE;

	LeftNeighbour=this;
	RightNeighbour=this;
	UpNeighbour=this;
	DownNeighbour=this;
	nextActiveMenuItem=NULL;
}

ActiveMenuItem::~ActiveMenuItem()
{

}

int ActiveMenuItem::IsActive(){return 1;}


int * ActiveMenuItem::GetISP(){return &InternalSelection;}

ActiveMenuItem * ActiveMenuItem::UpdateInput(){

	Beacon(355236);
	if(MoveToNeighbours){
		Beacon(355237);
		if(TrueDown(DIK_LEFT)){
			return LeftNeighbour;
		}else if(TrueDown(DIK_RIGHT)){

			return RightNeighbour;
		}else if(TrueDown(DIK_UP)){
			return UpNeighbour;
		}else if(TrueDown(DIK_DOWN)){
			return DownNeighbour;
		}
			Beacon(355238);
	}
		
	if(Unlocked(TrueDown(DIK_RETURN))){
		Beacon(355239);
		InternalUpdateInput();
	}


	if(ReturnNext){
		ReturnNext=FALSE;
		Beacon(355237);
		if(nextActiveMenuItem)
			return nextActiveMenuItem;
	}
	Beacon(355237);
	return this;
}

int ActiveMenuItem::CatchesEscape(){return CatchingEscape;}

void ActiveMenuItem::SetSelection(int newSelection){
	if(IsOption())
		((OptionMenuItem *)this)->SetOptionSelection(newSelection);
	InternalSelection=newSelection;
	if(!   (StatusBits&(LOCK_TAKEEFFECTNEXTGAME|LOCK_TAKEEFFECTNEXTROUND) ))
		ExternalSelection=newSelection;
}
void ActiveMenuItem::AddToSelection(int addition){
	SetSelection(InternalSelection+addition);
}
int ActiveMenuItem::GetInternalSelection(){
	return InternalSelection;
}
int ActiveMenuItem::GetExternalSelection(){
	return ExternalSelection;
}

int ActiveMenuItem::Unlocked(int DisplayMessage){
	if(StatusBits&LOCK_NEEDTOREGISTER){
		if(DisplayMessage){
			cgs->Message("This option is only available in the full version",mcRed);		
			Beep(500,200);
		}
		return 0;
	}
	if(StatusBits&LOCK_NOTAVAILABLEINTHISVERSION){
		if(DisplayMessage)
			cgs->Message("Feature is not included in this version",mcGreen);		
		return 0;
	}
	if(TheGame->GAMEACTIVE){

		if(StatusBits&LOCK_NOTWHILEGAMEACTIVE){
			if(DisplayMessage)
				cgs->Message("You can't do this while a game is running",mcRed);	
			return 0;
		}
		if(StatusBits&LOCK_TAKEEFFECTNEXTGAME){
			if(DisplayMessage)
				cgs->Message("Changes will take effect next game",mcGreen);		
			return 1;
		}
		if(StatusBits&LOCK_TAKEEFFECTNEXTROUND){
			if(DisplayMessage)
				cgs->Message("Changes will take effect next round",mcGreen);	
			return 1;
		}
	}
	return 1;
}

void ActiveMenuItem::Lock(int LockType){
	StatusBits=StatusBits|LockType;
}
void ActiveMenuItem::ValidateDataLoop(int LockType){
	if(StatusBits&LockType)
		ExternalSelection=InternalSelection;
	if(nextActiveMenuItem)
		nextActiveMenuItem->ValidateDataLoop(LockType);
}

void ActiveMenuItem::SetLeftNeighbour(ActiveMenuItem * l){LeftNeighbour=l;}
void ActiveMenuItem::SetRightNeighbour(ActiveMenuItem * r){RightNeighbour=r;}
void ActiveMenuItem::SetUpNeighbour(ActiveMenuItem * u){UpNeighbour=u;}
void ActiveMenuItem::SetDownNeighbour(ActiveMenuItem * d){DownNeighbour=d;}

void ActiveMenuItem::Blit(int Selected){

	if(Selected&&TheMenuStyle)
		TheMenuStyle->BlitMark(this,Selected);
	InternalBlit(Selected);
}

int ActiveMenuItem::IsLocked(int LockType){
	return LockType&StatusBits;
}
void ActiveMenuItem::ActivateNeighbourLinkage(){
	if(UpNeighbour!=this)
		UpNeighbour->SetDownNeighbour(this);
	if(DownNeighbour!=this)
		DownNeighbour->SetUpNeighbour(this);
	if(LeftNeighbour!=this)
		LeftNeighbour->SetRightNeighbour(this);
	if(RightNeighbour!=this)
		RightNeighbour->SetLeftNeighbour(this);
}


ActiveMenuItem * ActiveMenuItem::GetLeftNeighbour(){return LeftNeighbour;}
ActiveMenuItem * ActiveMenuItem::GetRightNeighbour(){return RightNeighbour;}
ActiveMenuItem * ActiveMenuItem::GetUpNeighbour(){return UpNeighbour;}
ActiveMenuItem * ActiveMenuItem::GetDownNeighbour(){return DownNeighbour;}