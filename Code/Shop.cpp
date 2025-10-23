#include "Actor.h"
#include "Player.h"
#include "Shop.h"
#include "Game.h"
#include "Weapon.h"
#include "NewStore.h"
#include "GeneralConstants.h"
#include "AmmoAndArmorName.h"
#include "Debugger.h"
#include <stdio.h>

int Player::GetPrice(int Store, int Item, int Buy){
	
	if(Buy==S_BUY)
		return TheNewStore->ItemPrice[Store-1][Item];
	else if(Store==2 && Item < 12 && !(Item%2))
		return (TheNewStore->ItemPrice[Store-1][Item]*4)/10;

	return (TheNewStore->ItemPrice[Store-1][Item]*6)/10;
}


void Player::Sell(int Store, int Item){


	int Price=GetPrice(Store,Item,S_SELL),Sold=FALSE;

	if(Store==1)
		Sold=SellWeapon(Item);
	else if(Store==2)
		Sold=SellAmmo(Item);
	else if(Store==3)
		Sold=SellArmor(Item);
	else if(Store==4)
		Sold=SellMisc(Item);
	else if(Store==5)
		Sold=SellSpecial(Item);

	if(Sold){
		Cash+=Price;
//		if(Store==1||Store==3)
//			TheNewStore->FixPlayerInfo(Team);
	}
}


int Player::SellWeapon(int Item){

	if(!OWNEDWEAPONS[Item]){
		TheNewStore->Message("You do not own this weapon!",mcRed);
		return FALSE;
	}

	if(OWNEDWEAPONS[Item]==2){
		OWNEDWEAPONS[Item]--;
		return TRUE;
	}

	if(Item==25)
		Bullets[B_ATSHOT]--;

	int wpns=0;

	for(int a=0;a<30;a++){
		wpns+=OWNEDWEAPONS[a];
	}



	if(wpns==2&&OWNEDWEAPONS[WPN_ATSHOT] && Item!=WPN_ATSHOT){
		TheNewStore->Message("You have to have another weapon besides from the AT Shot!",mcRed);
		return FALSE;
	}

	if(wpns==1){
		TheNewStore->Message("You can't sell your last weapon!",mcRed);
		return FALSE;
	}


	OWNEDWEAPONS[Item]=FALSE;
	if(ActiveGun==Item)
		SwitchWeapon();
	return TRUE;

	

}


int Player::SellAmmo(int Item){
	


								
	if(Item==0&&Bullets[B_9]>=100)
		Bullets[B_9]-=100;				
	else if(Item==1&&Bullets[B_9]>=500)
		Bullets[B_9]-=500;
	else if(Item==2&&Bullets[B_556]>=100)
		Bullets[B_556]-=100;					
	else if(Item==3&&Bullets[B_556]>=500)
		Bullets[B_556]-=500;
	else if(Item==4&&Bullets[B_762]>=100)
		Bullets[B_762]-=100;									
	else if(Item==5&&Bullets[B_762]>=500)
		Bullets[B_762]-=500;
	else if(Item==6&&Bullets[B_12]>=100)
		Bullets[B_12]-=100;							
	else if(Item==7&&Bullets[B_12]>=500)
		Bullets[B_12]-=500;
	else if(Item==8&&Bullets[B_SH]>=100)
		Bullets[B_SH]-=100;							
	else if(Item==9&&Bullets[B_SH]>=500)
		Bullets[B_SH]-=500;
	else if(Item==10&&Bullets[B_NRJ]>=100)
		Bullets[B_NRJ]-=100;							
	else if(Item==11&&Bullets[B_NRJ]>=500)
		Bullets[B_NRJ]-=500;
	else if(Item==12&&Bullets[B_SHELL]>=5)
		Bullets[B_SHELL]-=5;							
	else if(Item==13&&Bullets[B_HESHELL]>=5)
		Bullets[B_HESHELL]-=5;
	else if(Item==14&&Bullets[B_APRKT]>=3)
		Bullets[B_APRKT]-=3;
	else if(Item==15&&Bullets[B_ATRKT])
		Bullets[B_ATRKT]--;
	else if(Item==16&&Bullets[B_CHARGE])
		Bullets[B_CHARGE]--;
	else if(Item==17&&Bullets[GR_GRENADE])
		Bullets[GR_GRENADE]--;
	else if(Item==18&&Bullets[B_MINE])
		Bullets[B_MINE]--;
	else if(Item==19&&Bullets[B_TMINE])
		Bullets[B_TMINE]--;
	else{
		TheNewStore->Message("You don't have that many!",mcRed);
		return FALSE;
	}

	return TRUE;
			
}
								
