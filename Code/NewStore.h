#ifndef __NewStore__ 
#define __NewStore__

class Player;
class Weapon;
class InfoBox;



#include "ButtonGameState.h"
#include "definitions.h"

const int	StoreInfo[6][4]={5 ,250,150,5,
							 27,250,115,10,
							 AMMOITEMS,250,150,5,
							 15,70 ,140,3,
							 MISCITEMS,250,150,6,
							 17,80 ,80 ,4 };



class NewStore : public ButtonGameState
{
private:
	int Choice[2], Selection[2], Section[2];
	int WhosTurn;
	int cpl, DoubleShop;
	int KEEPSETTINGS;
	char Information[6][30][4][50];

	Weapon * CurrentWeapon;
	Player * apl;


	InfoBox * ShortInfoBox;
	InfoBox * CurrentItemBox[3];
	InfoBox * WeaponBox[3];
	InfoBox * MiscBox;
	InfoBox * ArmorBox;
	InfoBox * PriceBox;
	InfoBox * GotBox;
	InfoBox * SpecialBox[3];


	// Don't forget to zero these surfaces!!

	LPDIRECTDRAWSURFACE7 g_pDDSShopSelected[2],g_pDDSShopNormal[2],
						 g_pDDSShopPlayerInfo[2], g_pDDSEnterStore[2][2],
						 g_pDDSLeaveStore[2], g_pDDSStoreBottom;

	LPDIRECTDRAWSURFACE7        g_pDDSSection ;   
	LPDIRECTDRAWSURFACE7        g_pDDSWeapons ;   
	LPDIRECTDRAWSURFACE7        g_pDDSAmmo ;  
	LPDIRECTDRAWSURFACE7        g_pDDSArmor ;   
	LPDIRECTDRAWSURFACE7        g_pDDSMisc ;   
	LPDIRECTDRAWSURFACE7        g_pDDSSpecial ;  // 19 surfaces, 21 is MaximumNumberOfSurfaces!
	
	LPDIRECTDRAWSURFACE7        * SectionSurfaces[6];  

	HFONT ShopFont12b,ShopFont16b, ShopFont21, ShopFont24, ShopFont24b;

	void (NewStore::*(ButtonFunctions[2][10]))();
	void (NewStore::*(BlitFunctions[6][3]))(InfoBox * Box);

	void SwitchPlayer();

	// PlayerInfo Side
	void BlitMiscInfo(int w);
	void BlitArmorInfo(int w);
	void BlitWeaponInfo(int w);

	// Shop Side
	void BlitCurrentItem(int w);
	void BlitSpecials(int w);
	void BlitShortInfo(int w);
	void BlitPriceAndGot(int w);
	void BlitEnterStoreButton(int w);
	void BlitLeaveStoreButton(int w);
	void BlitSelectedButton(int w);

	// Both sides
	void BlitMessageGfx();

	// Current Item

	void BlitRegularShopInfo(InfoBox * Box);
	void BlitSelectStoreInfo(InfoBox * Box);
	void BlitAmmoInfo(InfoBox * Box);
	void BlitMiscInfo(InfoBox * Box);
	void BlitSpecialInfo(InfoBox * Box);
	void BlitArmorInfo(InfoBox * Box);
	void BlitWeaponInfo(InfoBox * Box);

	void BlitRegularShopTitle(InfoBox * Box);
	void BlitWeaponTitle(InfoBox * Box);

	void BlitRegularShopItem(InfoBox * Box, int xa=10000, int ya=10000);
	void BlitSelectStoreItem(InfoBox * Box);
	void BlitAmmoItem(InfoBox * Box);
	void BlitMiscItem(InfoBox * Box);
	void BlitSpecialItem(InfoBox * Box);
	void BlitArmorItem(InfoBox * Box);
	void BlitWeaponItem(InfoBox * Box);


	// General

	void SetupPlayer(int pl);
	void LeaveStore();

    // NewStoreInput.cpp

	// Keys
	int esc[2],action[2],left[2],right[2],up[2],down[2];


	void StoreInput();
	void RegisterKeys();


	// Buttons
	void OnBuy();
	void OnSell();
	void OnEnterShop();
	void OnLeft();
	void OnRight();
	void OnQuit();
	void OnGive();
	void OnDetails();
	void OnSwitch();
	void DoNothing();
	void OnGoBack();

	// NewStoreItemInfo.cpp
	void SetPriceAndInfo();


protected:
	void InternalUpdateInput();
	void InternalBlit();

public:
	int	ItemPrice[5][30];
	NewStore();
	~NewStore();
	void Setup();

//	void FixPlayerInfo(int who);
	void SetSection(int Sec);
	void SetSurfaces();
	void BlitShop(int w);
	void BlitInfo(int w);
//	void BlitWeaponInfo(int x, int y, Weapon * aWeapon);
};

extern NewStore * TheNewStore;
#endif

