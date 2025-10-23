#ifndef __GettingStarted__ 
#define __GettingStarted__

#include "FadingGameState.h"

class GettingStarted : public FadingGameState
{
private:


protected:
	void InternalUpdateInput();

public:

	GettingStarted();
	~GettingStarted();

	void Blit();
	void Setup();
	void SetupSurfaces();

	void SetSurfaces();

};

extern GettingStarted * TheGettingStarted;

#endif