int Player::SellArmor(int Item){


	ArmorType at=(ArmorType)(Item/3);
	
	if(GetArmorStrength(at)==1+Item%3){
		SetArmor(at,0);
		return TRUE;
	}
	TheNewStore->Message("You don't own this armor!",mcRed);
	return FALSE;
			
}			

int Player::SellMisc(int Item){

	if(!Status[Item])
		return FALSE;
	if(Item==MISC_SLOTDEPTH){
		if(Special[0][1]>Status[Item]||Special[1][1]>Status[Item]||Special[2][1]>Status[Item]){
			TheNewStore->Message("You must sell some of your specials first!",mcRed);
			return FALSE;
		}
	}
	if(Item==MISC_DUALITY){		
		for(int a=0;a<30;a++){
			if(OWNEDWEAPONS[a]==2){
				TheNewStore->Message("You own two guns of the same kind!",mcRed);
				return FALSE;
			}
		}
	}

	Status[Item]--;
	return TRUE;
			
}								

int Player::SellSpecial(int Item){


	for(int which=2;which>=0;which--){
		if(Special[which][0]==Item+1){
			Special[which][1]--;
			if(!Special[which][1])
				Special[which][0]=0;
			return TRUE;
		}
	}
	TheNewStore->Message("You don't have this special!",mcRed);									
	return FALSE;
}


void Player::Buy(int Store, int Item){



	int Cost=GetPrice(Store,Item,S_BUY),Bought=0;
//	char sap[30];
	//sprintf(sap,"St:%i It:%i   %i",Store,Item,Cost);
	//MessageBox(TheGame->hWnd,sap,"Error",0);

	if(Cash<Cost){
		TheNewStore->Message("You don't have enough cash!",mcRed);	
		return;
	}

	if(Store==1)
		Bought=BuyWeapon(Item);
	else if(Store==2)
		Bought=BuyAmmo(Item);
	else if(Store==3)
		Bought=BuyArmor(Item);
	else if(Store==4)
		Bought=BuyMisc(Item);
	else if(Store==5)
		Bought=BuySpecial(Item);

	if(Bought){
		MoneySpent[Store-1]+=Cost;
		Cash-=Cost;
//		if(Store==1||Store==3)
//			TheNewStore->FixPlayerInfo(Team);
	}
}

int Player::BuyWeapon(int Item){

	
	if(!OWNEDWEAPONS[Item] ){
		OWNEDWEAPONS[Item]=TRUE;
		if(Item==25)											
			Bullets[B_ATSHOT]++;
		ActiveGun=Item;
		return TRUE;

	}else if(TheGame->Weapons[Item]->IsDualable()){
		if(Duality()){
			if(OWNEDWEAPONS[Item]==1){
				OWNEDWEAPONS[Item]=2;
				ActiveGun=Item;
				return TRUE;
			}else
				TheNewStore->Message("You already own two ",TheGame->Weapons[Item]->GetName(), mcRed);
		}else
			TheNewStore->Message("If you want to have two of these you have to buy Duality first!", mcRed);

	}else TheNewStore->Message("You already own a ",TheGame->Weapons[Item]->GetName(), mcRed);
	return FALSE;
}


const BulletType bbbType[AMMOITEMS]={B_9,B_9,B_556,B_556,B_762,B_762,B_12,B_12,B_SH,B_SH,
									  B_NRJ,B_NRJ,B_SHELL,B_HESHELL,B_APRKT,B_ATRKT,B_CHARGE,GR_GRENADE,B_MINE,B_TMINE};

const int bbAmount[AMMOITEMS]={100,500,100,500,100,500,100,500,100,500,
									 100,500, 5,5,3,1,1,1,1,1};


