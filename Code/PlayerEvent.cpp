#include "Actor.h"
#include "Player.h"
#include "Game.h"
#include "GeneralConstants.h"
#include "DamageConstants.h"
#include "PriceConstants.h"
#include "TimeConstants.h"
#include "SpecialClusterBomb.h"
#include "Debugger.h"


void Player::BeginRound(){
	// Shall be called after deciding if its moneymaze

/*#ifdef DEBUGGING
	char str[45];
	for(int i=0;i<2;i++){
		sprintf(str,"%s, Pos[%i]: ",GetName(),i);
		TheGame->dw->AddMonitor(str, &Pos[i]);
	}
	sprintf(str,"%s, UP: ",GetName());
	TheGame->dw->AddMonitor(str, &UP);
	sprintf(str,"%s, DOWN: ",GetName());
	TheGame->dw->AddMonitor(str, &DOWN);
	sprintf(str,"%s, LEFT: ",GetName());
	TheGame->dw->AddMonitor(str, &LEFT);
	sprintf(str,"%s, RIGHT: ",GetName());
	TheGame->dw->AddMonitor(str, &RIGHT);

#endif*/
	CashBefore=Cash;

	LooseStates();

	nextQuadMortal=NULL;

	Lives=2;
	
	UnFreeze();
	FullHealth();	


	if(!TheGame->MoneyMaze()){

		if(Sh60()){
			Status[SH60]=FALSE;
			BecomeShielded();
		}
		if(ShL())
			BecomeShielded();
		if(Sp60()){
			Status[SP60]=FALSE;
			GoFast();
		}
		if(SpR()||SpL())
			GoFast();
		
		if(Iv60()){
			Status[IV60]=FALSE;
			TurnInvisible();
		}
		Lives+=Status[MISC_XLIFE];
		Status[MISC_XLIFE]=0;	

	}
	else{
		ChargesB4=Bullets[B_CHARGE];
		Bullets[B_CHARGE]=10;
		GoFast();
		moneymult=3;
	}

	SetStartPos(FALSE);
}

void Player::EndRound(int Winner){

	RoundStats[rsPlayed]++;
	if(Winner==Team)
		RoundStats[rsWon]++;
	else if(Winner==2)
		RoundStats[rsDraw]++;
	else
		RoundStats[rsLost]++;

	if(!TheGame->MoneyMaze()){
		if(Winner==Team){
			Cash+=RoundPrice;
		}
	}

	Beacon(1810666);
	StopTazerSound();

	for(int temp=Lives;temp>0;temp--)
		Cash+=LifePrice;
	
	
	Statistics[ST_EARNED]+=Cash-CashBefore;	

	for(int i=0;i<3;i++){
		if(Special[i][0]&&!Special[i][1])
			Special[i][0]=0;
	}
	
	if(TheGame->LASTMM){
		moneymult=1;
		Bullets[B_CHARGE]=ChargesB4;
		TheGame->LASTMM=FALSE;
	}
	else{
		Status[SPR]=FALSE;
		Status[SPL]=FALSE;
		Status[SHL]=FALSE;
	}
	Lives=0;
}

