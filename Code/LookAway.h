#ifndef __LookAway__ 
#define __LookAway__

#include "FadingGameState.h"

class Game;


class LookAway : public FadingGameState
{
private:
	int State, KEEPSETTINGS;
	char * LABitmaps[2];
public:
	LookAway();
	~LookAway();
	void Blit();
	void Setup();
	void InternalUpdateInput();
	void SetSurfaces();
};
#endif