int Player::BuyAmmo(int Item){
								

	BulletType bType=bbbType[Item];

	if(Bullets[bType]<bbLimit[bType]){
		GiveBullets(bType,bbAmount[Item]);

		return TRUE;
	}
	return FALSE;
/*
	if(Item==0&&Bullets[B_9]<llimit)
		Bullets[B_9]+=100;
	else if(Item==1&&Bullets[B_9]<llimit)
		Bullets[B_9]+=500;
	else if(Item==2&&Bullets[B_556]<llimit)
		Bullets[B_556]+=100;
	else if(Item==3&&Bullets[B_556]<llimit)
		Bullets[B_556]+=500;
	else if(Item==4&&Bullets[B_762]<llimit)
		Bullets[B_762]+=100;
	else if(Item==5&&Bullets[B_762]<llimit)
		Bullets[B_762]+=500;
	else if(Item==6&&Bullets[B_12]<llimit)
		Bullets[B_12]+=100;
	else if(Item==7&&Bullets[B_12]<llimit)
		Bullets[B_12]+=500;
	else if(Item==8&&Bullets[B_SH]<llimit)
		Bullets[B_SH]+=100;
	else if(Item==9&&Bullets[B_SH]<llimit)
		Bullets[B_SH]+=500;
	else if(Item==10&&Bullets[B_NRJ]<llimit)
		Bullets[B_NRJ]+=100;
	else if(Item==11&&Bullets[B_NRJ]<llimit)
		Bullets[B_NRJ]+=500;
	else if(Item==12&&Bullets[B_SHELL]<llimit)
		Bullets[B_SHELL]+=5;
	else if(Item==13&&Bullets[B_HESHELL]<llimit)
		Bullets[B_HESHELL]+=5;
	else{
		if(Item==14&&Bullets[B_APRKT]<rlimit)
			Bullets[B_APRKT]+=3;
		else if(Item==15&&Bullets[B_ATRKT]<rlimit)
			Bullets[B_ATRKT]++;
		else if(Item==16&&Bullets[B_CHARGE]<llimit)
			Bullets[B_CHARGE]++;
		else if(Item==17&&Bullets[GR_GRENADE]<llimit)
			Bullets[GR_GRENADE]++;
		else if(Item==18&&Bullets[B_MINE]<rlimit)
			Bullets[B_MINE]++;
		else if(Item==19&&Bullets[B_TMINE]<rlimit)
			Bullets[B_TMINE]++;
		else{
		    TheNewStore->Message("You cannot carry any more than you already do!", mcRed);
 
			return FALSE;
		}
		
		
		return TRUE;

	}

	for(int i=0; i<PlayerBulletTypes; i++)
		if(Bullets[i]>llimit)
			Bullets[i]=llimit;
								
	return TRUE;
*/	
}

int Player::BuyArmor(int Item){

	ArmorType at=(ArmorType)(Item/3);

	if(!GetArmorStrength(at)){
		SetArmor(at, Item%3+1);
		return TRUE;
	}

	static char str[100];
	sprintf(str,"You already own %s %s!",ArmorName[at][4],ArmorName[at][1]);
    TheNewStore->Message(str, mcRed);
 
	return FALSE;

}

int Player::BuyMisc(int Item){

	if(Item!=MISC_XLIFE&&Item!=MISC_SLOTDEPTH&&Status[Item]){
	   TheNewStore->Message("You already have this!", mcRed);
		return FALSE;
	}
	
	if(Item==MISC_SLOTDEPTH&&Status[Item]>=2){
		TheNewStore->Message("Maximum total slot depth is three!", mcRed);
		return FALSE;
	}
	if(Item==MISC_XLIFE&&Status[Item]>=3){
		TheNewStore->Message("You are not allowed to buy more than three extra lives!", mcRed);
		return FALSE;
	}
	Status[Item]++;
/*
	if(Item==MISC_SLOTDEPTH){
		for(int i=0, a=0, b=1;i<3;i++){
			if(Special[a][0]==Special[b][0]&&Special[a][0]){
				while(Special[a][1]<=Status[MISC_SLOTDEPTH]&&Special[b][0]){
					Special[a][1]++;
					Special[b][1]--;
					if(!Special[b][1])
						Special[b][0]=0;
				}
			}
			if(i%2)
				a++;
			else
				b++;
		}
	}
*/
	return TRUE;

}

int Player::BuySpecial(int Item){

	int which;


	for(which=0;which<3;which++){
		if(Item+1==Special[which][0]){
			if(Special[which][1]<=Status[MISC_SLOTDEPTH]){
				Special[which][1]++;
				return TRUE;
			}else{
			   TheNewStore->Message("Maximum slot depth reached! Only one special type per slot is allowed", mcRed);
			   return FALSE;
			}
		}
	}
	for(which=0;which<3;which++){
		if(!Special[which][0]){
			Special[which][0]=Item+1;
			Special[which][1]=1;
			return TRUE;
		}			
	}

    TheNewStore->Message("All special slots are filled!", mcRed);
 	
	return FALSE;

}
								


//////////////////////////////////////////////////////////////////////
// Shop Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Shop::Shop()
{

}

Shop::~Shop()
{

}
