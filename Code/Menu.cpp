// Menu.cpp: implementation of the Menu class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "Menu.h"
#include "MenuStyle.h"

#include "NumericalSliderMenuItem.h"
#include "GraphicalSliderMenuItem.h"
#include "OptionMenuItem.h"
#include "SelectKeyMenuItem.h"
#include "SubscriptMenuItem.h"
#include "NumberedStaticMenuItem.h"
#include "SwitchMenuItem.h"
#include "MarkableMenuItem.h"
#include "BorderMenuItem.h"
#include "BorderEndMenuItem.h"
#include "Debugger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


Menu::Menu(MenuStyle * iSisterStyle)
{
	Initialize();
	SisterStyle=iSisterStyle;
	TheMenuStyle = new MenuStyle(this, SisterStyle);

}




Menu::~Menu()
{
	SAFE_DELETE(TheMenuStyle);
	KillEntireCircularLinkedList(MenuItem, TheMenuItems, nextMenuItem)
}

void Menu::Initialize(){
	MultipleMarks=FALSE;
	LastAddedActiveItem=NULL;
	TheActiveMenuItems=NULL;
	TheMenuItems=NULL;
	TheMenuStyle=NULL;
	LastAddedOption=FALSE;
	TheMarkableItems=NULL;
	SelectedItem=NULL;
	pSelectedItem=&SelectedItem;
}

void Menu::UpdateInput(){
	if((*pSelectedItem))
		(*pSelectedItem)=(*pSelectedItem)->UpdateInput();
}


void Menu::SetPos(int X, int Y){
	MenuPos[0]=X;
	MenuPos[1]=Y;
}

MenuItem * Menu::GetMenuItems(){return TheMenuItems;}

void Menu::ValidateData(int LockType){
	TheActiveMenuItems->ValidateDataLoop(LockType);	
}

ActiveMenuItem * Menu::GetChoice(){
	return (*pSelectedItem);
}



MarkableMenuItem * Menu::GetFirstMarked(){return TheMarkableItems;}
int Menu::GetXPos(){return MenuPos[0];}
int Menu::GetYPos(){return MenuPos[1];}



void Menu::CreatePredefinedMenu(int left, int top, int right, int bottom, PredefinedStyle pdm){

	MenuPos[0]=left;
	MenuPos[1]=top;

	DP2("Menu X", MenuPos[0]);

	Width=right-left;
	Height=bottom-top;

		Beacon(25524);
	TheMenuStyle->CreatePredefinedStyle(Width, Height, pdm);
		Beacon(25525);
	SetNeighbours(TheMenuStyle->GetColumns());
		Beacon(25526);
	DP2("Menu X", MenuPos[0]);
}

void Menu::BlitMenu(){

	Beacon(123454321);
	TheGame->BlankArea(MenuPos[0],MenuPos[1],Width,Height);
	Beacon(1234543211);
	if (TheGame->g_pDDSBack->GetDC(&TheGame->hdc) == DD_OK){
			Beacon(123454322);
		TheMenuStyle->SetTextAttributes();
			Beacon(123454323);
		int i=0;
		for(MenuItem * mi=TheMenuItems;mi;mi=mi->nextMenuItem){
			Beacon(1230000+i++);
			mi->Blit(mi==(*pSelectedItem));
		}
	}	TheGame->g_pDDSBack->ReleaseDC(TheGame->hdc);
		Beacon(12437343);
}

void Menu::LISetSelection(int iSelection){

	LastAddedActiveItem->SetSelection(iSelection);
	if(	LastAddedOption){
		((OptionMenuItem *)LastAddedActiveItem)->SetOptionSelection(iSelection);
	}
}

void Menu::LISetUnlimitedZero(){
	LILock(LOCK_UNLIMITEDZERO);
}

void Menu::LILock(int LockType){
	LastAddedActiveItem->Lock(LockType);
}

MenuStyle * Menu::GetMenuStyle(){return TheMenuStyle;}


ActiveMenuItem * Menu::AddMenuItem(MenuItem * mi, int IsOption){
	LastAddedOption=IsOption;
	AddLastToLinkedList(MenuItem, TheMenuItems, nextMenuItem, mi);
	if(mi->IsActive()){
		if(!LastAddedActiveItem){
			(*pSelectedItem)=((ActiveMenuItem *)mi);
		}
		LastAddedActiveItem=((ActiveMenuItem *)mi);
		AddLastToLinkedList(ActiveMenuItem, TheActiveMenuItems, nextActiveMenuItem, LastAddedActiveItem);
		return ((ActiveMenuItem *)mi);
	}
	return NULL;
}

void Menu::SetSelections(){
	for(ActiveMenuItem * ami=TheActiveMenuItems;ami;ami=ami->nextActiveMenuItem){
		ami->SetSelection(ami->GetInternalSelection());
	}
}

