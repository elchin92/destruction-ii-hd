#include "NewStore.h"	
#include "Debugger.h"
#include "Game.h"
#include "Weapon.h"
#include "AmmoAndArmorName.h"
#include "InfoBox.h"
#include <stdio.h>



void NewStore::	SetPriceAndInfo(){
	for(int i=0;i<6;i++)
		for(int j=0;j<30;j++)
			for(int k=0;k<4;k++){
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
	strcpy(Information[0][4][0],"Specials");
	strcpy(Information[0][4][1],"Mastering the specials sold in this");
	strcpy(Information[0][4][2],"store is the key to victory!");

	strcpy(Information[2][0][0],"9mm Parabellum - 100 Bullets");
	strcpy(Information[2][0][1],"Used by the following Weapons:");
	strcpy(Information[2][0][2],"Glock 17, UZI, HKMP5 and Colt 6450");
	strcpy(Information[2][1][0],"9mm Parabellum - 500 Pieces");
	strcpy(Information[2][1][1],Information[2][0][1]);
	strcpy(Information[2][1][2],Information[2][0][2]);
	strcpy(Information[2][2][0],"5.56mm NATO - 100 Bullets");
	strcpy(Information[2][2][1],"Used by the following Weapons:");
	strcpy(Information[2][2][2],"Aksu, M16, Enfield,");
	strcpy(Information[2][2][2],"FNFAL Sporter & Minimi");
	strcpy(Information[2][3][0],"5.56mm NATO - 500 Bullets");
	strcpy(Information[2][3][1],Information[2][2][1]);
	strcpy(Information[2][3][2],Information[2][2][2]);
	strcpy(Information[2][4][0],"7.62mm NATO - 100 Bullets");
	strcpy(Information[2][4][1],"Used by the following Weapons:");
	strcpy(Information[2][4][2],"RKM-60, AK-47, M14 and M60");
	strcpy(Information[2][5][0],"7.62mm NATO - 500 Bullets");
	strcpy(Information[2][5][1],Information[2][4][1]);
	strcpy(Information[2][5][2],Information[2][4][2]);
	strcpy(Information[2][6][0],"12.7mm Armor Piercing - 100 Bullets");
	strcpy(Information[2][6][1],"Used by the following Weapons:");
	strcpy(Information[2][6][2],"M81 Pistol, Barret and Browning");
	strcpy(Information[2][7][0],"12.7mm Armor Piercing - 500 Bullets");
	strcpy(Information[2][7][1],Information[2][6][1]);
	strcpy(Information[2][7][2],Information[2][6][2]);
	strcpy(Information[2][8][0],"100 Shotgun Shells");
	strcpy(Information[2][8][1],"Used by the following Weapons:");
	strcpy(Information[2][8][2],"Benelli M3&M1014, KS23K and Saiga");
	strcpy(Information[2][9][0],"500 Shotgun Shells");
	strcpy(Information[2][9][1],Information[2][8][1]);
	strcpy(Information[2][9][2],Information[2][8][2]);
	strcpy(Information[2][10][0],"100 kJ battery");
	strcpy(Information[2][10][1],"Used by the following Weapons:");
	strcpy(Information[2][10][2],"Tazer, Assault Laser, Laser Rifle");
	strcpy(Information[2][11][0],"100 kJ battery - 5 Pieces");
	strcpy(Information[2][11][1],Information[2][10][1]);
	strcpy(Information[2][11][2],Information[2][10][2]);
	strcpy(Information[2][12][0],"XM1622 40mm Explosive - 5 Shells");
	strcpy(Information[2][12][1],"40mm Shells are primarily meant to");
	strcpy(Information[2][12][2],"kill your opponent. The are fired");
	strcpy(Information[2][12][3],"by the M79 Dooper and M16wM203");
	strcpy(Information[2][13][0],"XM26 40mm HE - 5 Shells");
	strcpy(Information[2][13][1],"Heavy Explosive shells have a");
	strcpy(Information[2][13][2],"powerful explosion. They are");
	strcpy(Information[2][13][3],"only fired by the M79 Dooper");
	strcpy(Information[2][14][0],"AP M9-A - 3 Rockets");
	strcpy(Information[2][14][1],"Anti Personnel rockets are best");
	strcpy(Information[2][14][2],"suited for humans. Rockets are");
	strcpy(Information[2][14][3],"only fired by the Bazooka");
	strcpy(Information[2][15][0],"AT M9-A Rocket");
	strcpy(Information[2][15][1],"Anti Tank rockets have very");
	strcpy(Information[2][15][2],"powerful explosions and should be");
	strcpy(Information[2][15][3],"aimed at larger enemies");
	strcpy(Information[2][16][0],"Dynamite Charge");
	strcpy(Information[2][16][1],"Charges are the explosives you");
	strcpy(Information[2][16][2],"primarily use to knock down walls");
	strcpy(Information[2][16][3],"They take two seconds to detonate");
	strcpy(Information[2][17][0],"AP Grenade");
	strcpy(Information[2][17][1],"Grenades are useful when there are");
	strcpy(Information[2][17][2],"walls between you and your opponent");
	strcpy(Information[2][17][3],"since they fly above them.");
	strcpy(Information[2][18][0],"PRB 409 AP Mine");
	strcpy(Information[2][18][1],"Mines are great to drop when you");
	strcpy(Information[2][18][2],"are being hunted. They are very");
	strcpy(Information[2][18][3],"weak against non human enemies");
	strcpy(Information[2][19][0],"VS 1.6 AT Mine");
	strcpy(Information[2][19][1],"Anti Tank mines are great weapon");
	strcpy(Information[2][19][2],"against tanks and other vehicles");
	strcpy(Information[2][19][3],"Requires heavy weight to detonate");

	strcpy(Information[4][0][0],"Invisibility 60");
	strcpy(Information[4][0][1],"You will be invisible for the first");
	strcpy(Information[4][0][2],"60 seconds of the next round");
	strcpy(Information[4][1][0],"Speed 60");
	strcpy(Information[4][1][1],"Gives you a regular 60 second speed");
	strcpy(Information[4][2][0],"Speed Life");
	strcpy(Information[4][2][1],"With this powerup, you will remain");
	strcpy(Information[4][2][2],"fast until you die your first life");
	strcpy(Information[4][3][0],"Speed Round");
	strcpy(Information[4][3][1],"You will stay fast the entire round");
	strcpy(Information[4][4][0],"Shield 60");
	strcpy(Information[4][4][1],"A regular 60 second shield");
	strcpy(Information[4][5][0],"Shield Life");
	strcpy(Information[4][5][1],"Will keep you shielded during your");
	strcpy(Information[4][5][2],"first life of the next round");
	strcpy(Information[4][6][0],"Static Shield Boost");
	strcpy(Information[4][6][1],"Raises the shield efficiency");
	strcpy(Information[4][6][2],"from 50% to 75%");
	strcpy(Information[4][7][0],"Enemy Warner");
	strcpy(Information[4][7][1],"The closer you get to an enemy, the");
	strcpy(Information[4][7][2],"brighter the Enemy Warner shines");
	strcpy(Information[4][8][0],"Mine Warner");
	strcpy(Information[4][8][1],"The mines become much more visible");
	strcpy(Information[4][9][0],"Player Radar");
	strcpy(Information[4][9][1],"Displays your opponent on the radar");
	strcpy(Information[4][10][0],"Unit Radar");
	strcpy(Information[4][10][1],"Displays the locations of all");
	strcpy(Information[4][10][2],"computer enemies in different");
	strcpy(Information[4][10][3],"colors on the radar screen");
	strcpy(Information[4][11][0],"Bazooka Boost");
	strcpy(Information[4][11][1],"The rockets fired by the bazooka");
	strcpy(Information[4][11][2],"fly faster, yield bigger ");
	strcpy(Information[4][11][3],"explosions and cause more damage");
	strcpy(Information[4][12][0],"Sniper Ability");
	strcpy(Information[4][12][1],"With this you will shoot twice as");
	strcpy(Information[4][12][2],"accurate when using any bullet");
	strcpy(Information[4][12][3],"weapon in a non automatic mode");
	strcpy(Information[4][13][0],"Clone Upgrade");
	strcpy(Information[4][13][1],"Your clones will seek towards");
	strcpy(Information[4][13][2],"your opponent and collect");
	strcpy(Information[4][13][3],"items for you");
	strcpy(Information[4][14][0],"Extra Special Slot Depth");
	strcpy(Information[4][14][1],"Allows you to carry one extra special");
	strcpy(Information[4][14][2],"per slot. You can still only have one");
	strcpy(Information[4][14][3],"type of special in each slot though");
	strcpy(Information[4][15][0],"Extra Power Up Time");
	strcpy(Information[4][15][1],"Will make all your time limited");
	strcpy(Information[4][15][2],"powerups last 50% longer");
	strcpy(Information[4][16][0],"Duality");
	strcpy(Information[4][16][1],"Grants you the possibility to buy");
	strcpy(Information[4][16][2],"and simultaneoulsy fire two of");
	strcpy(Information[4][16][3],"all pistols and sub-machine guns");
	strcpy(Information[4][17][0],"Extra Life");
	strcpy(Information[4][17][1],"Speaks for itself!");

	strcpy(Information[5][0][0],"Clone Copy");
	strcpy(Information[5][0][1],"Creates an explosive copy of your");
	strcpy(Information[5][0][2],"of yourself which walks around");
	strcpy(Information[5][0][3],"in the maze until it hits somebody");
	strcpy(Information[5][1][0],"Cover Item");
	strcpy(Information[5][1][1],"Disguises a lethal bomb as a");
	strcpy(Information[5][1][2],"random item. Detonates when");
	strcpy(Information[5][1][3],"somebody tries to collect it!");
	strcpy(Information[5][2][0],"Cluster Bomb");
	strcpy(Information[5][2][1],"A heavy bomb which spreads explosive");
	strcpy(Information[5][2][2],"shrapnel all around you. After");
	strcpy(Information[5][2][3],"you have activated it, don't move!");
	strcpy(Information[5][3][0],"Base Teleporter");	
	strcpy(Information[5][3][1],"Carries you instantly and safely");
	strcpy(Information[5][3][2],"back to your home base.");	
	strcpy(Information[5][4][0],"Kamikaze");
	strcpy(Information[5][4][1],"Turns you into a flying bomb.");	
	strcpy(Information[5][4][2],"Don't plan on surviving!");
	strcpy(Information[5][5][0],"Pain Reflection");
	strcpy(Information[5][5][1],"For ten seconds, all bullets shot");
	strcpy(Information[5][5][2],"by your opponent, which hit");
	strcpy(Information[5][5][2],"you, will damage him instead");
	strcpy(Information[5][6][0],"Base Key");
	strcpy(Information[5][6][1],"Give you access to your");
	strcpy(Information[5][6][2],"opponents base");
	strcpy(Information[5][7][0],"Bullet Reflection");
	strcpy(Information[5][7][1],"All bullets shot by your");
	strcpy(Information[5][7][2],"opponent will bounce off of");
	strcpy(Information[5][7][3],"you for ten seconds");
	strcpy(Information[5][8][0],"Air Strike");
	strcpy(Information[5][8][1],"A mighty wipeout of the entire");
	strcpy(Information[5][8][2],"screen. Once activated, don't move!");
	strcpy(Information[5][9][0],"Ghost");
	strcpy(Information[5][9][1],"Allows you walk to through walls");
	strcpy(Information[5][9][2],"Wears out once you leave them");
	strcpy(Information[5][10][0],"Invisible Bullets");
	strcpy(Information[5][10][1],"All bullets fired within ten");
	strcpy(Information[5][10][2],"seconds will remain invisible.");
	strcpy(Information[5][11][0],"Vulcan G2 Sentry Cannon");
	strcpy(Information[5][11][1],"Deploys a motion detecting Vulcan");
	strcpy(Information[5][11][2],"cannon firing at your opponent");
	strcpy(Information[5][12][0],"Remote Bomb");
	strcpy(Information[5][12][1],"Press once to drop and");
	strcpy(Information[5][12][2],"again to detonate!");
	strcpy(Information[5][13][0],"First Aid Kit");
	strcpy(Information[5][13][1],"Restores your health to maximum");
	strcpy(Information[5][14][0],"Super Bullets");
	strcpy(Information[5][14][1],"Within ten seconds, all bullets you");
	strcpy(Information[5][14][2],"fire will go through the walls");
	strcpy(Information[5][15][0],"Missile Attack");
	strcpy(Information[5][15][1],"Launches a heavy missile. Press the");
	strcpy(Information[5][15][2],"same special key again to detonate");
	strcpy(Information[5][16][0],"Darkness");
	strcpy(Information[5][16][1],"Makes your opponents screen");
	strcpy(Information[5][16][2],"turn totally black for 10 s");
	Beacon(32);

	for(int r=0;r<StoreInfo[3][0];r++){
		sprintf(tstr,"%s %s Class %c",ArmorName[r/3][0],ArmorName[r/3][1],65+r%3);
		strcpy(Information[3][r][0],tstr);
		strcpy(Information[3][r][1],"Besides from giving you extra life (AP)");
		strcpy(Information[3][r][2],"this armor also protects");
		sprintf(tstr,"you against %s",ArmorName[r/3][3]);
		strcpy(Information[3][r][3],tstr);
	}

	// The Prices
	for(int u=0;u<StoreInfo[1][0];u++){
		strcpy(Information[1][u][0],TheGame->Weapons[u]->GetName());
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
	ItemPrice[2][0]=22500;
	ItemPrice[2][1]=45000;
	ItemPrice[2][2]=75000;

	ItemPrice[2][3]=9000;
	ItemPrice[2][4]=19500;
	ItemPrice[2][5]=30000;

	ItemPrice[2][6]=16500;
	ItemPrice[2][7]=37500;
	ItemPrice[2][8]=64500;

	ItemPrice[2][9]=7500;
	ItemPrice[2][10]=18000;
	ItemPrice[2][11]=28500;

	ItemPrice[2][12]=18000;
	ItemPrice[2][13]=31500;
	ItemPrice[2][14]=48000;
	// Misc Section
	ItemPrice[3][0]=1000; // Invis
	ItemPrice[3][1]=1500; // Sp60
	ItemPrice[3][2]=3500; // SpL
	ItemPrice[3][3]=8000; // Sp60
	ItemPrice[3][4]=700;  // Sh60
	ItemPrice[3][5]=2500; // ShL
	ItemPrice[3][6]=16000; // SSB
	ItemPrice[3][7]=5000; // TW
	ItemPrice[3][8]=12000; // MW
	ItemPrice[3][9]=12000; // PR
	ItemPrice[3][10]=25000; // UR
	ItemPrice[3][11]=20000; // BBoost
	ItemPrice[3][12]=4000; // SniperA
	ItemPrice[3][13]=5000; // CUpgr
	ItemPrice[3][14]=11000; // SS Depth
	ItemPrice[3][15]=19500; // ET
	ItemPrice[3][16]=9000; // DU
	ItemPrice[3][17]=6000; // XtraLife

	// Special Section

	ItemPrice[4][0]=1000;
	ItemPrice[4][1]=600; 
	ItemPrice[4][2]=1200;
	ItemPrice[4][3]=500;

	ItemPrice[4][4]=400; 
	ItemPrice[4][5]=1200; 
	ItemPrice[4][6]=500; 
	ItemPrice[4][7]=600;

	ItemPrice[4][8]=1700;
	ItemPrice[4][9]=400;
	ItemPrice[4][10]=400; 
	ItemPrice[4][11]=700; 

	ItemPrice[4][12]=900; 
	ItemPrice[4][13]=1000; 
	ItemPrice[4][14]=1200; 
	ItemPrice[4][15]=2400;
	
	ItemPrice[4][16]=800; 
}