#include "ddex4.h"

#include "NewStore.h"
#include "LookAway.h"

#include "InteractiveHelp.h"
#include "SettingsExplanation.h" 
#include "PlayerSetup.h"
#include "NextRoundInfo.h"
#include "FullVersionInfo.h"
#include "LeagueInfo.h"
#include "CupInfo.h"
#include "LastRoundsInfo.h"
#include "GettingStarted.h"
#include "EnemyEncyclopedia.h"
#include "ItemEncyclopedia.h"
#include "DetailedInstructions.h"
#include "BuyNow.h"
#include "DIK_CODES.h"
#include "GameKeeper.h"
#include "Aviator.h"
#include "Load.h"
#include "Save.h"

#include <fstream>
#include <shellapi.h> // For ShellExecute

using namespace std; // For ofstream, ifstream, endl



Game::Game(){
	SoundStatus=SS_NOSOUND;

	// CRITICAL: Initialize quadrant pointers/sizes to prevent heap corruption
	OldWorldSize = 0;
	WorldSize = 0;

	Initialize("Game");
//	RegisterItem("NumberOfPlayers",&NumberOfPlayers, sizeof(NumberOfPlayers));
//	RegisterItem("TenElemArray",&teArray[0], sizeof(teArray[0])*10);


	// Do not ever change the order of these!!!
	RegisterItem("lastWorld",&lastWorld,sizeof(lastWorld));
	RegisterItem("StartingMoney",&StartingMoney,sizeof(StartingMoney));
	RegisterItem("RoundsCounter",&RoundsCounter,sizeof(RoundsCounter));
	RegisterItem("NumberOfRoundz",&NumberOfRoundz,sizeof(NumberOfRoundz));
	RegisterItem("LASTMM",&LASTMM,sizeof(LASTMM));
	RegisterItem("FIRSTTO",&FIRSTTO,sizeof(FIRSTTO));
	RegisterItem("STARTNEWROUND",&STARTNEWROUND,sizeof(STARTNEWROUND));








}
Game::~Game(){

	// Deletes all Actors

	KillEntireLinkedList(SoundSource, TheSoundEngine->TheSounds, nextSoundSource);

	// Non Actors

	delete TheTazerBeams;
	delete TheBases[0];
	delete TheBases[1];
#ifdef __D2PROTECTED__
	delete[] RegCode;
#endif

	for(int i = 0; i<StoreInfo[1][0];i++)
		delete Weapons[i];

	for(int j=0;j<NumberOfGameStates;j++)
		delete GameStates[j];

	delete TheSoundEngine;

	DeleteTheQuadrants();

	delete mo;


 DeleteObject(g_hFontMedium);DeleteObject( g_hFont);DeleteObject(g_hFontSmall);DeleteObject(g_hFontBig);DeleteObject(g_hFontBigPlus);DeleteObject(g_hFontHuge);DeleteObject(g_hStatsSub);DeleteObject(g_hStatsLine);DeleteObject(g_hFontAngled[2]);DeleteObject(thePen);DeleteObject(aPen);DeleteObject(bPen);DeleteObject(cPen);DeleteObject( lifePen);DeleteObject( greenPen);DeleteObject( redPen);DeleteObject( bluePen);DeleteObject(theBrush);DeleteObject(aBrush);DeleteObject( lifeBrush);

}
void Game::CreateObjects(){

#ifdef DEBUGGING
	dw=new DebugWindow();
#endif


	TheSpawnPit = new Spawner();
	TheSoundEngine = new SoundEngine();
	TheInputEngine = new InputEngine();
	TheTimeKeeper = new TimeKeeper();
	TheGameKeeper = new GameKeeper();

	TheSaveableClasses=this;
	nextSaveableClass=TheGameKeeper;

	//                                   Weapon Characteristics

	//		        	 --------  Primary Attack  ---------	-------------  Weapon Info  --------------		   ----  Secondary Attack  ----

	//     Name				BTp	   Dmg Spd Acc Clip FR  RDlay   Price   Type    Lengths	  Name				Game		BTp		Dmg		Spd Acc FR
	Glock = new Weapon(   B_9,		20,	8 ,	18,	10,	1.5, 750,	8000,   wGun,	11,	 7,	"Glock 17"				);
	UZI = new Weapon(     B_9,		12,	7,	3,	45,	6,	1500,	12000,	wSMG,	13,	8,	"UZI",						B_9,	16,		7,	12,	1.5	);
	HKMP5 = new Weapon(   B_9,		14,	7,	6,	36,	5,	1000,	17000,	wARf,	13,	7,	"HKMP5",					B_9,	20,		7,	20,	1.5	);
	Colt = new Weapon(    B_9,		22,	10,	10,	25,	5,	1250,	26000,	wARf,	18,	12,	"Colt 6450",				B_9,	40,		10,	25,	1.5	);

	Aksu = new Weapon(    B_556,	30,	10,	4,	30,	6,	1750,	38500,	wSMG,	15,	10,	"AKSU 74U",					B_556,	40,		10,	9,	1.5	);
	M16 = new Weapon(     B_556,	36,	11,	9,	30,	5,	1000,	45000,	wARf,	20,	12,	"M16",						B_556,	50,		10,	22,	1.5	);
	M16203 = new Weapon(  B_556,	36,	11,	9,	30,	5,	1000,	60000,	wARf,	20,	12,	"M16 with M203",			B_SHELL,500,	6,	15,	0.75);
	Enfield = new Weapon( B_556,	50,	13,	14,	30,	4,	2000,	55000,	wARf,	19,	12,	"Enfield",					B_556,	42,		10,	30,	1.5	);
	Sporter = new Weapon( B_556,	80,	15,	40,	25,	1.5,1000,	31000,	wRRf,	22,	14,	"FNFAL Sporter",			B_556,	40,		12,	4,	3.5	);
	Minimi = new Weapon(  B_556,	50,	10,	4,	200,10,	3000,	95000,	wMGn,	23,	15,	"FN Minimi",				B_556,	60,		15,	12,	4	);

	RKM60 = new Weapon(   B_762,	50,	15,	3,	36,	6,	2000,	64000,	wSMG,	18,	11,	"RKM-60",					B_762,	60,		18,	8,	2	);
	AK47 = new Weapon(    B_762,	70,	18,	10,	30,	5,	1500,	75000,	wARf,	22,	14,	"AK-47",					B_762,	60,		12,	24,	1.5	);
	M14 = new Weapon(     B_762,	120,30,	50,	10,	1.5,750,	54000,	wRRf,	22,	14,	"M14",						B_762,	54,		16,	14,	3	);
	M60 = new Weapon(     B_762,	70,	16, 5,	250,12,	4000,  170000,	wMGn,	26,	16,	"M60",						B_762,	90,		20,	15,	3	);

	BarretGun = new Weapon(B_12,	90,	16,	10,	10,	1.5,600,	34000,	wGun,	14,	10,	"Barret M81"			);
	Barret = new Weapon(  B_12,		480,45,	90,	10,	1,	1000,	110000,	wSRf,	27,	19,	"Barret Light 50"		);
	Browning = new Weapon(B_12,		100,20,	4,	500,12,	3500,	240000,	wMGn,	27,	18,	"Browning .50",				B_12,  120,		17,	14,	4 	);

	BenelliM3 = new Weapon(B_SH,	10,	9,	7,	8,	1,	3500,	18000,	wRSg,	18,	12,	"Benelli M3T",				B_SH,	20,		7,	20,	1.5	);
	BenelliM1014 = new Weapon(B_SH,	8,	7,	3,	8,	2,	3500,	26000,	wRSg,	18,	11,	"M1014 Semi Auto",			B_SH,	20,		7,	20,	1.5	);
	KS23K = new Weapon(   B_SH,		8,	6,	2,	12,	3,	2000,	34000,	wRSg,	17,	11,	"KS23K Combat",				B_SH,	20,		7,	20,	1.5	);
	Saiga = new Weapon(   B_SH,		8,	8,	4,	20,	5,	1200,	70000,	wASg,	23,	15,	"Saiga 12K",				B_SH,	20,		7,	20,	1.5	);

	LaserRifle = new Weapon(  B_NRJ,360,C_RED,45,100,1.2,1000,	98000,	wSRf,	23,	13,	"GZ-12R Laser",				B_NRJ,	10,	 C_RED,	20,	1.5	);
	AssaultLaser = new Weapon(B_NRJ,150,C_BLUE,7,100,10,1000,  270000,	wARf,	25, 15,	"AZ-54B Laser",			B_NRJ,	10,	 C_BLUE,20,	2	);

	Tazer = new Weapon(       B_NRJ,20,C_BLUE,-1,100,500/TAZERTIME,
														 500,   20000,	wTzr,	 9,	 6,	"Tazer"				);

	Mortar = new Weapon(  B_SHELL,	400, 8,	25,	1,	0.5,  0,	54000,	wMor,	13,	 9,	"M79 Dooper",				B_HESHELL,900,	8,	25, 0.25);
	ATShot = new Weapon(  B_ATSHOT,	1000,6,	40,	1,	0.5,  0,	7000,	wShd,	25,	19,	"AT Shot"				);
	Bazooka = new Weapon( B_APRKT,	800,7,30,	1,  0.25, 0,	42000,	wShd,	28,	21,	"Bazooka",					B_ATRKT,  1500, 7,	40,	0.25);

	Weapons[0]=Glock;
	Weapons[1]=UZI;
	Weapons[2]=HKMP5;
	Weapons[3]=Colt;
	Weapons[4]=Aksu;
	Weapons[5]=M16;
	Weapons[6]=M16203;
	Weapons[7]=Enfield;
	Weapons[8]=Sporter;
	Weapons[9]=Minimi;
	Weapons[10]=RKM60;
	Weapons[11]=AK47;
	Weapons[12]=M14;
	Weapons[13]=M60;
	Weapons[14]=BarretGun;
	Weapons[15]=Barret;
	Weapons[16]=Browning;
	Weapons[17]=BenelliM3;
	Weapons[18]=BenelliM1014;
	Weapons[19]=KS23K;
	Weapons[20]=Saiga;
	Weapons[21]=LaserRifle;
	Weapons[22]=AssaultLaser;
	Weapons[23]=Tazer;
	Weapons[24]=Mortar;
	Weapons[25]=ATShot; // resets in another location, must be 25
	Weapons[26]=Bazooka;
	//Weapons=[Glock,Dual,UZI,HKMP5,Colt,Aksu,M16,M16203,Enfield,Sporter,Minimi,RKM60,AK47,M14,M60,BarretGun,Barret,Browning,BenelliM3,BenelliM1014,KS23K,Saiga,LaserRifle,AssaultLaser,Tazer,Mortar,ATShot, Bazooka];

	// Players m�ste skapas f�rst, allt som skapas d�refter kommer att d� ut n�r Cloner och annat deletas
	
	TheClones=NULL;
	TheMortals=NULL;
	TheDeadMortals=NULL;
	TheShooters=NULL;
	TheShots=NULL;
	TheAviators=NULL;
	TheGrenades=NULL;
	TheLaserBeams=NULL;
	TheTazerBeams=NULL;

	TheBases[0]= new Base(0);
	TheBases[1]= new Base(1);


	TheStats = new Stats();
	ThePlayerSetup = new PlayerSetup();
	Beacon(6231);
	TheSettings = new Settings(); // Requires PlayerSetup
//	TheGameSetup = new GameSetup();
		Beacon(6232);
	TheIntro = new Intro();
//	TheStores = new Stores();
	TheNewStore = new NewStore();
		Beacon(6233);
	TheControls = new Controls();
	Beacon(6234);
	ThePlay = new Play();
		Beacon(6235);
	TheStart = new Start();
	ThePlayerSelecter = new PlayerSelecter();
	TheInteractiveHelp = new InteractiveHelp();
	TheSettingsExplanation = new SettingsExplanation();
	TheNextRoundInfo = new NextRoundInfo();
	TheFullVersionInfo = new FullVersionInfo();
	TheLeagueInfo = new LeagueInfo();
	TheCupInfo = new CupInfo();
	TheLastRounds = new LastRoundsInfo();
	TheGettingStarted = new GettingStarted();
	TheEnemyEncyclopedia = new EnemyEncyclopedia();
	TheItemEncyclopedia = new ItemEncyclopedia();
	TheDetailedInstructions = new DetailedInstructions();
	TheBuyNow = new BuyNow();


	GameStates[ PLAY ] = ThePlay;
//	GameStates[ STORES ] = TheStores;
	GameStates[ NEWSTORE ] = TheNewStore;
	GameStates[ STATISTICS ] = TheStats;				
//	GameStates[ GAMESETUP ] = TheGameSetup;			
	GameStates[ START ] = TheStart;				
	GameStates[ ABOUT ] = new About();				
	GameStates[ SETTINGS ] = TheSettings;			
	GameStates[ LOAD ] = new Load();		
	GameStates[ SAVE ] = new Save();	
	GameStates[ INTRO ] = TheIntro;			
	GameStates[ GETREADY ] = new GetReady();					
	GameStates[ CONTROLS ] = TheControls;
	GameStates[ LOOKAWAY ] = new LookAway();
	GameStates[ PLAYERSELECTER ] = ThePlayerSelecter;

	GameStates[ HELP ] = TheInteractiveHelp;	
	GameStates[ SETTINGSEXPLANATION ] = TheSettingsExplanation;
	GameStates[ PLAYERSETUP	 ] = ThePlayerSetup;
	GameStates[ NEXTROUND ] = TheNextRoundInfo;	
	GameStates[ FULLVERSION	 ] = TheFullVersionInfo;
	GameStates[ LEAGUE ] = TheLeagueInfo;	
	GameStates[ LASTROUNDS ] = TheLastRounds;	
	GameStates[ GETTINGSTARTED ] = TheGettingStarted;	
	GameStates[ ENEMYENCYCLOPEDIA ] = TheEnemyEncyclopedia;	
	GameStates[ ITEMENCYCLOPEDIA ] = TheItemEncyclopedia;	
	GameStates[ DETAILEDINSTRUCTIONS ] = TheDetailedInstructions;	
	GameStates[ CUP ] = TheCupInfo;
	GameStates[ BUYNOW ] = TheBuyNow;

	mo = new MovementObject();


	Beacon(1246);

}

