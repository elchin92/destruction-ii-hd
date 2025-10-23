#include "windows.h"


#ifndef __Menu__
#define __Menu__

#include <ddraw.h>

enum PredefinedStyle{pLoadSave,pSettings,pControls,pGameSetup,pEnemySetup, pPlayerSetup};

enum gState;
class Game;
class MenuItem;
class BorderMenuItem;
class BorderEndMenuItem;
class MenuStyle;
class ActiveMenuItem;
class MarkableMenuItem;

const int MAXMENUITEMS=200;
const int MAXSELECTIONS=15;


class Menu		
{

protected:

	int MenuPos[2], Width, Height;
	int LastAddedOption, MultipleMarks;

	MenuStyle * TheMenuStyle;
	MenuStyle * SisterStyle; // Must be saved for ClearMenu()

	ActiveMenuItem * TheActiveMenuItems;

	MenuItem * TheMenuItems;
	BorderMenuItem * LastBorder;
	ActiveMenuItem * LastAddedActiveItem;

	ActiveMenuItem * SelectedItem;

public:

	MarkableMenuItem * TheMarkableItems;
	ActiveMenuItem ** pSelectedItem;

	Menu(MenuStyle * SisterStyle=NULL);
	~Menu();
	void Initialize();

	void BlitMenu();
	void SetPos(int X, int Y);
	void UpdateInput();
	void SetNeighbours(int Columns);

	void ValidateData(int LockType);

	MenuItem * GetMenuItems();

	// Menu Creating Calls

	ActiveMenuItem * AddMenuItem(MenuItem * mi, int IsOption=FALSE);

	ActiveMenuItem * AddNumericalSlider(const char * SliderTitle,int Start, int Min, int Max, int Step=1, char * unit=NULL, int pre=0);
	ActiveMenuItem * AddNumericalSliderWPM(const char * SliderTitle,int Start, int Min, int * MaxPointer, int Step=1);
	ActiveMenuItem * AddGraphicalSlider(const char * SliderTitle,int Start, int Max);
	ActiveMenuItem * AddSwitch(const char * SliderTitle, gState nextState,int SubMenu);
	ActiveMenuItem * AddSelectKey(const char * KeyName, int iid, int iside);
	ActiveMenuItem * AddOptionItem(const char * OptionItemName="");
	ActiveMenuItem * AddMarkableItem(const char * Title, int mx, int my, int mid, LPDIRECTDRAWSURFACE7 * ms);
	ActiveMenuItem * AddNumberedStaticItem(const char * ItemName, int id);

	void LIAddOption(const char * OptionName, int SetStandard=FALSE);
	void AddStaticItem(const char * Title);

	BorderMenuItem * StartNewBorder(const char * iTitle);
	BorderEndMenuItem * EndBorder();

	void LILock(int iLockType);
	void LISetSelection(int iSelection);

	void LISetUnlimitedZero();

	void CreatePredefinedMenu(int left, int top, int right, int bottom, PredefinedStyle pdm);

	void SetSelections();

	ActiveMenuItem * GetActiveMenuItem(int no);

	int GetItems();
	int GetXPos();
	int GetYPos();
	int GetWidth();
	int GetHeight();
	int GetNextMenuStart();

	void SetChoice(ActiveMenuItem * newChoice);
	ActiveMenuItem * GetChoice();
	ActiveMenuItem ** GetMotherInput();
	void SetMotherInput(ActiveMenuItem ** MotherpSelectedItem);

	int GetMultipleMarks();
	void SetMultipleMarks();
	MarkableMenuItem * GetFirstMarked();

	MenuStyle * GetMenuStyle();



};

#endif