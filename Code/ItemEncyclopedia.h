#ifndef __ItemEncyclopedia__ 
#define __ItemEncyclopedia__

#include "FadingGameState.h"

class ItemEncyclopedia : public FadingGameState
{
private:


protected:
	void InternalUpdateInput();

public:

	ItemEncyclopedia();
	~ItemEncyclopedia();

	void Blit();
	void Setup();

	void SetSurfaces();
	void SetupSurfaces();

};

extern ItemEncyclopedia * TheItemEncyclopedia;

#endif
