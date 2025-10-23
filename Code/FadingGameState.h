#include "GameState.h"

#ifndef __FadingGameState__ 
#define __FadingGameState__

class FadingGameState : public GameState
{
protected:
	int lBrightness, FadeTime[2], FADEDOWN;
	__int64 NextFade;

	void Initialize();
public:

	virtual ~FadingGameState();

	void SwitchState(gState iNextState, int SubMenu=0);
	void SetFadeInTime(int iFadeTime);
	void SetFadeOutTime(int iFadeTime);
	virtual void InitializeFadeIn();
	virtual void UpdateFade();
	virtual int IsSlideShow();
	virtual int PerformSwitch();
	int IsFading();

	COLORREF GetWhite();
	COLORREF GetRed();
	COLORREF GetGreen();
	COLORREF GetBlue();
	COLORREF ContrastAdjust(COLORREF rgb);
	COLORREF ContrastAdjust(int r, int g, int b);

};
#endif

