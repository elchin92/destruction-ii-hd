#ifndef __LeagueInfo__ 
#define __LeagueInfo__

#include "FadingGameState.h"

class InfoBox;


class LeagueInfo : public FadingGameState
{
private:
	HFONT	LeagueFont;
	InfoBox * LeagueBox;
	InfoBox * OutOfBox[2];

protected:
	void InternalUpdateInput();

public:

	LeagueInfo();
	~LeagueInfo();

	void Blit();
	void Setup();

	void SetSurfaces();

};

extern LeagueInfo * TheLeagueInfo;

#endif
