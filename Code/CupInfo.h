#ifndef __CupInfo__ 
#define __CupInfo__

#include "GameState.h"

class CupInfo : public GameState
{
private:
	HFONT CupFont;
protected:
	void InternalUpdateInput();

public:

	CupInfo();
	~CupInfo();

	void Blit();
	void Setup();

	void SetSurfaces();

};

extern CupInfo * TheCupInfo;

#endif
