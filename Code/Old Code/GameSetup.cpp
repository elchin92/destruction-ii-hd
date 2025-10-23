#include "GameSetup.h"
#include "Game.h"
#include "Player.h"
#include "Menu.h"
#include "PCSetup.h"
#include "ActiveMenuItem.h"
//#include "Settings.h"
#include "InputEngine.h"

#define EdgeDist	10
#define BreakPoint	150


// Not working
GameSetup * TheGameSetup;


GameSetup::GameSetup(){

	TheGame->Beacon(16600);
	cgt=gtOneOnOne;
	PlayersPointer=ThePCSetup->GetNumberOfPlayersPointer();
	TheGame->Beacon(16601);
	static int Groups=0; // Actual groups counter
	GroupsPointer=&Groups;
	TheGame->Beacon(16602);
	UpdateGroups();
	TheGame->Beacon(16603);

	EnemySetupMenu = new Menu();
	TheGame->Beacon(16604);
	EnemySetupMenu->StartNewBorder("Enemy Setup");
	EnemySetupMenu->AddStaticItem("Type");
	EnemySetupMenu->AddStaticItem("Droids");
	EnemySetupMenu->AddStaticItem("Tanks");
	EnemySetupMenu->AddStaticItem("Cars");
	EnemySetupMenu->AddStaticItem("Machines");
	TheGame->Beacon(16605);
	EnemySetupMenu->AddStaticItem("BaseCount");
	for(int et=0;et<eTypes;et++)
		EnemySettings[et][esBC]=EnemySetupMenu->AddNumericalSlider("",EnemyDefaultSettings[et][esBC],1,100,1);

	EnemySetupMenu->AddStaticItem("Frequency");
	for(et=0;et<eTypes;et++){
		EnemySettings[et][esFQ]=EnemySetupMenu->AddOptionItem();
		EnemySetupMenu->LIAddOption("Very Low");
		EnemySetupMenu->LIAddOption("Low");
		EnemySetupMenu->LIAddOption("Medium");
		EnemySetupMenu->LIAddOption("High");
		EnemySetupMenu->LIAddOption("Very High");
		EnemySetupMenu->LISetSelection(EnemyDefaultSettings[et][esFQ]);
	}
	TheGame->Beacon(16606);
	EnemySetupMenu->AddStaticItem("Upgrade Likeliness");
	for(et=0;et<eTypes;et++){
		EnemySettings[et][esUL]=EnemySetupMenu->AddOptionItem();
		EnemySetupMenu->LIAddOption("Very Low");
		EnemySetupMenu->LIAddOption("Low");
		EnemySetupMenu->LIAddOption("Medium");
		EnemySetupMenu->LIAddOption("High");
		EnemySetupMenu->LIAddOption("Very High");
		EnemySetupMenu->LISetSelection(EnemyDefaultSettings[et][esUL]);
	}

	EnemySetupMenu->EndBorder();
	TheGame->Beacon(16607);
	for(int a=0;a<eTypes;a++){
		for(int b=0;b<eSettings;b++){	
			EnemySettings[a][b]->Lock(LOCK_TAKEEFFECTNEXTROUND);
		}
	}

	EnemySetupMenu->CreatePredefinedMenu(EdgeDist,EdgeDist,800-EdgeDist,BreakPoint-EdgeDist,pEnemySetup);
		TheGame->Beacon(16608);
	GameSetupMenus[gtOneOnOne] = new Menu(EnemySetupMenu->GetMenuStyle());
	SwitchMenuItems[gtOneOnOne]=GameSetupMenus[gtOneOnOne]->AddOptionItem("Game Type");
	GameSetupMenus[gtOneOnOne]->LIAddOption("1 on 1");
	GameSetupMenus[gtOneOnOne]->LIAddOption("Tournament");
	GameSetupMenus[gtOneOnOne]->LIAddOption("League");
	GameSetupMenus[gtOneOnOne]->LIAddOption("Cup");
	GameSetupMenus[gtOneOnOne]->LILock(LOCK_TAKEEFFECTNEXTGAME);

	GameSetupMenus[gtOneOnOne]->AddNumericalSlider("Time Limit",0,1,99,""," minutes");
	GameSetupMenus[gtOneOnOne]->LILock(LOCK_TAKEEFFECTNEXTGAME);
	GameSetupMenus[gtOneOnOne]->LISetUnlimitedZero();

	GameSetupMenus[gtOneOnOne]->AddOptionItem("Victory Condition");
	GameSetupMenus[gtOneOnOne]->LIAddOption("Best of");
	GameSetupMenus[gtOneOnOne]->LIAddOption("First to");
	GameSetupMenus[gtOneOnOne]->LILock(LOCK_TAKEEFFECTNEXTGAME);

	//int FREENUMBEROFROUNDS 4;
	GameSetupMenus[gtOneOnOne]->AddNumericalSlider("Number Of Rounds",FREENUMBEROFROUNDS,1,100);
	GameSetupMenus[gtOneOnOne]->LISetUnlimitedZero();
	GameSetupMenus[gtOneOnOne]->LILock(LOCK_TAKEEFFECTNEXTGAME);

	TheGame->Beacon(16609);
	GameSetupMenus[gtCup] = new Menu(EnemySetupMenu->GetMenuStyle());
	SwitchMenuItems[gtCup]=GameSetupMenus[gtCup]->AddOptionItem("Game Type");
	GameSetupMenus[gtCup]->LIAddOption("Cup");
	GameSetupMenus[gtCup]->LILock(LOCK_TAKEEFFECTNEXTGAME);

	GameSetupMenus[gtCup]->AddNumericalSlider("Time Limit",0,1,99,""," minutes");
	GameSetupMenus[gtCup]->LILock(LOCK_TAKEEFFECTNEXTGAME);
	GameSetupMenus[gtCup]->LISetUnlimitedZero();

	GameSetupMenus[gtCup]->AddOptionItem("Victory Condition");
	GameSetupMenus[gtCup]->LIAddOption("Best of");
	GameSetupMenus[gtCup]->LIAddOption("First to");
	GameSetupMenus[gtCup]->LILock(LOCK_TAKEEFFECTNEXTGAME);

	GameSetupMenus[gtCup]->AddNumericalSlider("Rounds Per Match",1,1,3);
	GameSetupMenus[gtCup]->LILock(LOCK_TAKEEFFECTNEXTGAME);


	TheGame->Beacon(16610);
	GameSetupMenus[gtLeague] = new Menu(EnemySetupMenu->GetMenuStyle());
	SwitchMenuItems[gtLeague]=GameSetupMenus[gtLeague]->AddOptionItem("Game Type");
	GameSetupMenus[gtLeague]->LIAddOption("League");
	GameSetupMenus[gtLeague]->LILock(LOCK_TAKEEFFECTNEXTGAME);

	GameSetupMenus[gtLeague]->AddNumericalSlider("Time Limit",0,1,99,""," minutes");
	GameSetupMenus[gtLeague]->LILock(LOCK_TAKEEFFECTNEXTGAME);
	GameSetupMenus[gtLeague->LISetUnlimitedZero();

	GameSetupMenus[gtLeague]->AddNumericalSlider("Rounds Per Match",1,1,3);
	GameSetupMenus[gtLeague]->LILock(LOCK_TAKEEFFECTNEXTGAME);

	GameSetupMenus[gtLeague]->AddOptionItem("Points Per Victory");
	GameSetupMenus[gtLeague]->LIAddOption("2");
	GameSetupMenus[gtLeague]->LIAddOption("3");
	GameSetupMenus[gtLeague]->LILock(LOCK_TAKEEFFECTNEXTGAME);

	TheGame->Beacon(16611);
	for(int i = 0;i<NoOfGameTypes;i++){
		GameSetupMenus[i]->CreatePredefinedMenu(EdgeDist,BreakPoint+EdgeDist,800-EdgeDist,600-EdgeDist,pGameSetup);
		// GameSetupMenus are not used for input at all!!!!
		GameSetupMenus[i]->InactivateInput();
	}
	TheGame->Beacon(16612);

	TheGame->Beacon(16613);
	SetNeighbours();
	TheGame->Beacon(16614);

}
GameSetup::~GameSetup(){

}

void GameSetup::SetNeighbours(){

	ActiveMenuItem * TopItem;
	ActiveMenuItem * BottomItem;

	for(int a=0;a<2;a++){
			TheGame->Beacon(16621);
//		TopItem=; Actually very needed!!!
		for(int b=0;b<2;b++){
			TopItem=EnemySettings[2*a+b][esUL];
			TopItem->SetDownNeighbour(BottomItem);
			// Actually, these will always be the same but i'm lazy so i set them over and over again!
			BottomItem->SetUpNeighbour(TopItem);

		}
	}

}


void GameSetup::UpdateGroups(){

	(*GroupsPointer) = (*PlayersPointer)/2;
}

void GameSetup::SwitchGameType(){
	cgt=(GameType)(((int)cgt+1)%(int)NoOfGameTypes);
	SetNeighbours();
}

void GameSetup::Blit(){
	EnemySetupMenu->BlitMenu();
	GameSetupMenus[cgt]->BlitMenu();
/*
	if (TheGame->g_pDDSBack->GetDC(&TheGame->hdc) == DD_OK)
	{

		SelectObject(TheGame->hdc,TheGame->g_hFontBigPlus);
		SetBkColor(TheGame->hdc, TRANSPARENT);
		SetTextColor(TheGame->hdc, RGB(80, 0, 150));
		TheGame->Text(100,500,"Under Construction");	
	}TheGame->g_pDDSBack->ReleaseDC(TheGame->hdc);	
*/
}
void GameSetup::UpdateInput(){
	if(EnemySetupMenu->SelectedItem==SwitchMenuItems[gtOneOnOne]){
		if(TrueUp(DIK_RETURN)){
			SwitchGameType();
			return;
		}
	}

	if(TrueUp(DIK_ESCAPE)&&!EnemySetupMenu->SelectedItem->CatchesEscape()){
		FadeOut(ReturnState);
		return;
	}
	// EnemyMenu is the sole input menu
	EnemySetupMenu->UpdateInput();
}
void GameSetup::Setup(){
	UpdateGroups();
}

void GameSetup::SetSurfaces(){

}