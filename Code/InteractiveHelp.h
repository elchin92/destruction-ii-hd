#ifndef __InteractiveHelp__ 
#define __InteractiveHelp__

#include "ButtonGameState.h"
#include "FadingGameState.h"

class InteractiveHelp : public ButtonGameState
{
private:


protected:
	void InternalUpdateInput();
	void InternalBlit();
public:

	InteractiveHelp();
	~InteractiveHelp();

	void Setup();

	void SetSurfaces();

};

extern InteractiveHelp * TheInteractiveHelp;

#endif