void Menu::SetNeighbours(int Columns){
	ActiveMenuItem ** ActiveRowAbove = new ActiveMenuItem*[Columns];
	ActiveMenuItem * ActiveLeftItem = NULL;
	for(int i=0;i<Columns;i++)
		ActiveRowAbove[i] = NULL;

	MenuItem * CurrentMenuItem=TheMenuItems;

	MenuItem * temp; // C++20: declare before loop so it's accessible after
	for(temp=TheMenuItems;temp->IsBorder();temp=temp->nextMenuItem);

	for(int x=0;CurrentMenuItem;x++, CurrentMenuItem=CurrentMenuItem->nextMenuItem){
		if(CurrentMenuItem->IsBorder()){
			x--;
		}else{

			if(x%Columns==0&&x>0){
				// New row begun
				for(int a=0;a<Columns&&temp;a++){
					if(temp->IsActive())
						ActiveRowAbove[a]=(ActiveMenuItem *)temp;
					temp = temp->nextMenuItem;

				}
				// Have to have this because of borderitems!!!!
				temp = CurrentMenuItem;
				//for(temp = CurrentMenuItem;temp&&!temp->IsActive();temp=temp->nextMenuItem())

				ActiveLeftItem=NULL;
			}


			if(CurrentMenuItem->IsActive()){
				ActiveMenuItem * cami = (ActiveMenuItem *)CurrentMenuItem;
				if(ActiveLeftItem){
					cami->SetLeftNeighbour(ActiveLeftItem);
					ActiveLeftItem->SetRightNeighbour(cami);
				}
				if(ActiveRowAbove[x%Columns]){
					cami->SetUpNeighbour(ActiveRowAbove[x%Columns]);
					ActiveRowAbove[x%Columns]->SetDownNeighbour(cami);			
				}
				ActiveLeftItem=cami;

			}
		}



	}
	// Correct?
	delete[] ActiveRowAbove;
}

int Menu::GetWidth(){
	return Width;
}
int Menu::GetHeight(){
	return Height;
}



ActiveMenuItem * Menu::AddNumericalSlider(const char * SliderTitle,int Start, int Min,int Max, int Step, char * unit, int pre){
	return AddMenuItem(new NumericalSliderMenuItem(TheMenuStyle,SliderTitle,Start,Min,Max,Step, unit, pre));
}
ActiveMenuItem * Menu::AddNumericalSliderWPM(const char * SliderTitle,int Start, int Min, int * MaxPointer, int Step){
	return AddMenuItem(new NumericalSliderMenuItem(TheMenuStyle,SliderTitle,Start,Min,MaxPointer,Step));
}
ActiveMenuItem * Menu::AddGraphicalSlider(const char * SliderTitle,int Start, int Max){
	return AddMenuItem(new GraphicalSliderMenuItem(TheMenuStyle,SliderTitle,Start,Max));
}
ActiveMenuItem * Menu::AddSwitch(const char * SwitchTitle, gState nextState,int SubMenu){
	return AddMenuItem(new SwitchMenuItem(TheMenuStyle,SwitchTitle,nextState,SubMenu));
}
ActiveMenuItem * Menu::AddSelectKey(const char * KeyName, int iid, int iside){
	return AddMenuItem(new SelectKeyMenuItem(TheMenuStyle,KeyName,iid, iside));
}
ActiveMenuItem * Menu::AddNumberedStaticItem(const char * ItemName, int id){
	return AddMenuItem(new NumberedStaticMenuItem(TheMenuStyle,ItemName,id));
}
ActiveMenuItem * Menu::AddOptionItem(const char * OptionItemName){
	return AddMenuItem(new OptionMenuItem(TheMenuStyle,OptionItemName),TRUE);
}
ActiveMenuItem * Menu::AddMarkableItem(const char * Title, int mx, int my, int mid, LPDIRECTDRAWSURFACE7 * ms){
	return AddMenuItem(new MarkableMenuItem(this,Title, mx, my, mid, ms));
}
void Menu::LIAddOption(const char * OptionName, int SetStandard){
	// Warning: Make sure last added item is a OptionMenuItem!
	((OptionMenuItem *)LastAddedActiveItem)->AddOption(OptionName,SetStandard);
}


void Menu::AddStaticItem(const char * Title){
	AddMenuItem(new SubscriptMenuItem(TheMenuStyle,Title));	
}
BorderMenuItem * Menu::StartNewBorder(const char * iTitle){
	LastBorder=new BorderMenuItem(TheMenuStyle,iTitle);
	AddMenuItem(LastBorder);
	return LastBorder;
}
BorderEndMenuItem * Menu::EndBorder(){	
	BorderEndMenuItem * EndBorder = new BorderEndMenuItem(LastBorder);
	AddMenuItem(EndBorder);
	return EndBorder;
}

ActiveMenuItem * Menu::GetActiveMenuItem(int no){
	ActiveMenuItem * ami=TheActiveMenuItems;
	for(int i=0;i<no&&ami;ami=ami->nextActiveMenuItem,i++);
	return ami;
}

int Menu::GetNextMenuStart(){


	return TheMenuStyle->GetNextMenuYPos();
}
void Menu::SetChoice(ActiveMenuItem * newChoice){(*pSelectedItem)=newChoice;}
ActiveMenuItem ** Menu::GetMotherInput(){return pSelectedItem;}
void Menu::SetMotherInput(ActiveMenuItem ** MotherpSelectedItem){pSelectedItem=MotherpSelectedItem;}
void Menu::SetMultipleMarks(){MultipleMarks=TRUE;}
int Menu::GetMultipleMarks(){return MultipleMarks;}
