#ifndef __EnemyEncyclopedia__ 
#define __EnemyEncyclopedia__

#include "FadingGameState.h"

class EnemyEncyclopedia : public FadingGameState
{
private:


protected:
	void InternalUpdateInput();

public:

	EnemyEncyclopedia();
	~EnemyEncyclopedia();

	void Blit();
	void Setup();
	void SetupSurfaces();

	void SetSurfaces();

};

extern EnemyEncyclopedia * TheEnemyEncyclopedia;

#endif
