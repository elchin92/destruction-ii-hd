#include "NewStore.h"
#include "InfoBox.h"
#include "Game.h"
#include "Player.h"
#include "Weapon.h"
#include "staticchar.h"
#include "AmmoAndArmorName.h"
#include "TazerBeam.h"
#include "GameKeeper.h"
#include "InputEngine.h"
#include "Debugger.h"
#include <stdio.h>

const ArmorType ArmorLayer[ActualArmorTypes] = {AT_AURA,AT_PANTS,AT_BOOTS,AT_HELMET,AT_WEST};

const int ArmorY[ActualArmorTypes][2] = {	{27,72}, // West
											{5,21}, // Helmet...
											{69,124},
											{108,132},
											{3,137}
};

void NewStore::InternalBlit(){

	//TheGame->BlankScreen();

	if(DoubleShop){
		for(int i=0;i<2;i++){
			if(Choice[cpl]==8)
				BlitInfo(cpl);
			else
				BlitShop(cpl);
			SwitchPlayer();
		}
	}else{
		BlitShop(0);
		BlitInfo(1);
	}
	BlitMessageGfx();

}
void NewStore::BlitShop(int w){
	
	TheGame->rcRect.left = 0;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = TheGame->rcRect.left+400;
	TheGame->rcRect.bottom = TheGame->rcRect.top+555;

	TheGame->BlitSetRcCC(400*w, 0, &g_pDDSShopNormal[w]);

	BlitCurrentItem(w);
	BlitPriceAndGot(w);
	BlitSpecials(w);
	BlitShortInfo(w);



	if(Section[cpl]==0)
		BlitEnterStoreButton(w);

	BlitSelectedButton(w);

	if(Section[cpl]!=8&&Section[cpl]!=0){
		BlitLeaveStoreButton(w);
	}

}
void NewStore::BlitInfo(int w){

	TheGame->rcRect.left = 0;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = TheGame->rcRect.left+400;
	TheGame->rcRect.bottom = TheGame->rcRect.top+555;

	TheGame->BlitSetRcCC(400*w, 0, &g_pDDSShopPlayerInfo[w]);

	BlitMiscInfo(w);
	BlitArmorInfo(w);
	BlitWeaponInfo(w);
}



const int SelectedButton[10][5]={ //Observere x1,y1,x2,y2 not w&h!!!!!
	{2,161,72,228,1}, // Prev
	{323,161,399,228,1}, // Next
	{135,325,213,369,2}, // Buy
	{213,325,292,369,2}, // Sell
	{27,473,199,513,1}, // Give
	{200,473,372,513,1}, // Switch
	{27,513,199,552,1}, // Details
	{200,513,372,552,1}, // Quit
	{0,0,0,0,0}, // Inside Details
	{0,0,0,0,0} // Quitted

};
void NewStore::BlitSelectedButton(int w){
	if(SelectedButton[Choice[cpl]][4]){
		if(Section[cpl]==0 && SelectedButton[Choice[cpl]][4]==2)
			return;
		
		TheGame->rcRect.left = SelectedButton[Choice[cpl]][0];
		TheGame->rcRect.top = SelectedButton[Choice[cpl]][1];
		TheGame->rcRect.right = SelectedButton[Choice[cpl]][2];
		TheGame->rcRect.bottom = SelectedButton[Choice[cpl]][3];

		TheGame->BlitSetRc(SelectedButton[Choice[cpl]][0]+400*w, SelectedButton[Choice[cpl]][1], &g_pDDSShopSelected[w]);
	}
}



void NewStore::BlitEnterStoreButton(int w){
	const int X=134,Y=324,W=157,H=45;	

	TheGame->BlankSurfaceArea(X+400*w,Y, W, H, &TheGame->g_pDDSBack);

	TheGame->rcRect.left = 0;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = W;
	TheGame->rcRect.bottom = H;

	TheGame->BlitSetRc(X+400*w, Y, &g_pDDSEnterStore[w][(Choice[cpl]==2||Choice[cpl]==3)]);	

}