void Player::CollectItem(int Item){

	Item--;

	int COLLECTED = TRUE, REMOVE=TRUE;

	if(Item==IT_9MM){
		CreateAndPlaySound(sSmallAmmo);
		GiveBullets(B_9, AmmoSize);
	}
	else if(Item==IT_556){
		CreateAndPlaySound(sMediumAmmo);
		GiveBullets(B_556, AmmoSize);
	}
	else if(Item==IT_762){
		CreateAndPlaySound(sMediumAmmo);
		GiveBullets(B_762, AmmoSize);
	}
	else if(Item==IT_12){
		CreateAndPlaySound(sBigAmmo);
		GiveBullets(B_12, AmmoSize);
	}
	else if(Item==IT_SSHELLS){
		CreateAndPlaySound(sSmallAmmo);
		GiveBullets(B_SH, AmmoSize);
	}
	else if(Item==IT_NRJ){
		CreateAndPlaySound(sMediumAmmo);
		GiveBullets(B_NRJ, AmmoSize);
	}
	else if(Item==IT_SHELLS){
		CreateAndPlaySound(sBigAmmo);
		GiveBullets(B_SHELL, 3);
	}
	else if(Item==IT_APMIS){
		CreateAndPlaySound(sBigAmmo);
		GiveBullets(B_APRKT, 3);
	}
	else if(Item==IT_1UP){
		Lives++;
		CreateAndPlaySound(s1UP);

		if(Lives>rlimit)
			Lives=rlimit;
	}
	else if(Item==IT_SHIELD){
		CreateAndPlaySound(sShield);
		BecomeShielded();
		
	}
	else if(Item==IT_SDOLLAR){
		CreateAndPlaySound(sCash);
		Cash+=400*moneymult;
	}
	else if(Item==IT_DDOLLAR){
		CreateAndPlaySound(sCash);
		Cash+=800*moneymult;
	}
	else if(Item==IT_CASHHEAP){
		CreateAndPlaySound(sCashHeap);
		Cash+=2000*moneymult;
	}
	else if(Item==IT_CHEST){
		CreateAndPlaySound(sTreasureChest);
		Cash+=12000*moneymult;
	}
	else if(Item==IT_MEDI||Item==IT_SMEDI){
		CreateAndPlaySound(sHealth);
		Heal(Item);
	}
	else if(Item==IT_SPEED){
		CreateAndPlaySound(sSpeed);
		GoFast();
	}
	else if(Item==IT_CHARGES){
		CreateAndPlaySound(sCharge);

		Bullets[B_CHARGE]+=3;
		if(Bullets[B_CHARGE]>llimit)
			Bullets[B_CHARGE]=llimit;
	}
	else if(Item==IT_QUESTION){
		if(Doubling()){
			BOOL TEMP=DBLCOLLECTED;
			DBLCOLLECTED=TRUE;
			CollectItem(TheGame->GenerateItem(GI_NOMINE));
			DBLCOLLECTED=TRUE;
			CollectItem(TheGame->GenerateItem(GI_NOMINE));
			DBLCOLLECTED=TEMP;
		}
		else
			CollectItem(TheGame->GenerateItem(GI_NOMINE));
	}
	else if(Item==IT_RFIRE){
		CreateAndPlaySound(sRapidFire);
		FireRapidly();
	}
	else if(Item==IT_X2){
		CreateAndPlaySound(sX2);
		StartDoubling();
	}
	else if(Item==IT_DBLDMG){
		CreateAndPlaySound(sDoubleDamage);
		DoDoubleDamage();
	}
	else if(Item==IT_CMINE){
		CreateAndPlaySound(sCharge);
		GiveBullets(B_MINE,1);
	}
	else if(Item==IT_GRENADE){
		CreateAndPlaySound(sCharge);
		GiveBullets(GR_GRENADE,1);
	}
	else if(Item==IT_SUPERMAN){
		CreateAndPlaySound(sSuperman);
		BecomeSuperman();
	}
	else if(Item==IT_LENSE){
		CreateAndPlaySound(sLookingGlass);
		BLITLIFE=TRUE;
	}else if(Item==IT_STOP){
		CreateAndPlaySound(sStop);
		ThePlayers[!Team]->Stop();
	}
	else if(Item==IT_TRINITY){
		CreateAndPlaySound(sTrinity);	
		TurnInvisible();
		GoFast();
		BecomeShielded();

	}else if(Item==IT_INVIS){
		CreateAndPlaySound(sInvisibility);
		TurnInvisible();
	}
	else
		COLLECTED = FALSE;
	if(Item==IT_TMINE1||Item==IT_TMINE2)
		REMOVE=FALSE;

	if(Item==IT_MINE||Item==IT_MINE1||Item==IT_MINE2){
		COLLECTED=TRUE;
		Item=IT_MINE;
		REMOVE=FALSE;
	}

	if(REMOVE)
		TheGame->WorldMap[GetXs()][GetYs()][2]=0;
	// bob


	if(COLLECTED){
		Items[Item]++;
	}


	if(Doubling()&&Item!=IT_MINE){
		DBLCOLLECTED=!DBLCOLLECTED;
		if(DBLCOLLECTED)
			CollectItem(Item+1);
			
	}

	return;

}

