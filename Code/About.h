#ifndef __About__ 
#define __About__

#include "FadingGameState.h"

class Game;


class About : public FadingGameState
{
private:

protected:
	void InternalUpdateInput();

public:
	About();
	~About();
	void Blit();
	void Setup();

	void SetSurfaces();
};
#endif