void Game::Reset(){

	UZ2();
	GAMEACTIVE=FALSE;
	SETWORLD=FALSE;
	OldWorldSize=0;
	WorldSize=0;

	ThePlayers[0]=ThePlayers[1]=NULL;

	// Reset Explosion, BloodStain, lbarz, Barz, UPDATETEXT, BLITBAR, ExplosionStart
	// Strs = "a"
	// MessageX
	// RegisterCode[60];
	// PlayerBullets[40];
	// PlayerSecondary[40];
	// PlayerCharges[35];
	// PlayerGrenades[35];
	// PlayerLives[35];
	// UserName[50],UserCode[50];
	// ErrorMessage[50];
	// FileNames[5][41];

	//-----------------------------------------------------------------------------
	// Private Variables
	//-----------------------------------------------------------------------------


	StartingMoney=0;
	Null[0]=0,Null[1]=0;
	CPlayer=0;
	WorldType=1;
	yup=0, Satan=2;
	lastWorld=0;
	SpecialEvents=0;
	Choice=0;
	NumberOfRoundz=0;	
	ActivePlayer=0;
	BlitLife[0]=BlitLife[1]=0;
	int y=0;
	CheatKeys[cPacman][y++]=DIK_P;
	CheatKeys[cPacman][y++]=DIK_A;
	CheatKeys[cPacman][y++]=DIK_C;
	CheatKeys[cPacman][y++]=DIK_M;
	CheatKeys[cPacman][y++]=DIK_A;
	CheatKeys[cPacman][y++]=DIK_N;
	CheatKeys[cPacman][y++]=DIK_W;
	CheatKeys[cPacman][y++]=DIK_A;
	CheatKeys[cPacman][y++]=DIK_N;
	CheatKeys[cPacman][y++]=DIK_T;
	CheatKeys[cPacman][y++]=DIK_S;
	CheatKeys[cPacman][y++]=DIK_M;
	CheatKeys[cPacman][y++]=DIK_E;
	CheatKeys[cPacman][y++]=-1;
	y=0;
	CheatKeys[cPrey][y++]=DIK_M;
	CheatKeys[cPrey][y++]=DIK_O;
	CheatKeys[cPrey][y++]=DIK_R;
	CheatKeys[cPrey][y++]=DIK_E;
	CheatKeys[cPrey][y++]=DIK_P;
	CheatKeys[cPrey][y++]=DIK_R;
	CheatKeys[cPrey][y++]=DIK_E;
	CheatKeys[cPrey][y++]=DIK_Y;
	CheatKeys[cPrey][y++]=-1;
	y=0;
	CheatKeys[cRocket][y++]=DIK_L;
	CheatKeys[cRocket][y++]=DIK_O;
	CheatKeys[cRocket][y++]=DIK_V;
	CheatKeys[cRocket][y++]=DIK_E;
	CheatKeys[cRocket][y++]=DIK_A;
	CheatKeys[cRocket][y++]=DIK_N;
	CheatKeys[cRocket][y++]=DIK_D;
	CheatKeys[cRocket][y++]=DIK_R;
	CheatKeys[cRocket][y++]=DIK_O;
	CheatKeys[cRocket][y++]=DIK_C;
	CheatKeys[cRocket][y++]=DIK_K;
	CheatKeys[cRocket][y++]=DIK_E;
	CheatKeys[cRocket][y++]=DIK_T;
	CheatKeys[cRocket][y++]=-1;
	y=0;
	CheatKeys[cBoom][y++]=DIK_I;
	CheatKeys[cBoom][y++]=DIK_A;
	CheatKeys[cBoom][y++]=DIK_L;
	CheatKeys[cBoom][y++]=DIK_W;
	CheatKeys[cBoom][y++]=DIK_A;
	CheatKeys[cBoom][y++]=DIK_Y;
	CheatKeys[cBoom][y++]=DIK_S;
	CheatKeys[cBoom][y++]=DIK_H;
	CheatKeys[cBoom][y++]=DIK_A;
	CheatKeys[cBoom][y++]=DIK_T;
	CheatKeys[cBoom][y++]=DIK_E;
	CheatKeys[cBoom][y++]=DIK_D;
	CheatKeys[cBoom][y++]=DIK_T;
	CheatKeys[cBoom][y++]=DIK_H;
	CheatKeys[cBoom][y++]=DIK_E;
	CheatKeys[cBoom][y++]=DIK_W;
	CheatKeys[cBoom][y++]=DIK_O;
	CheatKeys[cBoom][y++]=DIK_R;
	CheatKeys[cBoom][y++]=DIK_L;
	CheatKeys[cBoom][y++]=DIK_D;
	CheatKeys[cBoom][y++]=DIK_A;
	CheatKeys[cBoom][y++]=DIK_N;
	CheatKeys[cBoom][y++]=DIK_Y;
	CheatKeys[cBoom][y++]=DIK_W;
	CheatKeys[cBoom][y++]=DIK_A;
	CheatKeys[cBoom][y++]=DIK_Y;
	CheatKeys[cBoom][y++]=DIK_S;
	CheatKeys[cBoom][y++]=-1;
	y=0;
	CheatKeys[cSpeed][y++]=DIK_S;
	CheatKeys[cSpeed][y++]=DIK_P;
	CheatKeys[cSpeed][y++]=DIK_E;
	CheatKeys[cSpeed][y++]=DIK_E;
	CheatKeys[cSpeed][y++]=DIK_E;
	CheatKeys[cSpeed][y++]=DIK_D;
	CheatKeys[cSpeed][y++]=-1;
	y=0;
	CheatKeys[cMight][y++]=DIK_M;
	CheatKeys[cMight][y++]=DIK_I;
	CheatKeys[cMight][y++]=DIK_G;
	CheatKeys[cMight][y++]=DIK_H;
	CheatKeys[cMight][y++]=DIK_T;
	CheatKeys[cMight][y++]=DIK_A;
	CheatKeys[cMight][y++]=DIK_N;
	CheatKeys[cMight][y++]=DIK_D;
	CheatKeys[cMight][y++]=DIK_M;
	CheatKeys[cMight][y++]=DIK_A;
	CheatKeys[cMight][y++]=DIK_G;
	CheatKeys[cMight][y++]=DIK_I;
	CheatKeys[cMight][y++]=DIK_C;
	CheatKeys[cMight][y++]=-1;

	CheatFunctions[cPacman]=&Game::ActivatePacman; // C++20: explicit member pointer syntax
	CheatFunctions[cPrey]=&Game::ActivatePrey;
	CheatFunctions[cBoom]=&Game::ActivateBoom;
	CheatFunctions[cRocket]=&Game::ActivateRocket;
	CheatFunctions[cSpeed]=&Game::ActivateSpeed;
	CheatFunctions[cMight]=&Game::ActivateMight;

	Zero(CheatLevel,NoOfCheats);
	CheatActive=0;
	ExpSlow=40;
	HPFrame[0]=0,HPFrame[0]=0,HPFrame[0]=0;
	MS=0;

	ColorKey=DDBLTFAST_SRCCOLORKEY;

	BPoint=0;	
	Pointer=0;

	sflop[0]=0, sflop[1]=0;
	RoundsCounter=0;
	Switch=1;
	OtherHealth[0]=0,OtherHealth[1]=0;

	TrCnt=0;




	Timer[0]=0,Timer[1]=1; // Total, Play
	TheFrame=0;
	
	SETVOLUME=FALSE;
	TIMEOUT=FALSE;

	FIRSTTO=FALSE;

	COORDS=FALSE;
	BETWEEN=FALSE;
	GAMEDONE=FALSE,READY[0]=FALSE,READY[1]=FALSE;
	COOL=FALSE, PAUSE=FALSE; SUDDENDEATH=FALSE;
	KILL=FALSE,SETGAMMA=FALSE;
	MONEYMAZE=FALSE;lTimeLeft=-1;
	ESCSUPRESSED=FALSE;
	LASTMM=FALSE,PAINTED=FALSE,MAGICDONE=FALSE;
	WASFROZEN[0]=FALSE,WASFROZEN[1]=FALSE;//,WASPAUSED=FALSE;
	COPYBACK=FALSE;

	FrameTickCount=thisTickCount;
	StartTime=thisTickCount;
	CantChangeStart=0;
	FrameCount=0,ufoTick=0,IncrStart=0,DecrStart=0;
	TankKillStart[0]=0,	TankKillStart[1]=0;

	Times[0]=Times[1]=0;


	Zero2d(BLITPUPS,2,5);
	Zero2d(UPDATETEXT,2,6);
	Zero2d(BLITBAR,2,6);
	UPDATETIMELIMIT=0;
	UPDATESCREEN=3;


}

