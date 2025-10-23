#include "ActiveMenuItem.h"
#include <ddraw.h>
#include <windef.h>
#ifndef __ButtonMenuItem__ 
#define __ButtonMenuItem__

class ButtonMenuItem : public ActiveMenuItem
{
private:

	RECT Trigger;
	LPDIRECTDRAWSURFACE7 * SelectedSurface;

	typedef void (*bf)();
	bf ButtonFunction;

	void InternalUpdateInput();

public:

	ButtonMenuItem(int ix, int iy, int w, int h, int ShineMargin,LPDIRECTDRAWSURFACE7 * iSelectedSurface, bf iButtonFunction);
	~ButtonMenuItem();

	void InternalBlit(int Selected);
};
/*
void OnGettingStarted();
void OnItemEncyclopedia();
void OnSettingsExplanation();
void OnFullVersion();
void OnDetailedInstructions();
void OnEnemyEncyclopedia();
void OnAbout();
void OnVisitWebsite();
*/
#endif