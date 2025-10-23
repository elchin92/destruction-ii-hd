#include "NewStore.h"
#include "Game.h"
#include "Weapon.h"
#include "Player.h"
#include "Stats.h"
#include "Start.h"
#include "staticchar.h"
#include "InputEngine.h"
#include "TazerBeam.h"
#include "Settings.h"
#include "InfoBox.h"


#define MajorTitleBox new InfoBox(10, 11, 382, 30, &ShopFont24b, mcGrey, TA_CENTER);
#define MajorInfoBox new InfoBox(10, 228, 382, 92, &ShopFont24b, mcWhite, TA_CENTER);
#define MajorBlitBox new InfoBox(73, 58, 250, 150, &ShopFont16b, mcGrey, TA_CENTER);

NewStore * TheNewStore;


NewStore::NewStore(){

	Initialize();


	ShortInfoBox = new InfoBox(18, 382, 97, 78, &ShopFont12b, mcWhite);
	CurrentItemBox[0] = MajorTitleBox;		
	WeaponBox[0] = MajorTitleBox;	
	CurrentItemBox[1] = MajorBlitBox;
	WeaponBox[1] = MajorBlitBox;
	CurrentItemBox[2] = MajorInfoBox;
	WeaponBox[2] = MajorInfoBox;
	UserMsgBox = new InfoBox(10,562, 780, 30, &ShopFont24, mcGreen,TA_CENTER);
	MiscBox	 = new InfoBox(275, 343, 112, 193, &ShopFont12b, mcWhite);
	ArmorBox = new InfoBox(6, 366, 253, 154, &ShopFont16b, mcWhite);
	PriceBox = new InfoBox(10, 337, 117, 24, &ShopFont16b, mcWhite,TA_CENTER);
	GotBox = new InfoBox(299, 337, 92,  24, &ShopFont16b, mcWhite,TA_CENTER);
	for(int sp=0;sp<3;sp++){
		SpecialBox[sp]= new InfoBox(132+89*sp, 386, 72, 72,  &ShopFont16b, mcBlue, TA_CENTER);
	}

	BlitFunctions[0][0]=&NewStore::BlitRegularShopTitle; // C++20: & required for member function pointers
	BlitFunctions[0][2]=&NewStore::BlitSelectStoreInfo;
	BlitFunctions[0][1]=&NewStore::BlitSelectStoreItem;
	BlitFunctions[1][0]=&NewStore::BlitRegularShopTitle;
	BlitFunctions[1][2]=&NewStore::BlitWeaponInfo;
	BlitFunctions[1][1]=&NewStore::BlitWeaponItem;
	BlitFunctions[2][0]=&NewStore::BlitRegularShopTitle;
	BlitFunctions[2][2]=&NewStore::BlitAmmoInfo;
	BlitFunctions[2][1]=&NewStore::BlitAmmoItem;
	BlitFunctions[3][0]=&NewStore::BlitRegularShopTitle;
	BlitFunctions[3][2]=&NewStore::BlitArmorInfo;
	BlitFunctions[3][1]=&NewStore::BlitArmorItem;
	BlitFunctions[4][0]=&NewStore::BlitRegularShopTitle;
	BlitFunctions[4][2]=&NewStore::BlitMiscInfo;
	BlitFunctions[4][1]=&NewStore::BlitMiscItem;
	BlitFunctions[5][0]=&NewStore::BlitRegularShopTitle;
	BlitFunctions[5][2]=&NewStore::BlitSpecialInfo;
	BlitFunctions[5][1]=&NewStore::BlitSpecialItem;

	SetPriceAndInfo();

	for(int two=0;two<2;two++){
		g_pDDSShopSelected[two]=NULL;
		g_pDDSShopNormal[two]=NULL;
		g_pDDSShopPlayerInfo[two]=NULL;
		g_pDDSEnterStore[0][two]=NULL;
		g_pDDSEnterStore[1][two]=NULL;
		g_pDDSLeaveStore[two]=NULL;
	}
	g_pDDSStoreBottom=NULL;
	g_pDDSSection=NULL;    
	g_pDDSWeapons=NULL;    
	g_pDDSAmmo=NULL;   
	g_pDDSArmor=NULL;  
	g_pDDSMisc=NULL;   
	g_pDDSSpecial=NULL;   
	
	SectionSurfaces[0]=&g_pDDSSection;
	SectionSurfaces[1]=&g_pDDSWeapons;
	SectionSurfaces[2]=&g_pDDSAmmo;
	SectionSurfaces[3]=&g_pDDSArmor;
	SectionSurfaces[4]=&g_pDDSMisc;
	SectionSurfaces[5]=&g_pDDSSpecial;

	cpl=1;
	WhosTurn=1;
	KEEPSETTINGS=FALSE;


	ShopFont12b=CreateFont(12,
		0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");
	ShopFont16b=CreateFont(16,
		0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");
	ShopFont21=CreateFont(21,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");
	ShopFont24=CreateFont(24,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");
	ShopFont24b=CreateFont(24,
		0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");

	for(int s=0;s<2;s++){
		Selection[s]=Section[s]=0;
		Choice[s]=1;
		ButtonFunctions[s][0]=&NewStore::OnLeft; // C++20: & required
		ButtonFunctions[s][1]=&NewStore::OnRight;
		ButtonFunctions[s][2]=&NewStore::OnEnterShop;
		ButtonFunctions[s][3]=&NewStore::OnEnterShop;
		ButtonFunctions[s][4]=&NewStore::OnGive;
		ButtonFunctions[s][5]=&NewStore::OnSwitch;
		ButtonFunctions[s][6]=&NewStore::OnDetails;
		ButtonFunctions[s][7]=&NewStore::OnQuit;
		ButtonFunctions[s][8]=&NewStore::OnGoBack;
		ButtonFunctions[s][9]=&NewStore::DoNothing;
	}



}
NewStore::~NewStore(){
	DeleteObject(ShopFont12b);
	DeleteObject(ShopFont16b);
	DeleteObject(ShopFont21);
	DeleteObject(ShopFont24);
	DeleteObject(ShopFont24b);

	delete ShortInfoBox;
	delete CurrentItemBox[0];		
	delete WeaponBox[0];	
	delete CurrentItemBox[1];
	delete WeaponBox[1];
	delete CurrentItemBox[2];
	delete WeaponBox[2];
	delete UserMsgBox;
	delete MiscBox;
	delete ArmorBox;
	delete PriceBox;
	delete GotBox;
	for(int sp=0;sp<3;sp++){
		delete SpecialBox[sp];
	}
}



void NewStore::SwitchPlayer(){
	cpl=!cpl;
	apl=ThePlayers[cpl];
}

void NewStore::Setup(){


	if(!KEEPSETTINGS){

		DoubleShop=AL_DS;

		KEEPSETTINGS=TRUE;
		if(DoubleShop){
			SetupPlayer(1);	
			SetupPlayer(0);	

		}else{
			WhosTurn=!WhosTurn;
			SetupPlayer(WhosTurn);
		}
	}
	RegisterKeys();

}

void NewStore::SetupPlayer(int pl){
	cpl = pl;
	apl=ThePlayers[pl];
	Choice[pl]=1;
	Selection[pl]=0;
	Section[pl]=0;
	apl->ReceiveDonation();
	ButtonFunctions[pl][2]=&NewStore::OnEnterShop; // C++20: & required
	ButtonFunctions[pl][3]=&NewStore::OnEnterShop;
}

/*void NewStore::FixPlayerInfo(int who){

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

}*/
const char * StoreBitmaps[6]={ShopSectionBitmap, ShopWeaponsBitmap, ShopAmmoBitmap, ShopArmorBitmap, ShopMiscBitmap, ShopSpecialBitmap};


void NewStore::SetSection(int Sec){

	Section[cpl] = Sec;

	
}

void NewStore::SetSurfaces(){
	for(int i=0;i<6;i++)
		AddBitmapAndSurface(StoreBitmaps[i], SectionSurfaces[i]);

	AddBitmapAndSurface(LeftShopSelectedBitmap, &g_pDDSShopSelected[0]);
	AddBitmapAndSurface(LeftShopNormalBitmap, &g_pDDSShopNormal[0]);
	AddBitmapAndSurface(LeftPlayerInfoBitmap, &g_pDDSShopPlayerInfo[0]);
	AddBitmapAndSurface(RightShopSelectedBitmap, &g_pDDSShopSelected[1]);
	AddBitmapAndSurface(RightShopNormalBitmap, &g_pDDSShopNormal[1]);
	AddBitmapAndSurface(RightPlayerInfoBitmap, &g_pDDSShopPlayerInfo[1]);
	AddBitmapAndSurface(StoreBottomBitmap, &g_pDDSStoreBottom);
	AddBitmapAndSurface(LeftShopEnterStoreNormalBitmap, &g_pDDSEnterStore[0][0]);
	AddBitmapAndSurface(LeftShopEnterStoreSelectedBitmap, &g_pDDSEnterStore[0][1]);
	AddBitmapAndSurface(RightShopEnterStoreNormalBitmap, &g_pDDSEnterStore[1][0]);
	AddBitmapAndSurface(RightShopEnterStoreSelectedBitmap, &g_pDDSEnterStore[1][1]);
	AddBitmapAndSurface("LeaveStoreNormal", &g_pDDSLeaveStore[0]);
	AddBitmapAndSurface("LeaveStoreSelected", &g_pDDSLeaveStore[1]);
}