void Game::ActivateCheat(){
	DP("Cheat perhaps run");
	for(int u=0;u<NoOfCheats;u++){
		CheatLevel[u]=0;
	}
	if(CheatActive){
		DP("Cheatrun");
		(this->*(CheatFunctions[CheatActive-1]))();
		CheatActive=0;
	}
}


void Game::ActivatePacman(){
	while(TRUE){

		int px=32*(5+((2*(rand()%(TheGame->WorldSize*11-10)))/2));
		int py=32*(5+((2*(rand()%(TheGame->WorldSize*11-10)))/2));
		if(TheSpawnPit->CreateMortalOK(T_PACMAN,px,py)){
			DP("Pacman created");
			TheSpawnPit->CreateMortal(px, py, T_PACMAN, 2);
			AddFX();
			break;
		}
	}
}
void Game::ActivateRocket(){
	DP("Gave ATshot");
	ThePlayers[0]->GiveATShot();
	ThePlayers[1]->GiveATShot();
}
void Game::ActivatePrey(){
		DP("Prey!");
	TheSpawnPit->SpawnRandomEnemy();
}
void Game::ActivateBoom(){
		DP("Bigbadaboom!");
	TheGame->RegExp(0,0,100,RET_ENTIREWORLD,DT_SPECIALUNIT,NULL);
}
void Game::ActivateSpeed(){
	DP("Speed!");
	ThePlayers[0]->GoSuperFast();
	ThePlayers[1]->GoSuperFast();
}
void Game::ActivateMight(){
	DP("Might!");
	ThePlayers[0]->MightAndMagic();
	ThePlayers[1]->MightAndMagic();
}
void Game::AddShot(int LeftRightShot, double SpeedOrColor, double AccOrAngle, BulletType bType, int Damage, int Alt, Shooter * Firerer){

	if(bType==B_NRJ&&AccOrAngle==-1){
		TazerBeam * newTazerBeam = new TazerBeam(Firerer, Damage, C_BLUE, TAZER_DISTANCE);
		AddToLinkedList(TheTazerBeams, nextTazerBeam, newTazerBeam);
		return;		
	}
	int StartX=Firerer->GetActualXPos();
	int StartY=Firerer->GetActualYPos();
	
	int Direction = Firerer->Get8Dir();
	
	LengthAdjust(&StartX,&StartY,Direction,Firerer->GetPipeLength(LeftRightShot));
	SideAdjust(&StartX,&StartY,Direction,LeftRightShot*Firerer->GetPipeDistance(Firerer->Get8Dir()%2)+Firerer->GetPipeSideAdjust());

	Sound snd=Ammo2Sound[bType];
	DistanceType dType=dNormal;

	if(bType==GR_SHELL||bType==GR_ROCKET||bType==GR_GRENADE){

		if(Firerer->GetMType()==M_PL) 
			SpeedOrColor+=((Player *)Firerer)->GetExtraThrowingSpeed();

		Grenade * newGrenade;
		newGrenade = new Grenade( StartX, StartY, (int)SpeedOrColor, AccOrAngle,  Damage, bType, Firerer);
		newGrenade->nextGrenade=TheGrenades;
		TheGrenades=newGrenade;

	}else if(bType==B_NRJ&&AccOrAngle!=-1){

		Beacon(243255);
		LaserBeam * newLaserBeam = new LaserBeam(StartX, StartY, (int)(1000.0*NewAsk(AccOrAngle)), Damage, Firerer, (int)SpeedOrColor);
		AddToLinkedList(TheLaserBeams, nextLaserBeam, newLaserBeam);
		Beacon(243257);
		
	}else if(bType==B_SH){

		int X,Y;
		for(int i = 0;i<10;i++){
			X=StartX+8;Y=StartY+8;
			SideAdjust(&X,&Y,Direction,rand()%7-3);
			LengthAdjust(&X,&Y,Direction,rand()%7);
			shot * newshot = new shot(X, Y, SpeedOrColor-1.0+((double)(rand()%2000))/1000.0, NewAsk(AccOrAngle), bType, Damage, Firerer,  RET_NONE);
			AddToLinkedList(TheShots,nextshot,newshot);

		}
	}else{
		RegExpType rType=RET_NONE;

		if(bType==B_HESHELL){
			rType=RET_SQUAD;
			dType=dFar;
		}else if(bType==B_SHELL)
			rType=RET_SWEAK;	
		else if(bType==B_APRKT||bType==B_ATRKT){
			rType=RET_SWEAK;
			if(Firerer->GetMType()==M_PL&&((Player *)Firerer)->BBoost()){
				if(bType==B_APRKT)
					if(Direction%2)
						rType=RET_MX;
					else
						rType=RET_MPLUS;
				else
					rType=RET_MQUAD;
			}else
				rType=RET_MWEAK;
		}else if(bType==B_ATSHOT){

			bType=B_ATRKT;	
			rType=RET_MQUAD;

		}
		if(bType==B_9)
			dType=dClose;

		shot * newshot = new shot(StartX, StartY, SpeedOrColor, NewAsk(AccOrAngle), bType,Damage, Firerer,  rType);
		AddToLinkedList(TheShots,nextshot,newshot);

	}

	Beacon(3534543);
	FireFlame * newFlame = new FireFlame(Firerer, LeftRightShot, bType, (int)SpeedOrColor);


	Beacon(2472932);

	if(snd!=sVoid)
		TheSoundEngine->PlayLocatedSound(snd, StartX, StartY, dType, spLowest);
	

}