void NewStore::BlitLeaveStoreButton(int w){
	const int X=223,Y=523,W=122,H=20;	

	//TheGame->BlankSurfaceArea(X+400*w,Y, W, H, &TheGame->g_pDDSBack);

	TheGame->rcRect.left = 0;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = W;
	TheGame->rcRect.bottom = H;

	TheGame->BlitSetRc(X+400*w, Y, &g_pDDSLeaveStore[(Choice[cpl]==7)]);	

}



//
// Boxes
//




void NewStore::BlitMiscInfo(int w){
	if(MiscBox->BlankAndPrepareBox(400*w)){

		for(int i=0, ya=5 ;i<MISCITEMS;i++){
			if(apl->GotItem(i)&&i!=MISC_XLIFE&&i!=MISC_SLOTDEPTH){
				MiscBox->Print(5,ya,Information[4][i][0]);
				ya+=12;
			}
		}
		sprintf(tstr, "Extra Lives  %i",apl->GotItem(MISC_XLIFE));
		MiscBox->Print(5,ya,tstr);
		ya+=12;
		sprintf(tstr, "Slot Depth  %i",apl->GotItem(MISC_SLOTDEPTH)+1);
		MiscBox->Print(5,ya,tstr);	

	}MiscBox->CloseBox();
}

void NewStore::BlitArmorInfo(int w){
	if(ArmorBox->BlankAndPrepareBox(400*w) ){
		int xa=84, ya=21;

		sprintf(tstr,"Armor Points  %i", apl->GetAP());
		ArmorBox->Print(xa,ya,tstr);
		ya+=16*2;

		for(int i=0 ;i<ActualArmorTypes;i++){
			sprintf(tstr,"%s  %i%%",ArmorName[i][2], apl->GetArmorPercentage((ArmorType)i));
			ArmorBox->Print(xa,ya,tstr);
			ya+=16;
		}		
	}ArmorBox->CloseBox();


	const int sx=10, sy=10;

	TheGame->rcRect.left = 70*ActualArmorTypes;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = TheGame->rcRect.left+70;
	TheGame->rcRect.bottom = TheGame->rcRect.top+140;
	TheGame->BlitOntoSetRc(ArmorBox->GetX()+sx,ArmorBox->GetY()+sx,&TheGame->g_pDDSBack,&g_pDDSArmor);

	for(int i=0 ;i<ActualArmorTypes;i++){
		ArmorType at=ArmorLayer[i];
		int s=apl->GetArmorStrength(at);
		if(s){
			s--;
			TheGame->rcRect.left = 70*at;
			TheGame->rcRect.top = ArmorY[at][0]+140*(s%3);
			TheGame->rcRect.right = TheGame->rcRect.left+70;
			TheGame->rcRect.bottom = ArmorY[at][1]+140*(s%3);
			TheGame->BlitOntoSetRc(ArmorBox->GetX()+sx,ArmorBox->GetY()+sx+ArmorY[at][0],&TheGame->g_pDDSBack,&g_pDDSArmor);
		}

	}

}

void NewStore::BlitWeaponInfo(int w){
	CurrentWeapon=apl->GetWeapon();

	BlitFunctions[1][0]=BlitWeaponTitle;
	for(int i=0;i<3;i++){
		if(WeaponBox[i]->BlankAndPrepareBox(400*w) ){
			(this->*(BlitFunctions[1][i]))(WeaponBox[i]);
		}WeaponBox[i]->CloseBox();	
	}
	BlitFunctions[1][0]=BlitRegularShopTitle;
}

void NewStore::BlitCurrentItem(int w){
	if(Section[cpl]==1)
		CurrentWeapon=TheGame->Weapons[Selection[cpl]];

	for(int i=0;i<3;i++){
		if(CurrentItemBox[i]->BlankAndPrepareBox(400*w) ){
			(this->*(BlitFunctions[Section[cpl]][i]))(CurrentItemBox[i]);
		}CurrentItemBox[i]->CloseBox();	
	}
}

