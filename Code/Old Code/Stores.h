/*
#ifndef __Stores__ 
#define __Stores__

class Game;
class Weapon;
#include "GameState.h"
#include "definitions.h"
const int	StoreInfo[6][4]={5 ,250,150,5,
							 27,250,115,10,
							 20,250,150,5,
							 15,70 ,140,3,
							 MISCITEMS,250,150,6,
							 17,80 ,80 ,4 };

class Stores : public GameState
{
private:
	int Choice, Selection, Section;
	int ActivePlayer;
	int KEEPSETTINGS;
	char Information[4][30][3][50];
	Weapon *					SelectedWeapon;

protected:
	void InternalUpdateInput();
public:
	int	ItemPrice[5][30];
	Stores();
	~Stores();
	void Blit();
	void Setup();

	void FixPlayerInfo(int who);
	void SetSection(int Sec);
	void SetSurfaces();
	void BlitShop(int w);
	void BlitInfo(int w);
	void BlitWeaponInfo(int x, int y, Weapon * aWeapon);
};

extern Stores * TheStores;
#endif

*/