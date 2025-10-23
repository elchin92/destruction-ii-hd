#include "FadingGameState.h"

#ifndef __SlideShowGameState__ 
#define __SlideShowGameState__

const int MAXSLIDES = 5; // Don't have the patience to make it dynamic

class SlideShowGameState : public FadingGameState
{
protected:
	int Slides, SlidesLeft;
	int SlideShowFadeTime[MAXSLIDES][2];
	int AutoFade[MAXSLIDES];
	void Initialize(int iSlides);

public:

	void InitializeFadeIn();
	void FixCorrectImage();
	void SubtractSlide();
	int IsSlideShow();
	int PerformSwitch();
	int AutomaticFadeDown();
	void SetSSFadeTime(int Slide, int iFadeInTime, int iFadeOutTime, int iAutoFadeTime=0);

};
#endif