void NewStore::BlitSpecials(int w){

	for(int sp=0;sp<3;sp++){

		if(SpecialBox[sp]->BlankAndPrepareBox(400*w) ){

			SpecialBox[sp]->CloseBox();
			int special=apl->GetSpecial(sp)-1;

			if(special>=0){
				TheGame->rcRect.left =(special%StoreInfo[5][3])*StoreInfo[5][1]+3;
				TheGame->rcRect.top = (special/StoreInfo[5][3])*StoreInfo[5][2]+3;
				TheGame->rcRect.right = TheGame->rcRect.left+StoreInfo[5][1]-3;
				TheGame->rcRect.bottom = TheGame->rcRect.top+StoreInfo[5][2]-3;
				TheGame->BlitSetRc(SpecialBox[sp]->GetX(),SpecialBox[sp]->GetY(),&g_pDDSSpecial);
			}
			SpecialBox[sp]->OpenBox();
			SetBkMode(TheGame->hdc, TRANSPARENT);
			if(special>=0){
				sprintf(tstr,"%i",apl->GetSpecialSlotAmount(sp));
				SpecialBox[sp]->Print(29,3,tstr);			
				
			}else{
				SpecialBox[sp]->SetTemporaryInfoBoxTextColor(mcWhite);
				sprintf(tstr,"Slot %i",sp+1);
				SpecialBox[sp]->Print(0,19,tstr);
				SpecialBox[sp]->Print(0,35,"Empty!");
			}

		}SpecialBox[sp]->CloseBox();



	}
}

void NewStore::BlitShortInfo(int w){
	if(ShortInfoBox->BlankAndPrepareBox(400*w) ){
		int xa=3,ya=3;
		sprintf(tstr,"Cash  $%i",apl->GetCash());
		ShortInfoBox->Print(xa,ya,tstr);ya+=12;
		sprintf(tstr,"Charges  %i",apl->GetBullets(B_CHARGE));
		ShortInfoBox->Print(xa,ya,tstr);ya+=12;
		sprintf(tstr,"Grenades  %i",apl->GetBullets(GR_GRENADE));
		ShortInfoBox->Print(xa,ya,tstr);ya+=12;
		sprintf(tstr,"Mines  %i",apl->GetBullets(B_MINE));
		ShortInfoBox->Print(xa,ya,tstr);ya+=12;
		ShortInfoBox->Print(xa,ya,apl->GetWeapon()->GetName());ya+=12;
		sprintf(tstr,"Ammo  %s",apl->GetAmmoString());
		ShortInfoBox->Print(xa,ya,tstr);

	}ShortInfoBox->CloseBox();
}
void NewStore::BlitPriceAndGot(int w){
	if(PriceBox->BlankAndPrepareBox(400*w) ){

		if(Section[cpl]){
			if(Choice[cpl]==3){
				sprintf(tstr,"Get: $%i",apl->GetPrice(Section[cpl], Selection[cpl], S_SELL));
			}
			else
				sprintf(tstr,"Cost: $%i",apl->GetPrice(Section[cpl], Selection[cpl], S_BUY));
			PriceBox->Print(0,5,tstr);
		}

	}PriceBox->CloseBox();
	if(GotBox->BlankAndPrepareBox(400*w) ){

		if(Section[cpl]){
			strcpy(tstr,"");
			if(Section[cpl]==2||Section[cpl]==5||Section[cpl]==4&&(Selection[cpl]==MISC_XLIFE&&apl->GotItem(Selection[cpl]))){
				int amount;
				int DOIT=TRUE;
				if(Section[cpl]==2){
					 if(Selection[cpl]<2){
						amount=apl->GetBullets(B_9);
					}else if(Selection[cpl]<4){
						amount=apl->GetBullets(B_556);
					}else if(Selection[cpl]<6){
						amount=apl->GetBullets(B_762);
					}else if(Selection[cpl]<8){
						amount=apl->GetBullets(B_12);
					}else if(Selection[cpl]<10){
						amount=apl->GetBullets(B_SH);
					}else if(Selection[cpl]<12){
						amount=apl->GetBullets(B_NRJ);
						sprintf(tstr, "You Got: %i.%i%i", amount/100,amount/10-10*(amount/100),amount%10);
						DOIT=FALSE;
					}else if(Selection[cpl]==12){
						amount=apl->GetBullets(B_SHELL);
					}else if(Selection[cpl]==13){
						amount=apl->GetBullets(B_HESHELL);
					}else if(Selection[cpl]==14){
						amount=apl->GetBullets(B_APRKT);
					}else if(Selection[cpl]==15){
						amount=apl->GetBullets(B_ATRKT);
					}else if(Selection[cpl]==16){
						amount=apl->GetBullets(B_CHARGE);
					}else if(Selection[cpl]==17){
						amount=apl->GetBullets(GR_GRENADE);
					}else if(Selection[cpl]==18){
						amount=apl->GetBullets(B_MINE);
					}else if(Selection[cpl]==19){
						amount=apl->GetBullets(B_TMINE);
					}
					
				}else if(Section[cpl]==5){
					amount=apl->GetSpecialAmount(Selection[cpl]);
				}else{
					amount=apl->GotItem(Selection[cpl]);
				}
				if(DOIT)
					sprintf(tstr, "You Got: %i", amount);
			}else{
				BOOL GOTIT=FALSE;
				if(Section[cpl]==1){
					GOTIT = apl->GotGun(Selection[cpl]);
				}else if(Section[cpl]==3){
					GOTIT = apl->HaveArmor((ArmorType)(Selection[cpl]/3),Selection[cpl]%3+1);
				}else if(Section[cpl]==4){
					GOTIT = apl->GotItem(Selection[cpl]);
				}
				if(GOTIT==1){
					strcpy(tstr,"Got It!");
				}else if(GOTIT==2){
					strcpy(tstr,"Got Two!");
				}
					
			}		

			GotBox->Print(0,5,tstr);
		}
	}GotBox->CloseBox();
}




