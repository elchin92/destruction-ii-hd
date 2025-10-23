#ifndef __Start__ 
#define __Start__

class GraphicMenu;

#include "ButtonGameState.h"

class Start : public ButtonGameState
{
protected:
	void InternalUpdateInput();
	void InternalBlit();
public:
	Start();
	~Start();
	void Setup();
	void SetSurfaces();
	void SetResumeSurfaces();

};

extern Start * TheStart;
#endif

