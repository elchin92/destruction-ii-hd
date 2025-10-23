#include <windows.h>
DECLARE_HANDLE(HPEN);
DECLARE_HANDLE(HFONT);
DECLARE_HANDLE(HBRUSH);



#ifndef __MenuStyle__ 
#define __MenuStyle__

enum PredefinedStyle;

class Menu;
class ActiveMenuItem;
class MenuStyle
{
private:

	COLORREF SelectedColor, UnselectedColor;
	COLORREF DiscreteColor;
	COLORREF BorderColor;
	COLORREF MarkedColor;

	HFONT						MenuFont;
	HPEN						MarkedPen,  BorderPen;
	HBRUSH						MarkedBrush,BorderBrush;

	int MenuAlign;

	// Item Position Related

	int Cols, NoItemToItemDistance[2];
	int ItemDimension[2],ItemToItemDistance[2], ItemToEdgeDistance[2], MarkedEdgeToTextDistance[2];
	int Shadow, MarkedRound[2], XAdjust;
	int BorderThickness, EndBorderToNextItemDistance, BorderRound;
	int SlideAdjust, SelectKeyAdjust;


	int TotalHeight;
	
	// Used when deciding Menu style dimensions

	void SetItemPositions();
	void CalculateMenuDimensions(int XPlay, int YPlay, int LockedCols, int LockedFontSize);

	int GetTotalHeight(int Items,int Borders, int Rows, int LastItemIsABorder);
	int GetTotalWidth();


	// Used when creating brushes & pens

	void CreateBorderItems();
	void CreateMarkedItems();

	MenuStyle * SisterStyle;

public:

	Menu * OwnerMenu;

	MenuStyle(Menu * iOwnerMenu, MenuStyle * SisterStyle=NULL);
	~MenuStyle();
	
	void CreatePredefinedStyle(int Width, int Height, PredefinedStyle pdm);
	int GetColumns();
	int GetNextMenuYPos();
	int GetMenuAlign();
	void SetTemporaryMenuAlign(int ta);

	// Used in Menu.cpp to decide how the menu is supposed to look like

	void SetMenuFont(HFONT iMenuFont);
	void SetMenuSelectedColor(COLORREF iSelectedColor, COLORREF iSelectedBackgroundColor=0);
	void SetMenuUnselectedColor(COLORREF iUnselectedColor);
	void SetMenuBorderPrefs(COLORREF iBorderColor, int iBorderWidth, int iBorderRounding);

	// Set by Menu before blitting MenuItems

	void SetTextAttributes();


	// Used when blitting various MenuItems.

	void SetBorderStyle();
	void SetBorderTitleStyle();
	void SetMarkedStyle();
	void SetDiscreteStyle();
	void SetSelectedStyle();
	void SetUnselectedStyle();
	void SetStyle(int Selected);
	void BlitMark(ActiveMenuItem * TheItem, int Selected);

	void SetSlideAdjust(int issa);
	void SetSelectKeyAdjust(int iska);
	int GetSlideAdjust();
	int GetSelectKeyAdjust();
	int GetXAdjust();


	// Used by Sister MenuStyle

	int GetBorderThickness();
	int GetMarkedRound(int i);
	int GetItemToEdgeDistance(int i);
	int GetMETTD(int i);
	int GetBorderRound();

};
#endif
