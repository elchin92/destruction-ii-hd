#include "NewStore.h"
#include "Player.h"
#include "Game.h"
#include "DIK_CODES.h"
#include "Debugger.h"
#include <stdio.h>

void NewStore::OnBuy(){
	apl->Buy(Section[cpl],Selection[cpl]);
}
void NewStore::OnSell(){
	apl->Sell(Section[cpl],Selection[cpl]);
}

void NewStore::OnLeft(){

	Selection[cpl]--;
	if(Selection[cpl]<0)
		Selection[cpl]=StoreInfo[Section[cpl]][0]-1;

	Beacon(6000 + Selection[cpl]);
}
void NewStore::OnRight(){
	Selection[cpl]++;
	Selection[cpl]%=StoreInfo[Section[cpl]][0];
	Beacon(7000 + Section[cpl]);
	Beacon(7000 + Selection[cpl]);
	Beacon(7000 + StoreInfo[Section[cpl]][0]);
}
void NewStore::OnEnterShop(){
	SetSection(Selection[cpl]+1);
	Selection[cpl]=0;
	Choice[cpl]=1;
	ButtonFunctions[cpl][2]=OnBuy;
	ButtonFunctions[cpl][3]=OnSell;
	sprintf(Msg,"Press <%s> to return and select a new store",DIK2STR(esc[cpl]));
	iMessage(mcBlue);
}

void NewStore::OnQuit(){

	if(Section[cpl]==0){
		if(DoubleShop){
			Choice[cpl]=9;
			if(Choice[!cpl]==9){
				KEEPSETTINGS=FALSE;
				SwitchState(GETREADY);
				WhosTurn=1;
			}else{
			sprintf(Msg,"%s has finished shopping. Press %s to return",apl->GetName(),DIK2STR(esc[cpl]));
			iMessage(mcWhite);		
			
			}
		}else{
			KEEPSETTINGS=FALSE;
			if(cpl){
				SwitchState(GETREADY);
				WhosTurn=1;
			}else
				SwitchState(LOOKAWAY);
		}
	}else{
		LeaveStore();
	}

}
void NewStore::OnGive(){
	if(apl->GetCash()>=5000){
		apl->GiveReward(-5000);
		if(DoubleShop){
			ThePlayers[!cpl]->GiveReward(5000);
			sprintf(Msg, "%s gave $5000 to %s!",apl->GetName(),ThePlayers[!cpl]->GetName());
		}else{
			ThePlayers[!cpl]->GetDonation(5000,apl);
			sprintf(Msg, "You gave $5000 to %s which will be delivered next round!",ThePlayers[!cpl]->GetName());
		}

		iMessage(mcGreen);
	}
}
void NewStore::OnDetails(){
	if(DoubleShop){
		Choice[cpl]=8;
		sprintf(Msg,"Press <%s> to return and select a new store",DIK2STR(esc[cpl]));
		iMessage(mcBlue);
	}else{
		sprintf(Msg,"Detailed info is displayed on the right side! This button is only used in DS mode",mcBlue);
		iMessage(mcBlue);		
	}
}

void NewStore::OnSwitch(){
	apl->SwitchWeapon();
}

void NewStore::DoNothing(){}
void NewStore::OnGoBack(){
	Choice[cpl]=6;
}