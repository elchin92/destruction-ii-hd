#ifndef __GetReady__ 
#define __GetReady__

class Game;
#include "FadingGameState.h"

class GetReady : public FadingGameState
{
private:
	int READY[2], TOGGLEALLOWED;
protected:
	void InternalUpdateInput();

public:
	GetReady();
	~GetReady();
	void Blit();
	void Setup();

	void SetSurfaces();
};
#endif