//
// CurrentItem Functions
//

void NewStore::BlitRegularShopInfo(InfoBox * Box){

	if(Information[Section[cpl]][Selection[cpl]][2][0]=='\0'){
		Box->Print(0,34,Information[Section[cpl]][Selection[cpl]][1]);
	}else if(Information[Section[cpl]][Selection[cpl]][3][0]=='\0'){
		Box->Print(0,20,Information[Section[cpl]][Selection[cpl]][1]);
		Box->Print(0,50,Information[Section[cpl]][Selection[cpl]][2]);
	}
	else{
		Box->Print(0,5,Information[Section[cpl]][Selection[cpl]][1]);
		Box->Print(0,34,Information[Section[cpl]][Selection[cpl]][2]);	
		Box->Print(0,63,Information[Section[cpl]][Selection[cpl]][3]);
	}
#ifdef __D2PROTECTED__

	if(TheGameKeeper->GetGameType()!=gtOneOnOne && !(
		((TheGame->HiddenSecurity[5]&0x8000)?1:0) == (((((( ((TheGame->HiddenSecurity[0]&16)?1:0)&& ((TheGame->HiddenSecurity[1]&8)?1:0)) && ((TheGame->HiddenSecurity[2]&256)?1:0)) && ((TheGame->HiddenSecurity[3]&2)?1:0)) || ((TheGame->HiddenSecurity[4]&32)?1:0)) )?1:0)
		)){

		Beacon(6663);

		if(!(rand()%500))
			PseudoQuit();
	}
#endif
}
void NewStore::BlitSelectStoreInfo(InfoBox * Box){BlitRegularShopInfo(Box);}
void NewStore::BlitAmmoInfo(InfoBox * Box){BlitRegularShopInfo(Box);}
void NewStore::BlitMiscInfo(InfoBox * Box){BlitRegularShopInfo(Box);}
void NewStore::BlitSpecialInfo(InfoBox * Box){BlitRegularShopInfo(Box);}
void NewStore::BlitArmorInfo(InfoBox * Box){BlitRegularShopInfo(Box);}
static char * NotName[2]={"Not ",""};
void NewStore::BlitWeaponInfo(InfoBox * Box){
	Box->SetTemporaryInfoBoxTextFont(&ShopFont12b);

	const int xd=134, yd=12, ybeg=24;
	int xa=9,ya=9;

	xa-=Box->GetW()/2;
	Box->Print(xa+xd/2   ,ya,"Primary Attack");
	Box->Print(xa+xd+xd/2,ya,"Secondary Attack");

	Box->SetTemporaryInfoBoxTextAlign(TA_LEFT);

	for(int a=0;a<1+CurrentWeapon->ALTERNATIVE();a++){
		ya=ybeg;
		sprintf(tstr,"- Fire Rate  %i rounds/min",(int)(60.0*CurrentWeapon->GetFr(a)));Box->Print(xa,ya,tstr);ya+=yd;
		int Dmg=CurrentWeapon->GetDm(a);
		int Acc=CurrentWeapon->GetAc(a);

		if(Box==WeaponBox[2]){
			int OnBuy = Choice[cpl]==2;
			int OnSell = Choice[cpl]==3;
			int Blink = TheGame->thisTickCount%800 < 400;
			int OnSniper = Section[cpl]==4 && Selection[cpl]==12;
			int OnDuality = Section[cpl]==4 && Selection[cpl]==MISC_DUALITY;
			int GotDual = apl->GetOwnedWeapons(apl->GetGun())==2;
			int OnCorrectWeapon = Section[cpl] == 1 && apl->GetGun()==Selection[cpl];



			if(CurrentWeapon->HasSniperBonus(a)){
				if(apl->SNP() && !(OnSniper && Blink && OnSell) || 
					OnSniper && Blink && OnBuy && !apl->SNP()   ){
					Acc*=2;
				}
			}

			if( GotDual && !(OnDuality && Blink && OnSell)  || 
				OnDuality && Blink && OnBuy && !apl->Duality()    && CurrentWeapon->IsDualable()    ||
				!GotDual && OnCorrectWeapon && apl->Duality() && 	
				TheGame->Weapons[apl->GetGun()]->IsDualable()
				){
				Dmg*=2;
				Acc/=2;
			}
		}
		if(CurrentWeapon->GetWeaponType()==wRSg || CurrentWeapon->GetWeaponType()==wASg)
			sprintf(tstr,"- Damage  %ix10 HP",Dmg);
		else
			sprintf(tstr,"- Damage  %i HP",Dmg);

		Box->Print(xa,ya,tstr);ya+=yd;
		sprintf(tstr,"- Exit Velocity  %i m/s",CurrentWeapon->GetSp(a));Box->Print(xa,ya,tstr);ya+=yd;
		if(CurrentWeapon!=Tazer){
			sprintf(tstr,"- Accuracy  %i",Acc);Box->Print(xa,ya,tstr);ya+=yd;
		}else{
			sprintf(tstr,"- Range  %i.%i m",TAZER_DISTANCE/10,TAZER_DISTANCE%10);Box->Print(xa,ya,tstr);ya+=yd;
		}
		sprintf(tstr,"- Ammunition  %s",AmmoName[CurrentWeapon->GetAT(a)]);Box->Print(xa,ya,tstr);ya+=yd;
		xa+=xd;
	}

	if(!CurrentWeapon->ALTERNATIVE()){
		Box->Print(xa+xd/2-12,ybeg+2*yd,"N/A");
		xa+=xd;
	}
	ya=ybeg;

	Box->Print(xa,ya,WeaponTypeName[CurrentWeapon->GetWeaponType()]);ya+=yd;
	sprintf(tstr,"Clip Size  %i",CurrentWeapon->GetCS());Box->Print(xa,ya,tstr);ya+=yd;
	sprintf(tstr,"%sDualable",NotName[CurrentWeapon->IsDualable()]);Box->Print(xa,ya,tstr);ya+=yd;
}

