#include "Stats.h"
#include "Game.h"
#include "Strconversions.h"
#include "Player.h"
#include "Weapon.h"
#include "Menu.h"
#include "ButtonMenuItem.h"
#include "InfoBox.h"
#include "FullVersionInfo.h"
#include "GameKeeper.h"
#include "Debugger.h"
#include <stdio.h>
#include "InputEngine.h"
#include "definitions.h"

Stats * TheStats;

void OnLeft(){TheStats->SwitchLeft();}
void OnRight(){TheStats->SwitchRight();}
void OnEndGame(){TheStats->EndGame();}





const int ItemConvert[30]={IT_9MM,IT_556,IT_762,IT_12,IT_SSHELLS,IT_NRJ,IT_SHELLS,IT_APMIS,IT_CHARGES,IT_GRENADE,IT_CMINE,IT_SDOLLAR,IT_DDOLLAR,IT_CASHHEAP,IT_CHEST,IT_X2,IT_RFIRE,IT_DBLDMG,IT_SUPERMAN,IT_SPEED,IT_SHIELD,IT_INVIS,IT_TRINITY,IT_QUESTION,IT_SMEDI,IT_MEDI,IT_1UP,IT_STOP,IT_LENSE,IT_MINE};
const DamageType DeathConvert[7]={DT_SHOT,DT_CHARGE,DT_GRENADE,DT_SPECIAL,DT_PMINE,DT_RUNOVER,DT_NONE};
const MortalType KillConvert[5]={M_CA,M_MA,M_DR,M_TK,M_PL};

