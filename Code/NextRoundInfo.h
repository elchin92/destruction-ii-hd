#ifndef __NextRoundInfo__ 
#define __NextRoundInfo__

#include "FadingGameState.h"

class InfoBox;

class NextRoundInfo : public FadingGameState
{
private:

	InfoBox * LeftPlayerBox, * RightPlayerBox;
	InfoBox * LeftPlayerSBox, * RightPlayerSBox;
	InfoBox * CurrentMatchBox;
	InfoBox * CurrentRoundBox, * MatchRoundBox;

	HFONT tFont;
protected:
	void InternalUpdateInput();
	
public:

	NextRoundInfo();
	~NextRoundInfo();

	void Blit();
	void Setup();

	void SetSurfaces();

};

extern NextRoundInfo * TheNextRoundInfo;

#endif
