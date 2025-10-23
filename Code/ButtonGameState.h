#include "FadingGameState.h"

#ifndef __ButtonGameState__ 
#define __ButtonGameState__

class Menu;

class ButtonGameState : public FadingGameState
{
private:
	void BlitButtonMenu();

protected:

	LPDIRECTDRAWSURFACE7 * BackgroundSurface;
	void Initialize();

	virtual void InternalBlit()=0;

public:

	Menu * ButtonMenu;

	void Blit();
	virtual ~ButtonGameState();
};
#endif

