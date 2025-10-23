#ifndef __Controls__ 
#define __Controls__

	//-----------------------------------------------------------------------------
	// Key Names
	//-----------------------------------------------------------------------------

#define INGAMEKEYS		14
static char	 KeyNames[INGAMEKEYS][30]= {"Go North","Go South","Go West","Go East",
								   "Primary Fire","Alternative Fire","Drop Charge","Throw Grenade",
								   "Drop Mine","Drop Tank Mine","Switch Weapon",
								   "Activate Special 1","Activate Special 2","Activate Special 3"};

#include "ButtonGameState.h"


#ifndef DIK_PAUSE
#include "SDL2_DirectInputCompat.h"
#endif

class Game;
class Menu;
class Player;

const int	ReservedKeys=5;
const int	Reserved[ReservedKeys]={DIK_PAUSE,DIK_ADD,DIK_SUBTRACT,DIK_ESCAPE};

class Controls : public ButtonGameState
{
private:

	int lJoysUsed;
	__int64 NextJoyCheck;
	int Side;
	int Keys[2][INGAMEKEYS];

	void UpdateJoyInfo();
protected:
	void InternalUpdateInput();
	void InternalBlit();
public:

	Menu * ControlsMenu[2];

	Controls();
	~Controls();

	void Setup();
	void SetupSurfaces();
	void SaveControls();
	void LoadControls();
	void SetSurfaces();
	void SetControls(Player * LeftPlayer, Player * RightPlayer);
	void SetKey(int id, int Key, int iSide);
	void SwitchKeys();
	void PrintKeys();

	int KeyFree(int key, int id, int side);
	int GetCurrentSideKey(int id);
	int GetKey(int id, int Side);

	const char * GetCurrentSideKeyName(int id);

};

extern Controls * TheControls;
#endif

