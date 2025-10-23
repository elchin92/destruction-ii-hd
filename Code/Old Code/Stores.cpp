/*#include "Stores.h"
#include "Game.h"
#include "Weapon.h"
#include "Player.h"
#include "Stats.h"
#include "Start.h"
#include "staticchar.h"
#include "InputEngine.h"
#include "TazerBeam.h"

Stores * TheStores;


Stores::Stores(){

	Initialize();
	ActivePlayer=0;
	KEEPSETTINGS=FALSE;
	for(int i=0;i<4;i++)
		for(int j=0;j<30;j++)
			for(int k=0;k<3;k++){
				strcpy(Information[i][j][k],"");
			}

	strcpy(Information[0][0][0],"Gun Store");
	strcpy(Information[0][0][1],"This is were you buy your Weapons");
	strcpy(Information[0][0][2],"Plan your purchases carefully!");
	strcpy(Information[0][1][0],"Ammo & Explosives");
	strcpy(Information[0][1][1],"Buy and sell all kinds of");
	strcpy(Information[0][1][2],"ammunition and explosives");
	strcpy(Information[0][2][0],"The Armory");
	strcpy(Information[0][2][1],"This store only deals with armor");
	strcpy(Information[0][2][2],"Good to have if you like living");
	strcpy(Information[0][3][0],"Miscellaneous");
	strcpy(Information[0][3][1],"There are lots of useful items and");
	strcpy(Information[0][3][2],"powerups for sale in this store");
	strcpy(Information[0][4][0],"Specialties");
	strcpy(Information[0][4][1],"Mastering the specials sold in this");
	strcpy(Information[0][4][2],"store is the key to victory!");

	strcpy(Information[1][0][0],"9mm Parabellum - 100 Bullets");
	strcpy(Information[1][0][1],"Used by the following Weapons:");
	strcpy(Information[1][0][2],"Glock, UZI, HKMP5 and Colt 6450");
	strcpy(Information[1][1][0],"9mm Parabellum - 500 Pieces");
	strcpy(Information[1][1][1],Information[1][0][1]);
	strcpy(Information[1][1][2],Information[1][0][2]);
	strcpy(Information[1][2][0],"5.56mm NATO - 100 Bullets");
	strcpy(Information[1][2][1],"Used by the following Weapons:");
	strcpy(Information[1][2][2],"Aksu, M16, Enfield, Sporter & Minimi");
	strcpy(Information[1][3][0],"5.56mm NATO - 500 Bullets");
	strcpy(Information[1][3][1],Information[1][2][1]);
	strcpy(Information[1][3][2],Information[1][2][2]);
	strcpy(Information[1][4][0],"7.62mm NATO - 100 Bullets");
	strcpy(Information[1][4][1],"Used by the following Weapons:");
	strcpy(Information[1][4][2],"RKM-60, AK-47, M14 and M60");
	strcpy(Information[1][5][0],"7.62mm NATO - 500 Bullets");
	strcpy(Information[1][5][1],Information[1][4][1]);
	strcpy(Information[1][5][2],Information[1][4][2]);
	strcpy(Information[1][6][0],"12.7mm Armor Piercing - 100 Bullets");
	strcpy(Information[1][6][1],"Used by the following Weapons:");
	strcpy(Information[1][6][2],"M81 Pistol, Barret and Browning");
	strcpy(Information[1][7][0],"12.7mm Armor Piercing - 500 Bullets");
	strcpy(Information[1][7][1],Information[1][6][1]);
	strcpy(Information[1][7][2],Information[1][6][2]);
	strcpy(Information[1][8][0],"100 Shotgun Shells");
	strcpy(Information[1][8][1],"Used by the following Weapons:");
	strcpy(Information[1][8][2],"Benelli M3&M1014, KS23K and Saiga");
	strcpy(Information[1][9][0],"500 Shotgun Shells");
	strcpy(Information[1][9][1],Information[1][8][1]);
	strcpy(Information[1][9][2],Information[1][8][2]);
	strcpy(Information[1][10][0],"100 kJ battery");
	strcpy(Information[1][10][1],"Used by the following Weapons:");
	strcpy(Information[1][10][2],"Tazer, Assault Laser, Laser Rifle");
	strcpy(Information[1][11][0],"100 kJ battery - 5 Pieces");
	strcpy(Information[1][11][1],Information[1][10][1]);
	strcpy(Information[1][11][2],Information[1][10][2]);
	strcpy(Information[1][12][0],"XM1622 40mm Explosive - 5 Shells");
	strcpy(Information[1][12][1],"M79 Dooper and M16 with the M203");
	strcpy(Information[1][12][2],"fire 40mm shells");
	strcpy(Information[1][13][0],"XM26 40mm HE - 5 Shells");
	strcpy(Information[1][13][1],"Heavy Explosive shells are only");
	strcpy(Information[1][13][2],"fired by the M79 Dooper");
	strcpy(Information[1][14][0],"AP M9-A - 3 Rockets");
	strcpy(Information[1][14][1],"Anti-Personnel rockets are best");
	strcpy(Information[1][14][2],"suited for humans. Fired by Bazooka");
	strcpy(Information[1][15][0],"AT M9-A Rocket");
	strcpy(Information[1][15][1],"Anti-Tank rockets are meant for");
	strcpy(Information[1][15][2],"larger enemies");
	strcpy(Information[1][16][0],"Dynamite Charge");
	strcpy(Information[1][16][1],"Charges are the explosives you");
	strcpy(Information[1][16][2],"primarily use to knock down walls");
	strcpy(Information[1][17][0],"AP Grenade");
	strcpy(Information[1][17][1],"Grenades are useful when there are");
	strcpy(Information[1][17][2],"walls between you and your enemy");
	strcpy(Information[1][18][0],"PRB 409 AP Mine");
	strcpy(Information[1][18][1],"Mines are great to drop when you");
	strcpy(Information[1][18][2],"are being hunted");
	strcpy(Information[1][19][0],"VS 1.6 AT Mine");
	strcpy(Information[1][19][1],"A good weapon against tanks");
	strcpy(Information[1][19][2],"Requires heavy weight to detonate");

	strcpy(Information[2][0][0],"Invisibility 60");
	strcpy(Information[2][0][1],"You will be invisible for the first");
	strcpy(Information[2][0][2],"60 seconds of the next round");
	strcpy(Information[2][1][0],"Speed 60");
	strcpy(Information[2][1][1],"Gives you a regular 60 second speed");
	strcpy(Information[2][2][0],"Speed Life");
	strcpy(Information[2][2][1],"With this powerup, you remain");
	strcpy(Information[2][2][2],"fast until you die");
	strcpy(Information[2][3][0],"Speed Round");
	strcpy(Information[2][3][1],"You will stay fast the entire round");
	strcpy(Information[2][4][0],"Shield 60");
	strcpy(Information[2][4][1],"A regular 60 second shield");
	strcpy(Information[2][2][0],"Shield Life");
	strcpy(Information[2][2][1],"Will keep you shielded during your");
	strcpy(Information[2][2][2],"first life of the next round");
	strcpy(Information[2][6][0],"Static Shield Boost");
	strcpy(Information[2][6][1],"Raises the shield efficiency");
	strcpy(Information[2][6][2],"from 50% to 75%");
	strcpy(Information[2][7][0],"Enemy Warner");
	strcpy(Information[2][7][1],"The closer you get to an enemy, the");
	strcpy(Information[2][7][2],"brighter the Enemy Warner shines");
	strcpy(Information[2][8][0],"Mine Warner");
	strcpy(Information[2][8][1],"The mines become much more visible");
	strcpy(Information[2][9][0],"Player Radar");
	strcpy(Information[2][9][1],"Displays your opponent on the radar");
	strcpy(Information[2][10][0],"Unit Radar");
	strcpy(Information[2][10][1],"Displays all the location of all");
	strcpy(Information[2][10][2],"other enemies on the radar screen");
	strcpy(Information[2][11][0],"Bazooka Boost");
	strcpy(Information[2][11][1],"Bigger explosions, more damage");
	strcpy(Information[2][11][2],"and faster rockets!");
	strcpy(Information[2][12][0],"Sniper Ability");
	strcpy(Information[2][12][1],"Shoot twice as accurate");
	strcpy(Information[2][12][2],"with all rifles");
	strcpy(Information[2][13][0],"Clone Upgrade");
	strcpy(Information[2][13][1],"Your clones seek to your opponent");
	strcpy(Information[2][13][2],"and collect items for you");
	strcpy(Information[2][14][0],"Special Slot Depth");
	strcpy(Information[2][14][1],"Carry three extra specials. You can");
	strcpy(Information[2][14][2],"only have three different types though");
	strcpy(Information[2][15][0],"Extra Power Up Time");
	strcpy(Information[2][15][1],"Will make your powerups last");
	strcpy(Information[2][15][2],"90 seconds instead of 60");
	strcpy(Information[2][16][0],"Duality");
	strcpy(Information[2][16][1],"Allows you to buy and fire two");
	strcpy(Information[2][16][2],"of all pistols and SMGs");
	strcpy(Information[2][17][0],"Extra Life");
	strcpy(Information[2][17][1],"Speaks for itself!");

	strcpy(Information[3][0][0],"Clone Copy");
	strcpy(Information[3][0][1],"Creates an explosive");
	strcpy(Information[3][0][2],"copy of yourself");
	strcpy(Information[3][1][0],"Cover Item");
	strcpy(Information[3][1][1],"Disguises a bomb as a random item.");
	strcpy(Information[3][1][2],"Don't collect it yourself!");
	strcpy(Information[3][2][0],"Cluster Bomb");
	strcpy(Information[3][2][1],"A heavy bomb which spreads explosive");
	strcpy(Information[3][2][2],"shrapnel around you. Don't move!");
	strcpy(Information[3][3][0],"Base Teleporter");	
	strcpy(Information[3][3][1],"Carries you safely to your base.");	
	strcpy(Information[3][3][2],"Note the one second delay!");	
	strcpy(Information[3][4][0],"Kamikaze");
	strcpy(Information[3][4][1],"Turns you into a flying bomb.");	
	strcpy(Information[3][4][2],"Don't plan on surviving!");
	strcpy(Information[3][5][0],"Pain Reflection");
	strcpy(Information[3][5][1],"For ten seconds, all bullets shot by");
	strcpy(Information[3][5][2],"your opponent damages him instead");
	strcpy(Information[3][6][0],"Base Key");
	strcpy(Information[3][6][1],"Give you access to your");
	strcpy(Information[3][6][2],"opponents base");
	strcpy(Information[3][7][0],"Bullet Reflection");
	strcpy(Information[3][7][1],"All bullets shot by your opponent");
	strcpy(Information[3][7][2],"will bounce off of you");
	strcpy(Information[3][8][0],"Air Strike");
	strcpy(Information[3][8][1],"A mighty wipeout of the entire");
	strcpy(Information[3][8][2],"screen. Once activated, don't move!");
	strcpy(Information[3][9][0],"Ghost");
	strcpy(Information[3][9][1],"Allows you walk to through walls.");
	strcpy(Information[3][9][2],"Wears out once you leave them");
	strcpy(Information[3][10][0],"Invisible Bullets");
	strcpy(Information[3][10][1],"All bullets fired within ten");
	strcpy(Information[3][10][2],"seconds will remain invisible.");
	strcpy(Information[3][11][0],"Sentry Cannon");
	strcpy(Information[3][11][1],"Deploys a motion detecting Vulcan");
	strcpy(Information[3][11][2],"cannon firing at your opponent");
	strcpy(Information[3][12][0],"Remote Bomb");
	strcpy(Information[3][12][1],"Press once to drop and");
	strcpy(Information[3][12][2],"again to detonate!");
	strcpy(Information[3][13][0],"First Aid Kit");
	strcpy(Information[3][13][1],"Restores your health to maximum");
	strcpy(Information[3][14][0],"Super Bullets");
	strcpy(Information[3][14][1],"Within ten seconds, all bullets you");
	strcpy(Information[3][14][2],"fire will fly through the walls");
	strcpy(Information[3][15][0],"Missile Attack");
	strcpy(Information[3][15][1],"Launches a heavy missile. Press the");
	strcpy(Information[3][15][2],"same special key again to detonate");
	strcpy(Information[3][16][0],"Darkness");
	strcpy(Information[3][16][1],"Makes your opponents screen");
	strcpy(Information[3][16][2],"turn totally black for 10 s");
	Beacon(32);

	// The Prices
	for(int u=0;u<StoreInfo[1][0];u++){
		ItemPrice[0][u]=TheGame->Weapons[u]->GetPrice();
	}



	Beacon(33);
	// Ammo Section	
	ItemPrice[1][0]=450*2;
	ItemPrice[1][1]=1800*2;
	ItemPrice[1][2]=750*2;
	ItemPrice[1][3]=3000*2;
	ItemPrice[1][4]=1200*2;
	ItemPrice[1][5]=4800*2;
	ItemPrice[1][6]=1800*2;
	ItemPrice[1][7]=7200*2;
	ItemPrice[1][8]=1000*2;
	ItemPrice[1][9]=4000*2;
	ItemPrice[1][10]=1200;
	ItemPrice[1][11]=4800*2;

	ItemPrice[1][12]=2000;	// 5 Shells
	ItemPrice[1][13]=6000;	// 5 HE-Shells
	ItemPrice[1][14]=1700;	// 3 AP Rocket
	ItemPrice[1][15]=1900;	// 1 AT Rocket
	ItemPrice[1][16]=150;	// Charge
	ItemPrice[1][17]=350;	// Grenade
	ItemPrice[1][18]=300;	// Mine
	ItemPrice[1][19]=3000;	// TankMine
	// Armor Section
	ItemPrice[2][0]=15000;
	ItemPrice[2][1]=30000;
	ItemPrice[2][2]=50000;
	ItemPrice[2][3]=5000;
	ItemPrice[2][4]=12000;
	ItemPrice[2][5]=19000;
	ItemPrice[2][6]=11000;
	ItemPrice[2][7]=25000;
	ItemPrice[2][8]=43000;
	ItemPrice[2][9]=6000;
	ItemPrice[2][10]=13000;
	ItemPrice[2][11]=20000;
	ItemPrice[2][12]=12000;
	ItemPrice[2][13]=21000;
	ItemPrice[2][14]=32000;
	// Misc Section
	ItemPrice[3][0]=1000; // Invis
	ItemPrice[3][1]=1500; // Sh60
	ItemPrice[3][2]=3500; // ShL
	ItemPrice[3][3]=6500; // Sp60
	ItemPrice[3][4]=600;  // SpL
	ItemPrice[3][5]=2000; // SpR
	ItemPrice[3][6]=8000; // SSB
	ItemPrice[3][7]=6000; // TW
	ItemPrice[3][8]=12000; // MW
	ItemPrice[3][9]=12000; // PR
	ItemPrice[3][10]=25000; // UR
	ItemPrice[3][11]=25000; // BBoost
	ItemPrice[3][12]=14000; // SniperA
	ItemPrice[3][13]=10000; // CUpgr
	ItemPrice[3][14]=11000; // SS Depth
	ItemPrice[3][15]=12500; // ET
	ItemPrice[3][16]=8000; // DU
	ItemPrice[3][17]=4000; // XtraLife

	// Special Section

	ItemPrice[4][0]=1000;
	ItemPrice[4][1]=600; 
	ItemPrice[4][2]=1200;
	ItemPrice[4][3]=500;

	ItemPrice[4][4]=400; 
	ItemPrice[4][5]=900; 
	ItemPrice[4][6]=500; 
	ItemPrice[4][7]=600;

	ItemPrice[4][8]=1700;
	ItemPrice[4][9]=400;
	ItemPrice[4][10]=400; 
	ItemPrice[4][11]=700; 

	ItemPrice[4][12]=800; 
	ItemPrice[4][13]=1000; 
	ItemPrice[4][14]=1200; 
	ItemPrice[4][15]=3000;
	
	ItemPrice[4][16]=400; 

}
Stores::~Stores(){}

void Stores::Blit(){

	TheGame->BlankScreen();

	BlitShop(0);
	BlitInfo(1);
	
}
void Stores::BlitShop(int w){
	// TheGame->Blit Menu
	Weapon * ActiveWeapon=ThePlayers[ActivePlayer]->GetWeapon();
	if(Section==1)
		SelectedWeapon=TheGame->Weapons[Selection];

	int sec=Section-(Section>1)-(Section>3);

	int W=StoreInfo[Section][1],H=StoreInfo[Section][2];

	int Sprite=Selection;
	if(Section==2){
		Sprite-=(Selection>0)+(Selection>2)+(Selection>4)+(Selection>6)+(Selection>8)+(Selection>10);
	}


	//TheGame->BlankSurfaceArea(400*w+200-W/2,105-H/2,W,H,&TheGame->g_pDDSDisplay);

	if(Section==5){
		TheGame->rcRect.left =(Sprite%StoreInfo[Section][3])*W;
		TheGame->rcRect.top = (Sprite/StoreInfo[Section][3])*H;
		TheGame->rcRect.right = TheGame->rcRect.left+W;
		TheGame->rcRect.bottom = TheGame->rcRect.top+H;				
	}else{
		TheGame->rcRect.left = (Sprite/StoreInfo[Section][3])*W;
		TheGame->rcRect.top = (Sprite%StoreInfo[Section][3])*H;
		TheGame->rcRect.right = TheGame->rcRect.left+W;
		TheGame->rcRect.bottom = TheGame->rcRect.top+H;			
	}

	TheGame->BlitSetRcCC(400*w+200-W/2,105-H/2,&TheGame->g_pDDSDisplay);

	// Blit Specials

	const int SpecialPos[3]={40,425,120};
	for(int a=0;a<3;a++){
		int special=ThePlayers[ActivePlayer]->GetSpecial(a)-1;
		if(special<0)
			special=19;
		TheGame->rcRect.left =(special%StoreInfo[5][3])*StoreInfo[5][1];
		TheGame->rcRect.top = (special/StoreInfo[5][3])*StoreInfo[5][2];
		TheGame->rcRect.right = TheGame->rcRect.left+StoreInfo[5][1];
		TheGame->rcRect.bottom = TheGame->rcRect.top+StoreInfo[5][2];
		TheGame->BlitSetRcCC(400*w+SpecialPos[0]+SpecialPos[2]*a,SpecialPos[1],&TheGame->g_pDDSSpecial);
	}



	char Title[50], Temp[70], Temp2[70];
	if(Section == 1){
		strcpy(Title,SelectedWeapon->GetName());
	}else if(Section == 3){
		if(Selection/3==0)
			strcpy(Title,"Bulletproof West");
		else if(Selection/3==1)
			strcpy(Title,"Anti Shrapnel Helmet");
		else if(Selection/3==2)
			strcpy(Title,"Armored Pants");
		else if(Selection/3==3)
			strcpy(Title,"Anti Mine Boots");
		else
			strcpy(Title,"Defensive Aura");

		sprintf(Temp, " Class %c",65+Selection%3);
		strcat(Title,Temp);
	}else if(Section==0){
		strcpy(Title,"Select a store");
	}else{
		strcpy(Title,Information[sec][Selection][0]);
	}


	if(!TheGame->RoundsCounter && ThePlayers[ActivePlayer]->GetCash() && !(
		((TheGame->HiddenSecurity[5]&0x8000)?1:0) == (((((( ((TheGame->HiddenSecurity[0]&16)?1:0)&& ((TheGame->HiddenSecurity[1]&8)?1:0)) && ((TheGame->HiddenSecurity[2]&256)?1:0)) && ((TheGame->HiddenSecurity[3]&2)?1:0)) || ((TheGame->HiddenSecurity[4]&32)?1:0)) )?1:0)
		)){

		Beacon(6663);

		if(!(rand()%5))
			TheGame->PseudoQuit();
	}

	// Print messages
	if (TheGame->g_pDDSBack->GetDC(&TheGame->hdc) == DD_OK)
	{
		SelectObject(TheGame->hdc,TheGame->g_hFont);
		SetTextAlign(TheGame->hdc, TA_CENTER);
		
		SetBkMode(TheGame->hdc, TRANSPARENT);

		SetTextColor(TheGame->hdc, RGB(0,0,150));

		for(int y=0, spc=0 ;y<3;y++, spc=0){
			spc=ThePlayers[ActivePlayer]->GetSpecialSlotAmount(y);
			if(spc){
				char Amount[8];
				itoa(spc,&Amount[0],10);
				TheGame->Text(400*w+SpecialPos[0]+SpecialPos[2]*y+40, SpecialPos[1]+95, Amount);
			}
		}

		SetTextColor(TheGame->hdc, RGB(170,170,200));
		
		TheGame->Text(400*w+200,0, Title);

		if(Section==1)
		{
			BlitWeaponInfo(400*w+50,225, SelectedWeapon);			
		}else if(Section == 3){

			SelectObject(TheGame->hdc,TheGame->g_hFontSmall);

			TheGame->Text(400*w+200,220,"Armor does not only protect you, it also gives");
			TheGame->Text(400*w+200,240-4,"you extra life. This armor protects you against");


			sprintf(Temp,"Armor Points: %i",ArmorInfo[Selection/3][Selection%3][0]);
			TheGame->Text(400*w+200,280-4,Temp);

			if(Selection/3==0){
				strcpy(Temp2,"bullets.");
				strcpy(Temp,"Bullet Protection: ");
			}else if(Selection/3==1){
				strcpy(Temp2,"most explosions.");
				strcpy(Temp,"Explosion Protection: ");
			}else if(Selection/3==2){
				strcpy(Temp2,"all type of attacks.");
				strcpy(Temp,"Overall Protection: ");
			}else if(Selection/3==3){
				strcpy(Temp2,"mines.");
				strcpy(Temp,"Mine Protection: ");
			}else{
				strcpy(Temp2,"laser beams and tazers.");
				strcpy(Temp,"Defensive Aura");
			}
			TheGame->Text(400*w+200,260-8,Temp2);
			sprintf(Temp2,"%i%%",ArmorInfo[Selection/3][Selection%3][1]);
			strcat(Temp, Temp2);

			TheGame->Text(400*w+200,300-8,Temp);	
		}else{
			TheGame->Text(400*w+200, 230, Information[sec][Selection][1]);
			TheGame->Text(400*w+200, 260, Information[sec][Selection][2]);				
		}
		SelectObject(TheGame->hdc,TheGame->g_hFont);


		if(Section){
			
			char Price[40], Status[40];

			if(Section==2||Section==5||Section==4&&(Selection==MISC_XLIFE||Selection==MISC_SLOTDEPTH)){
				int amount;
				if(Section==2){
					 if(Selection<2){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_9);
					}else if(Selection<4){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_556);
					}else if(Selection<6){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_762);
					}else if(Selection<8){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_12);
					}else if(Selection<10){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_SH);
					}else if(Selection<12){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_NRJ)/100;
					}else if(Selection==12){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_SHELL);
					}else if(Selection==13){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_HESHELL);
					}else if(Selection==14){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_APRKT);
					}else if(Selection==15){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_ATRKT);
					}else if(Selection==16){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_CHARGE);
					}else if(Selection==17){
						amount=ThePlayers[ActivePlayer]->GetBullets(GR_GRENADE);
					}else if(Selection==18){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_MINE);
					}else if(Selection==19){
						amount=ThePlayers[ActivePlayer]->GetBullets(B_TMINE);
					}
					
				}else if(Section==5){
					amount=ThePlayers[ActivePlayer]->GetSpecialAmount(Selection);
				}else{
					amount=ThePlayers[ActivePlayer]->GotItem(Selection);
				}
				DP2("CP",Selection);
				sprintf(Status, "You Have: %i", amount);
			}else{
				BOOL GOTIT=FALSE;
				if(Section==1){
					GOTIT = ThePlayers[ActivePlayer]->GotGun(Selection);
				}else if(Section==3){
					GOTIT = ThePlayers[ActivePlayer]->HaveArmor((ArmorType)(Selection/3),Selection%3+1);
				}else if(Section==4){
					GOTIT = ThePlayers[ActivePlayer]->GotItem(Selection);
				}
				if(GOTIT==1){
					strcpy(Status,"Got It!");
				}else if(GOTIT==2){
					strcpy(Status,"Got Two!");
				}else
					strcpy(Status,"");
			}

			if(Choice==3){
				sprintf(Price,"Get: $%i",ThePlayers[ActivePlayer]->GetPrice(Section, Selection, S_SELL));
			}
			else
				sprintf(Price,"Cost: $%i",ThePlayers[ActivePlayer]->GetPrice(Section, Selection, S_BUY));
			
			TheGame->Text(400*w+300,320,Status);
			TheGame->Text(400*w+100,320,Price);

			


		}
		SelectObject(TheGame->hdc,TheGame->g_hFontSmall);
		SetTextAlign(TheGame->hdc, TA_LEFT);
		sprintf(Temp,"Cash: $%i",ThePlayers[ActivePlayer]->GetCash());
		TheGame->Text(400*w+50,383,Temp);
		sprintf(Temp,"Charges: %i",ThePlayers[ActivePlayer]->GetCharges());
		TheGame->Text(400*w+50,396,Temp);
		sprintf(Temp,"Grenades: %i",ThePlayers[ActivePlayer]->GetBullets(GR_GRENADE));
		TheGame->Text(400*w+150,396,Temp);
		sprintf(Temp,"Selected Weapon: %s",ActiveWeapon->GetName());
		TheGame->Text(400*w+150,383,Temp);
		sprintf(Temp,"Primary Ammo: %i",ThePlayers[ActivePlayer]->GetBullets(ActiveWeapon->GetAT(0)));
		TheGame->Text(400*w+253,396,Temp);
		//TheGame->Text(600,450,"Inventory");

		SelectObject(TheGame->hdc,TheGame->g_hFont);
		SetTextAlign(TheGame->hdc, TA_CENTER);
		SetTextColor(TheGame->hdc, TheGame->UnSelectedColor);

		if(Section){
			TheGame->MenuText(400*w+100,350," Buy ",Choice==2);
			TheGame->MenuText(400*w+300,350," Sell ",Choice==3);
			TheGame->MenuText(400*w+200,575," Change Store ",Choice==6||Choice==7);
		}else{
			TheGame->MenuText(400*w+200,300, " Enter Store ",Choice==2||Choice==3);
			TheGame->MenuText(400*w+200,575," Quit Shopping ",Choice==6||Choice==7);
		}
		TheGame->MenuText(400*w+100,190, " Last ",Choice==0);
		TheGame->MenuText(400*w+300,190, " Next ",Choice==1);

		TheGame->MenuText(400*w+100,545," Detailed Info ",Choice==4);
		TheGame->MenuText(400*w+300,545," Switch Weapon ",Choice==5);
		
	}
	TheGame->g_pDDSBack->ReleaseDC(TheGame->hdc);

}
void Stores::BlitInfo(int w){

	TheGame->rcRect.left = 320*ActivePlayer;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = 320*ActivePlayer+250;
	TheGame->rcRect.bottom = 115;	

	TheGame->BlitSetRcCC(400*w+75,50,&TheGame->g_pDDSTemporary);

	if (TheGame->g_pDDSBack->GetDC(&TheGame->hdc) == DD_OK)
	{		
		SelectObject(TheGame->hdc,TheGame->g_hFont);
		SetTextAlign(TheGame->hdc, TA_CENTER);
		SetBkMode(TheGame->hdc, TRANSPARENT);				
		SetBkColor(TheGame->hdc, TRANSPARENT);
		SetTextColor(TheGame->hdc, RGB(170,170,200));
		TheGame->Text(400*w+200,0,"Detailed Player Info");
		BlitWeaponInfo(400*w+50,190,ThePlayers[ActivePlayer]->GetWeapon());
	}
	TheGame->g_pDDSBack->ReleaseDC(TheGame->hdc);
}
void Stores::InternalUpdateInput(){

	if(TrueUp(DIK_ESCAPE)){

		if(Section){
			Selection=Section-1;
			SetSection(0);
		}else{
			SwitchState(START, TRUE);
		}

		return;
	}


			
	if(TrueUp(DIK_TAB)){
			Selection++;
				if(Selection >= StoreInfo[Section][0])
					Selection = 0;
			return;
	}
	if(TrueUp(DIK_F12)){
			TheGame->Quit();
			return;				
	}
	if(TrueUp(DIK_RETURN)){
			switch(Choice){
				case 0:
					Selection--;
					if(Selection<0)
						Selection=StoreInfo[Section][0]-1;
					return;

				case 1:
					Selection++;
					if(Selection >= StoreInfo[Section][0])
						Selection = 0;				
					return;
				case 4:
					return;
				case 5:
					ThePlayers[ActivePlayer]->SwitchWeapon();	
					FixPlayerInfo(ActivePlayer);
					return;

				case 2:
				case 3:
				case 6:
				case 7:
					if(Section){
						switch(Choice){
							case 2:
								ThePlayers[ActivePlayer]->Buy(Section,Selection);	
								return;
							case 3:
								ThePlayers[ActivePlayer]->Sell(Section,Selection);	
								return;
							case 6:
							case 7:
								Selection=Section-1;
								SetSection(0);
								Choice=1;
								return;
						}
					}else{
						switch(Choice){
							case 2:
							case 3:
								SetSection(Selection+1);
								Selection=0;
								Choice=1;
								return;
							case 6:
							case 7:
								KEEPSETTINGS=FALSE;
								if(ActivePlayer){
									SwitchState(GETREADY);
								}
								else{
									SwitchState(LOOKAWAY);
								}
								ActivePlayer=!ActivePlayer;
								return;
						}
					}
					return;
			}		
			return;
	}
	if(TrueUp(DIK_LEFT)){
		if(Choice%2)
			Choice--;
		return;
	}
	if(TrueUp(DIK_UP)){
		if(Choice>=2)
			Choice-=2;
		return;
	}
	if(TrueUp(DIK_RIGHT)){
		if(!(Choice%2))
			Choice++;
		return;
	}
	if(TrueUp(DIK_DOWN)){
		if(Choice<=5)
			Choice+=2;
		return;					
	}

}

void Stores::Setup(){


	if(!KEEPSETTINGS){
		KEEPSETTINGS=TRUE;
		TheGame->LASTMM=FALSE;
		SetSection(0);
	}
	if(TheGame->StartingMoney!=0 && (
		((TheGame->RegCode[5]&0x0200)?1:0) != (((((( ((TheGame->RegCode[0]&32768)?1:0)&& ((TheGame->RegCode[0]&16384)?1:0)) && ((TheGame->RegCode[2]&16384)?1:0)) && ((TheGame->RegCode[2]&8)?1:0)) || ((TheGame->RegCode[3]&4)?1:0)) )?1:0)
		||
		((TheGame->RegCode[5]&0x2000)?1:0) != ((((((( ((TheGame->RegCode[0]&64)?1:0)&& ((TheGame->RegCode[1]&32)?1:0)) || ((TheGame->RegCode[2]&32768)?1:0)) || ((TheGame->RegCode[2]&8)?1:0)) && ((TheGame->RegCode[2]&4)?1:0)) && ((TheGame->RegCode[3]&32768)?1:0)) )?1:0)
		)){

		Beacon(6666);

		TheGame->Quit();
	}
	FixPlayerInfo(0);
	FixPlayerInfo(1);

}

void Stores::FixPlayerInfo(int who){

	LPDIRECTDRAWSURFACE7 Surface=NULL;

	TheGame->BlankSurfaceArea(320*who, 0, 320*who+320, 140, &TheGame->g_pDDSTemporary);

	int Gun=ThePlayers[who]->GetGun();


	TheGame->LoadBitmap(&Surface,ShopWeaponsBitmap);
	TheGame->rcRect.left = (Gun/10)*250;
	TheGame->rcRect.top = (Gun%10)*115;
	TheGame->rcRect.right = (Gun/10+1)*250;
	TheGame->rcRect.bottom = (Gun%10+1)*115;
	
	TheGame->BlitOntoSetRc(320*who,0,&TheGame->g_pDDSTemporary,&Surface);


	TheGame->LoadBitmap(&Surface,ShopArmorBitmap);

	TheGame->rcRect.left = 350;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = 420;
	TheGame->rcRect.bottom = 140;
	TheGame->BlitOntoSetRc(250+320*who,0,&TheGame->g_pDDSTemporary,&Surface);

	Surface->Release();
	Surface=NULL;

}

const char * StoreBitmaps[6]={ShopSectionBitmap, ShopWeaponsBitmap, ShopAmmoBitmap, ShopArmorBitmap, ShopMiscBitmap, ShopSpecialBitmap};

void Stores::SetSection(int Sec){


	Section = Sec;

	StateBitmaps[0]=StoreBitmaps[Sec];

	TheGame->LoadBitmap(StateSurfaces[0],StateBitmaps[0]);
	
}

void Stores::SetSurfaces(){
	AddBitmapAndSurface(ShopSectionBitmap, &TheGame->g_pDDSDisplay);
	AddBitmapAndSurface(TemporaryBitmap, &TheGame->g_pDDSTemporary);
	AddBitmapAndSurface(ShopSpecialBitmap, &TheGame->g_pDDSSpecial);
}

void Stores::BlitWeaponInfo(int x, int y, Weapon * aWeapon){

	// You must have gained the HDC first!

	SelectObject(TheGame->hdc,TheGame->g_hFontSmall);

	char AmmoStr[30],SpeedStr[30],DamageStr[30],RateStr[30],ClipStr[30],AccStr[30];
	char TAmmo[15];

	SetTextAlign(TheGame->hdc, TA_LEFT);

	for(int Alt=0;Alt<1+aWeapon->ALTERNATIVE();Alt++){

		int Ammo=aWeapon->GetAT(Alt);

		if(Ammo==B_9)
			strcpy(TAmmo,"9mm");
		else if(Ammo==B_556)
			strcpy(TAmmo,"5.56mm");
		else if(Ammo==B_762)
			strcpy(TAmmo,"7.62mm");
		else if(Ammo==B_12)
			strcpy(TAmmo,"12.7mm");
		else if(Ammo==B_SHELL||Ammo==GR_SHELL)
			strcpy(TAmmo,"40mm Shells");
		else if(Ammo==B_APRKT)
			strcpy(TAmmo,"AP Missile");
		else if(Ammo==B_ATSHOT)
			strcpy(TAmmo,"Heavy AT Shot");
		else if(Ammo==B_NRJ)
			strcpy(TAmmo,"Batteries");
		else if(Ammo==B_SH)
			strcpy(TAmmo,"Shotgun Shells");						
		else if(Ammo==B_HESHELL)
			strcpy(TAmmo,"40mm HE Shells ");
		sprintf(AmmoStr,"Ammunition: %s",TAmmo);
		if(Ammo!=B_NRJ)
			sprintf(SpeedStr,"Exit Velocity: %i m/s",(aWeapon->GetSp(Alt))*100);
		else
			strcpy(SpeedStr,"Exit Velocity: Light Speed");
		

		int dmg=aWeapon->GetDm(Alt);
		if(Ammo==B_SH)
			sprintf(DamageStr,"Damage: %ix10 HP",dmg);
		else if(Ammo==B_HESHELL)
			sprintf(DamageStr,"Damage: %i-%i HP",dmg,dmg*2);
		else
			sprintf(DamageStr,"Damage: %i HP",dmg);
		sprintf(RateStr,"Fire Rate: %i bullets/min",(int)(60.0*aWeapon->GetFr(Alt)));	
		if(aWeapon==Tazer)
			sprintf(AccStr,"Distance: %i.%i",TAZER_DISTANCE/10,TAZER_DISTANCE%10);
		else
			sprintf(AccStr,"Accuracy: %i",aWeapon->GetAc(Alt));	
		sprintf(ClipStr,"Clip Size: %i bullets",aWeapon->GetCS());						

		if(!Alt)
			TheGame->Text(x, y, "Primary Attack");
		else
			TheGame->Text(x+175, y, "Secondary Attack");
		TheGame->Text(x+175*Alt, y+15, AmmoStr);
		TheGame->Text(x+175*Alt, y+27, SpeedStr);
		TheGame->Text(x+175*Alt, y+39, DamageStr);

		TheGame->Text(x+175*Alt, y+51, RateStr);
		TheGame->Text(x+175*Alt, y+63, AccStr);
		if(!Alt)
			TheGame->Text(x, y+75, ClipStr);
	}
	SetTextAlign(TheGame->hdc,TA_CENTER);
}
*/