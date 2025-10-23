#include "GameState.h"

#ifndef __Play__ 
#define __Play__


class Play : public GameState
{
private:

	int PlayStatus;
	int MAGICDONE;

protected:
	void InternalUpdateInput();

public:

	Play();
	~Play();

	void Blit();
	void Setup();
	void SetupSurfaces();
	void SetSurfaces();
	void EndRound();
	void NewGame();
	
	void SetPlayStatus(int iPlayStatus);
	int GetPlayStatus();

};

extern Play * ThePlay;
#endif