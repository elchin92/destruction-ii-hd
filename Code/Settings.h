#ifndef __Settings__ 
#define __Settings__

#include "definitions.h"

#define FREENUMBEROFROUNDS 5
#define FREESTARTINGMONEY  10000

enum Setting{WS,WT,SM,IT,VC,RS,DS,TO,TT,ST,GS,SE,X1,SS,VL,X2,GA,GT,TL,RM,PB,IM,NumberOfSettings};

// ALIASES

#define AL_WS TheSettings->GetValue(WS)
#define AL_WT TheSettings->GetValue(WT)
#define AL_SM TheSettings->GetValue(SM)
#define AL_IT TheSettings->GetValue(IT)
#define AL_DS TheSettings->GetValue(DS)
#define AL_TO TheSettings->GetValue(TO)
#define AL_TT TheSettings->GetValue(TT)
#define AL_ST TheSettings->GetValue(ST)
#define AL_GS TheSettings->GetValue(GS)
#define AL_SE TheSettings->GetValue(SE)
#define AL_SS TheSettings->GetValue(SS)
#define AL_VL TheSettings->GetValue(VL)
#define AL_XX TheSettings->GetValue(XX)
#define AL_GA TheSettings->GetValue(GA)

#define AL_GT TheSettings->GetValue(GT)
#define AL_TL TheSettings->GetValue(TL)

#define AL_RM TheSettings->GetValue(RM)
#define AL_RS TheSettings->GetValue(RS)

#define AL_VC TheSettings->GetValue(VC)
#define AL_PB TheSettings->GetValue(PB)
#define AL_IM TheSettings->GetValue(IM)

// Must be same order as AddItem() order in constructor
#define SS_STEREO	0
#define SS_NOSOUND	1
#define SS_MONO		2
#define SS_RSTEREO	3

#define ITEM_FEW			0
#define ITEM_NORMAL			1
#define ITEM_MANY			2


enum TheTimeOuts{TO_NEVER,TO_VSLOW,TO_SLOW,TO_NORMAL,TO_QUICK,TO_VQUICK};

enum TheWorldTypes{WT_RANDOM,WT_SHIFTING,WT_NUMBER1,WT_NUMBER2,WT_NUMBER3,WT_NUMBER4,WT_NUMBER5,WT_NUMBER6,WT_NUMBER7,WT_NUMBER8};
#define WT_NUMBEROFWORLDS	8	

#define R_UNLIMITED			0
#define R_3					1
#define R_5					2
#define R_10				3
#define R_20				4
#define R_30				5
#define R_40				6
#define R_50				7
#define R_75				8
#define R_100				9
#define R_200				10

// Must be size-sorted
enum TheWorldSizes{WS_VSMALL,WS_SMALL,WS_NORMAL,WS_LARGE,WS_XLARGE,WS_HUGE,WS_GIGANTIC,WS_EXAGGERATED};

enum TheWallStrengths{ST_WEAK,ST_MODERATE,ST_STRONG,ST_VSTRONG};

enum TheTerrainTypes{TT_LIGHT,TT_MODERATE,TT_ROUGH, TT_RANDOM};
#define TT_TYPES			3
#define TT_LOWEST			TT_LIGHT

#define GS_SLOW				0
#define GS_NORMAL			1
#define GS_FAST				2
#define GS_ULTRA			3

#define VC_BESTOF			0
#define VC_FIRSTTO			1

// Make sure gtOneOnOne is first!
enum GameType{gtOneOnOne, gtCup, gtLeague,  NoOfGameTypes};

enum EnemySettingsType{esBC,esFQ,esUL,eSettings};
enum EnemyType{etDR,etTK,etCA,etMA,eTypes,etNone};

const EnemyType	MType2eType[NumberOfMortals] = {etNone, etNone, etTK, etCA, etMA, etCA, etNone, etNone};
const int			eType2MType[eTypes]	= {M_DR,M_TK,M_CA,M_MA};

const int FQp[eTypes][7]={	{10,17,25,40,55,65,87}, // Droids
							{8,12, 20,30,40,60,80}, // Tanks
							{6,10,18,28,38,57,75}, // Cars
							{6,9,19,29,40,62,77}  // Machines
};
const int ULp[eTypes][7]={	{1,3,5,7,10,20,30},  // Droids
							{2,5,7,9,15,28,40},  // Tanks
							{1,2,4,5,7,10,15},  // Cars
							{1,2,4,5,7,9,13}   // Machines
};
//													    BC	FQ	UL
const int EnemyDefaultSettings[eTypes][eSettings]={	{	1,	3,	2	}, // Droids
													{	0,	3,	3	}, // Tanks
													{	1,	4,	1	}, // Cars
													{	1,	2,	1	}};// Machines


class Game;
class Menu;
class MenuItem;
class ActiveMenuItem;

#include "FadingGameState.h"
#include "SaveableClass.h"
class Settings : public FadingGameState, public SaveableClass
{
private:

	__int64 LastIncrTime;
	ActiveMenuItem * s[(int)NumberOfSettings];
	ActiveMenuItem * EnemySettings[eTypes][eSettings];

	ActiveMenuItem * RightSideControls;

	// Game Setup related

	GameType cgt;

	ActiveMenuItem * scgt[NoOfGameTypes][2];


	void CreateGameSetupItems();
	void CopyGameSetupLinkage();
	void SwitchGameType();
	void SetEnemySettingsUpNeighbours();

protected:
	void InternalUpdateInput();

public:

	Menu * SettingsMenu;
	Menu * EnemySetupMenu;
	Settings();
	~Settings();
	void Blit();
	void Setup();

	void SaveSettings();
	void IncreaseVolume(); // Don't call these!
	void DecreaseVolume(); // Only SoundEngine::XxcreaseVolume must!

	void SetSurfaces();
	void LoadSettings();

	int GetCash();
	int GetVolumeControl();
	int GetVolumeControlMax();
	int GetGammaControl();
	int GetGammaControlMax();
	int GetValue(Setting set);

	int GetUpgradePercentage(EnemyType et);
	int GetExtraEnemyPercentage(EnemyType et);
	int GetEnemyBaseCount(EnemyType et);

	ActiveMenuItem * GetItem(Setting RequestedSetting);
	ActiveMenuItem * GetRightSideControls();
	ActiveMenuItem * GetEnemySetting(EnemySettingsType es,EnemyType et);


};
extern Settings * TheSettings;

#endif

