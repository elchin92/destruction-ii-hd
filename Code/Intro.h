#ifndef __Intro__ 
#define __Intro__

#include "SlideShowGameState.h"

class Intro : public SlideShowGameState
{


protected:
	void InternalUpdateInput();

public:
	Intro();
	~Intro();
	void Blit();
	void Setup();

	void SetSurfaces();
};

extern Intro * TheIntro;
#endif