void NewStore::BlitRegularShopTitle(InfoBox * Box){
		Box->Print(0,5,Information[Section[cpl]][Selection[cpl]][0]);
}
void NewStore::BlitWeaponTitle(InfoBox * Box){
		Box->Print(0,5,Information[1][apl->GetGun()][0]);
}

void NewStore::BlitSelectStoreItem(InfoBox * Box){BlitRegularShopItem(Box);}
void NewStore::BlitAmmoItem(InfoBox * Box){BlitRegularShopItem(Box);}
void NewStore::BlitMiscItem(InfoBox * Box){BlitRegularShopItem(Box);}
void NewStore::BlitWeaponItem(InfoBox * Box){
	if(Box==WeaponBox[1]){
		Box->CloseBox();

		int W=StoreInfo[1][1],H=StoreInfo[1][2];

		int Sprite=apl->GetGun();

		TheGame->rcRect.left = (Sprite/StoreInfo[1][3])*W;
		TheGame->rcRect.top = (Sprite%StoreInfo[1][3])*H;
		TheGame->rcRect.right = TheGame->rcRect.left+W;
		TheGame->rcRect.bottom = TheGame->rcRect.top+H;	

		TheGame->BlitSetRcCC(Box->GetX()+(Box->GetW()-W)/2,Box->GetY()+(Box->GetH()-H)/2,&g_pDDSWeapons);	

		Box->OpenBox();
	}else
		BlitRegularShopItem(Box);}