Stats::Stats(){
	Initialize();

	ButtonMenu = new Menu();

	ActiveMenuItem * b1 = ButtonMenu->AddMenuItem(new ButtonMenuItem( 36,528,225, 54, 14, &TheGame->g_pDDSLife, OnLeft));
	ActiveMenuItem * b2 = ButtonMenu->AddMenuItem(new ButtonMenuItem(288,528,225, 54, 14, &TheGame->g_pDDSLife, OnEndGame));
	ActiveMenuItem * b3 = ButtonMenu->AddMenuItem(new ButtonMenuItem(542,528,225, 54, 14, &TheGame->g_pDDSLife, OnRight));

	Beacon(2462460);
      
	b1->SetRightNeighbour(b2);
	b2->SetLeftNeighbour(b1);
	b2->SetRightNeighbour(b3);
	b3->SetLeftNeighbour(b2);
	Beacon(2462461);

	BackgroundSurface=&TheGame->g_pDDSBackground;

	NameFont=CreateFont(24,
		0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");

	StatsFont=CreateFont(14,
		0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");

	for(int pl=0;pl<2;pl++){
		NameBox[pl] = new InfoBox(146+400*pl,107,182,24,&NameFont, mcBlue,TA_CENTER);
		StatsBox[pl] = new InfoBox(108+400*pl,163,68,86,&StatsFont, RGB(181,176,244),TA_CENTER);
		MoneyBox[pl] = new InfoBox(284+400*pl,165,98,71,&StatsFont, RGB(196,4,196),TA_CENTER);
		ItemBox[pl] = new InfoBox(101+400*pl,284,48,216,&StatsFont, RGB(4,4,236),TA_CENTER);
		KillsBox[pl] = new InfoBox(330+400*pl,279,41,71,&StatsFont, RGB(20,188,4),TA_CENTER);
		DeathBox[pl] = new InfoBox(298+400*pl,399,82,100,&StatsFont, RGB(200,6,6),TA_CENTER);
		sp[pl]=pl;
	}


}
Stats::~Stats(){
	DeleteObject(NameFont);
	DeleteObject(StatsFont);

	for(int pl=0;pl<2;pl++){
		delete NameBox[pl];
		delete StatsBox[pl];
		delete MoneyBox[pl];
		delete ItemBox[pl];
		delete KillsBox[pl];
		delete DeathBox[pl];

	}
}

void Stats::InternalBlit(){


	for(int pl=0;pl<2;pl++){
		Player * apl = TheGameKeeper->GetPlayer(sp[pl]);
		if(NameBox[pl]->BlankAndPrepareBox(0) ){

			NameBox[pl]->Print(0,0,apl->GetName());
		}NameBox[pl]->CloseBox();

		if(StatsBox[pl]->BlankAndPrepareBox(0) ){
			sprintf(tstr,"#%i",TheGameKeeper->GetRank(sp[pl]));
			if(TheGameKeeper->WasSharedPlacement())
				strcat(tstr," (shared)");
			StatsBox[pl]->Print(0,0,tstr);
			StatsBox[pl]->Print(0,14,apl->GetRoundsPlayed());
			StatsBox[pl]->Print(0,28,apl->GetRoundsWon());
			StatsBox[pl]->Print(0,14*3,apl->GetRoundsLost());
			StatsBox[pl]->Print(0,14*4,apl->GetRoundsDraw());
			sprintf(tstr,"$%i",apl->GetStat(ST_EARNED));
			Beacon(991);
			StatsBox[pl]->Print(0,14*5,tstr);
		}StatsBox[pl]->CloseBox();
					Beacon(992);
		if(MoneyBox[pl]->BlankAndPrepareBox(0) ){
						Beacon(993);
			for(int s=0;s<5;s++){
							Beacon(994);
				sprintf(tstr,"$%i",apl->GetMoneySpent(s));
				MoneyBox[pl]->Print(-20,14*s,tstr);
							Beacon(995);
				sprintf(tstr,"%i%%",apl->GetMoneySpentPercentage(s));
				Beacon(996);
				MoneyBox[pl]->Print(40,14*s,tstr);
			}

		}MoneyBox[pl]->CloseBox();
		if(ItemBox[pl]->BlankAndPrepareBox(0) ){
			for(int s=0;s<2;s++){
				for(int i=0;i<15;i++){
					Beacon(990000+s*100+i);
					ItemBox[pl]->Print(-10+20*s,14*i+2,apl->GetItemCount(ItemConvert[15*s+i]));
				}
			}
		}ItemBox[pl]->CloseBox();
		if(KillsBox[pl]->BlankAndPrepareBox(0) ){
			for(int k=0;k<5;k++){
				Beacon(980000+k);
				KillsBox[pl]->Print(0,14*k,apl->GetKills(KillConvert[k]));
			}
		}KillsBox[pl]->CloseBox();

		if(DeathBox[pl]->BlankAndPrepareBox(0) ){
			for(int s=0;s<3;s++){
				for(int d=0;d<7;d++){
					Beacon(970000+s*100+d);
					DeathBox[pl]->Print(-27+27*s,14*d,apl->GetDeath(DeathConvert[d], s));
				}
			}
			
		}DeathBox[pl]->CloseBox();
	}
	
}
void Stats::InternalUpdateInput(){

	// Shaln't be able to go to start menu from here!
	// Then you can avoid the nag screen
	Beacon(234234);
	ButtonMenu->UpdateInput();
	Beacon(234235);
}
void Stats::Setup(){
}
void Stats::SetSurfaces(){

	AddBitmapAndSurface("StatisticsBMP", &TheGame->g_pDDSBackground);
	AddBitmapAndSurface("StatisticsSBMP", &TheGame->g_pDDSLife);

}

void Stats::SwitchLeft(){
	sp[0]++;
	sp[0]%=TheGameKeeper->GetNumberOfPlayers();
}
void Stats::SwitchRight(){
	sp[1]++;
	sp[1]%=TheGameKeeper->GetNumberOfPlayers();
}
void Stats::EndGame(){
#ifdef __D2PROTECTED__
	if(TheGame->ISREGISTERED){
		DP("Quit through stats");
		TheGame->Quit();
	}else{
		TheFullVersionInfo->SetCountDown();
		SwitchState(FULLVERSION);
	}
#else
	SwitchState(FULLVERSION);
#endif

}