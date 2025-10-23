#ifndef __PLSetup__ 
#define __PLSetup__

class Game;

#include "GameState.h"

class PLSetup : public GameState
{
private:

public:
	PLSetup();
	~PLSetup();
	void Blit();
	void Setup();
	void Input(int key);
};
#endif