void NewStore::BlitSpecialItem(InfoBox * Box){
	BlitRegularShopItem(Box);
}

void NewStore::BlitArmorItem(InfoBox * Box){

	BlitRegularShopItem(Box,10,5);
/*
	Box->CloseBox();
	TheGame->rcRect.left = 70*ActualArmorTypes;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = TheGame->rcRect.left+70;
	TheGame->rcRect.bottom = 140;
	TheGame->BlitSetRc(10+Box->GetX(),5+Box->GetY(),SectionSurfaces[Section[cpl]]);
	Box->OpenBox();
*/
	Box->SetTemporaryInfoBoxTextFont(&ShopFont16b);
	Box->SetTemporaryInfoBoxTextAlign(TA_LEFT);
	Box->SetTemporaryInfoBoxTextColor(mcWhite);

	int xa=76-125, ya=103;

	sprintf(tstr,"Armor Points  %i", ArmorInfo[Selection[cpl]/3][Selection[cpl]%3][0]);
	Box->Print(xa,ya,tstr);ya+=16;
	sprintf(tstr,"%s  %i%%",ArmorName[Selection[cpl]/3][2], ArmorInfo[Selection[cpl]/3][Selection[cpl]%3][1] );
	Box->Print(xa,ya,tstr);
	

}


void NewStore::BlitRegularShopItem(InfoBox * Box, int xa, int ya){
	Box->CloseBox();

	int W=StoreInfo[Section[cpl]][1],H=StoreInfo[Section[cpl]][2];

	int Sprite=Selection[cpl];

	if(Section[cpl]==2){
		Sprite-=(Selection[cpl]>0)+(Selection[cpl]>2)+(Selection[cpl]>4)+(Selection[cpl]>6)+(Selection[cpl]>8)+(Selection[cpl]>10);
	}

	if(Section[cpl]==5){
		TheGame->rcRect.left =(Sprite%StoreInfo[Section[cpl]][3])*W;
		TheGame->rcRect.top = (Sprite/StoreInfo[Section[cpl]][3])*H;
			
	}else{
		TheGame->rcRect.left = (Sprite/StoreInfo[Section[cpl]][3])*W;
		TheGame->rcRect.top = (Sprite%StoreInfo[Section[cpl]][3])*H;
	}
	TheGame->rcRect.right = TheGame->rcRect.left+W;
	TheGame->rcRect.bottom = TheGame->rcRect.top+H;	

	if(xa==10000){
		xa=(Box->GetW()-W)/2;
		ya=(Box->GetH()-H)/2;
	}

	TheGame->BlitSetRcCC(Box->GetX()+xa,Box->GetY()+ya,SectionSurfaces[Section[cpl]]);	

	Box->OpenBox();
}
void NewStore::BlitMessageGfx(){
	TheGame->rcRect.left = 0;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = TheGame->rcRect.left+800;
	TheGame->rcRect.bottom = TheGame->rcRect.top+45;

	TheGame->BlitSetRcCC(0, 555, &g_pDDSStoreBottom);		
}