double Game::NewAsk(double Acc){
	int randomized=rand();

	long randed=randomized*randomized;
	long Max=RAND_MAX;
	Max*=Max;

	double ask = randed;
	ask /=(double)Max;

	// ask is now a number between 0 and 1
	ask /=Acc;

	if(randomized%2)
		return -ask;

	return ask;
}

void Game::KillShots(){
	Beacon(353587);
	KillLinkedList(shot, TheShots, KILLME, nextshot); 
	KillLinkedList(Grenade, TheGrenades, KILLME, nextGrenade); 
	KillLinkedList(LaserBeam, TheLaserBeams, KILLME, nextLaserBeam); 
	KillLinkedList(TazerBeam, TheTazerBeams, KILLME, nextTazerBeam); 
	KillLinkedList(FireFlame, TheFireFlames, KILLME, nextFireFlame); 
	KillLinkedList(Aviator, TheAviators, KILLME, nextAviator); 
	Beacon(353588);
}



void Game::SetVariables(){

	GetCurrentDirectory(256,&WorkingDir[0]);
//	GetModuleFileName(NULL, &WorkingDir[0],256);
//	SetCurrentDirectory(&WorkingDir[0] );
	CreateDirectory("Data",0);


	Beacon(30);
	UnSelectedColor = RGB(0,0,200);
	SelectedColor = RGB(50,0,255);
	SelectedBGColor = RGB(50,0,100);

	// Unit Thickness

	ThickArray[T_PLAYER]=PLAYERTHICKNESS;
	ThickArray[T_JEEP]=10;
	ThickArray[T_HUMVEE]=10;
	ThickArray[T_STEAM]=10;
	ThickArray[T_WHEEL]=12;
	ThickArray[T_INFANTRYDROID]=7;
	ThickArray[T_BATTLEDROID]=10;
	ThickArray[T_WALKER]=12;
	ThickArray[T_MINITANK]=11;
	ThickArray[T_SILVERTANK]=12;
	ThickArray[T_DOUBLEBARRELEDTANK]=13;
	ThickArray[T_BIGTANK]=15;
	ThickArray[T_ROCKETTANK]=11;
	ThickArray[T_PACMAN]=11;
	ThickArray[T_SENTRYGUN]=11;



	// Item Amount

	Probability[0]=100;
	Probability[1]=70;
	Probability[2]=55;
	Probability[3]=15;
	Probability[4]=45;
	Probability[5]=15;
	Probability[6]=35;
	Probability[7]=10;

	Probability[8]=40;
	Probability[9]=60;
	Probability[10]=30;
	Probability[11]=75;
	Probability[12]=45;
	Probability[13]=15;
	Probability[14]=1;
	Probability[15]=2;

	Probability[16]=40;
	Probability[17]=50;
	Probability[18]=25;
	Probability[19]=3;
	Probability[20]=30;
	Probability[21]=55;
	Probability[22]=25;
	Probability[23]=4;

	Probability[24]=7;
	Probability[25]=5;
	Probability[26]=10;
	Probability[27]=2;
	Probability[28]=2;
	Probability[29]=120;
	//Probability[30]=0;
	//Probability[31]=0;

	Beacon(31);
	// Players (doesn't matter)

	for(int i=0;i<NumberOfMortals;i++)
		for(int j=0;j<NumberOfMortals;j++)
			MortalMatrix[i][j]=FALSE;


	// Tanks
	MortalMatrix[M_TK][M_PL] = TRUE;
	MortalMatrix[M_TK][M_CL] = TRUE;
	MortalMatrix[M_TK][M_DR] = TRUE;
	MortalMatrix[M_TK][M_CA] = TRUE;

	// Clones
	MortalMatrix[M_CL][M_PL] = TRUE;

	// Droids
	MortalMatrix[M_DR][M_PL] = TRUE;
	MortalMatrix[M_DR][M_CL] = TRUE;
	MortalMatrix[M_DR][M_DR] = TRUE;

	// Machines
	MortalMatrix[M_MA][M_PL] = TRUE;
	MortalMatrix[M_MA][M_CL] = TRUE;
	MortalMatrix[M_MA][M_DR] = TRUE;

	// Cars
	MortalMatrix[M_CA][M_PL] = TRUE;
	MortalMatrix[M_CA][M_DR] = TRUE;

	// Sentry Guns
	MortalMatrix[M_SG][M_PL] = TRUE;
	MortalMatrix[M_SG][M_CL] = TRUE;

	// Pacmans
	MortalMatrix[M_PM][M_PL] = TRUE;

	RadarColors[M_PL][0] = 96;		// Green
	RadarColors[M_PL][1] = 96+11;
	RadarColors[M_TK][0] = 32;		// Red
	RadarColors[M_TK][1] = 32+9;
	RadarColors[M_DR][0] = 168;		// Purple
	RadarColors[M_DR][1] = 168+5;
	RadarColors[M_MA][0] = 72;		// Yellow
	RadarColors[M_MA][1] = 72+6;
	RadarColors[M_CA][0] = 144;		// Blue
	RadarColors[M_CA][1] = 144+7;	
	RadarColors[M_SG][0] = 20;		// Grey
	RadarColors[M_SG][1] = 20+6;
	RadarColors[M_PM][0] = 56;		// Orange
	RadarColors[M_PM][1] = 56+7;

	RadarColors[M_CL][0] = RadarColors[M_PL][0];
	RadarColors[M_CL][1] = RadarColors[M_PL][1];


	// Weapon Blit Adjustment

	WeaponAdjust[0][0]=0;
	WeaponAdjust[0][1]=-14;
	WeaponAdjust[1][0]=16;
	WeaponAdjust[1][1]=-15;
	WeaponAdjust[2][0]=14;
	WeaponAdjust[2][1]=0;
	WeaponAdjust[3][0]=15;
	WeaponAdjust[3][1]=16;

	for(int k=4;k<8;k++){
		WeaponAdjust[k][0]=-WeaponAdjust[k-4][0];
		WeaponAdjust[k][1]=-WeaponAdjust[k-4][1];
	}

	lSoundStatus=SS_NOSOUND;

	Beacon(34);

	SetDikNames();

	Beacon(34);

	Beacon(300);

	TheTimeKeeper->SetLastCallToNow();
	UpdateTime();

	Beacon(301);

	srand((int)TheTimeKeeper->GetRealTime());

	// Variable Initialation
	
	FOCUSLOST=FALSE;
}

