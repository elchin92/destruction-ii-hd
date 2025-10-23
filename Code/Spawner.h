#ifndef __Spawner__ 
#define __Spawner__

#include "definitions.h"

class Spawner
{
private:

	int SumOfCashEarned;

	void SpawnMortal(int MType, int uType);
	void UpdateSum();

	int GetEarnedPoints();

public:

	Spawner();
	~Spawner();

	int GetThick(int ThickType);

	void SpawnEnemiesForNewRound();
	void SpawnParticularMortal(int MType);
	void SpawnRandomEnemy();
	void CreateMortal(int x, int y, int Type, int Team);

	int CreateMortalOK(ThicknessType tType, int X, int Y);
	int CreateItemOK(int X, int Y, int BulletSensitive=FALSE);
	int CreateGeneralObjectOK(int Thickness, int X, int Y, int BulletSensitive);



};
#endif

extern Spawner * TheSpawnPit;