void Game::UserMessage(const char * title, const char * msg, int mboxandquit){
	Beacon(666);
	ofstream File("DebugOutput.txt");
	File << title << endl << msg << endl ;
	File.close();
	if(mboxandquit){
		DP("MessageBoxQuit");
		MessageBox(hWnd,msg,title,MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
		Quit();
	}
}

void Game::BeginNewRound(){

	TheSettings->SettingsMenu->ValidateData(LOCK_TAKEEFFECTNEXTROUND);
	TheSettings->EnemySetupMenu->ValidateData(LOCK_TAKEEFFECTNEXTROUND);

	Beacon(41);
#ifdef __D2PROTECTED__
	if(!ISREGISTERED&&NumberOfRoundz!=FREENUMBEROFROUNDS){
		DP("!Registered error Quit");
		Quit();
	}
#endif
	Beacon(42);

	MAGICDONE=FALSE;
	SUDDENDEATH=FALSE;
	

	Beacon(43);	
	
	
	if(TheGameKeeper->GetGameType()==gtOneOnOne&&
	   (ThePlayers[0]->GetStat(ST_EARNED)*2<ThePlayers[1]->GetStat(ST_EARNED)||ThePlayers[1]->GetStat(ST_EARNED)*2<ThePlayers[0]->GetStat(ST_EARNED))){
		if(ThePlayers[0]->GetStat(ST_EARNED)+ThePlayers[1]->GetStat(ST_EARNED)>25000){
			if(!(rand()%5)){
				MONEYMAZE=TRUE;
				
				AddFX();
			}
		}
	}
	
	
	int a,b;
	a=AL_WT;
	if(a==WT_RANDOM)
		if(FIRSTTIMESTARTUP&&!SETWORLD){
			a=0;
			SETWORLD=TRUE;
		}
		else a=rand()%WT_NUMBEROFWORLDS;
	else if(a==WT_SHIFTING){
		a=lastWorld+1;
		if(a==WT_NUMBEROFWORLDS)
			a=0;
		lastWorld=a;
	}else
		a-=WT_NUMBER1;
	
	TheWorldSize=AL_WS;
	b = TheWorldSize*2+3;
	
	if(TheWorldSize==WS_EXAGGERATED)
		b+=4;
	
	
	
	Beacon(44);
	GenerateWorld(a,b);	
	Beacon(45);
	Beacon(800000000+ThePlayers[0]->GetTeam());
	ThePlayers[0]->BeginRound();
	ThePlayers[1]->BeginRound();
	Beacon(800000000+ThePlayers[0]->GetTeam());
	Beacon(46);
	
	// Generate Mortals. This must be done AFTER resetting ThePlayers due to TheQuadrants
	
	TheSpawnPit->SpawnEnemiesForNewRound();

	
	int TimeOut=AL_TO;
	
	// Rules for time-out
	if(TimeOut==TO_VQUICK){
		Timer[0]=2 + (b-3)/2;
		Timer[1]=2;
	}
	else if(TimeOut==TO_QUICK){
		Timer[0]=5 + (b-3);
		Timer[1]=3;
	}
	else if(TimeOut==TO_NORMAL){
		Timer[0]=10 + (int)(((double)b-3.0)*1.25);
		Timer[1]=5;
	}
	else if(TimeOut==TO_SLOW){
		Timer[0]=20 + (int)(((double)b-3.0)*2.5);
		Timer[1]=8;
	}else if(TimeOut==TO_VSLOW){
		Timer[0]=30 + (b-3)*5;
		Timer[1]=10;
	}

	// Reload all weapons
	for(int pl=0;pl<2;pl++)
		for(int alt=0;alt<2;alt++)
			for(int i = 0; i<StoreInfo[1][0];i++)
				Weapons[i]->Reload(pl, alt,ThePlayers[pl]->GetOwnedWeapons(i)==2);
	
	
	Timer[0]*=60000;
	Timer[1]*=60000;
	
	TheSoundEngine->PlaySound(sStart,spNormal);
	UpdateTime();
	StartTime=thisTickCount;	

	if(MONEYMAZE){
		DP("StopTime set to 1 minute");
		StopTime=StartTime+1*60000;		
	}else{
		if(AL_TL){
			DP("uhl");
			StopTime=StartTime+AL_TL*60000;		
		}else{
			DP("StopTimeSetToZero");
			StopTime=0;
		}
	}



}

int Game::IsPaused(){return PAUSE;}
int Game::IsSuddenDeath(){return SUDDENDEATH;}
HINSTANCE Game::GoToURL(const char * URL, int QUITGAME){

	ClearScreen();

	if(QUITGAME){
		DP("Quitting through GoToURL()");
		PostMessage(TheGame->hWnd,WM_DESTROY,0,0);
	}else{
		PostMessage(TheGame->hWnd,WM_KILLFOCUS,0,0);
	}

	HINSTANCE hi = ShellExecute( NULL, "open", URL, NULL, NULL,SW_SHOWNORMAL);

	if((int)hi<=32){
		UserMessage("Error Opening File",URL,FALSE);
	}
	else{
		DP("Opened");
		DP(URL);
	}

	return hi;
}

