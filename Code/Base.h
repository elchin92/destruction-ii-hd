#ifndef __Base__ 
#define __Base__

#include <windows.h>
class Player;
class Game;

class Base
{
private:

	int * WorldSize;
	int Corner;
	int Door[2];
	int DoorCoords[2][2];
	int Coords[2];
	int TIMEOUT;
	int Rolling[2];
	Player * Owner;
	Player * Opponent;

public:


	Base(int theCorner);
	~Base();

	void Update();
	void UpdateBarriers();
	void Close(int Door);
	void Open(int Door);
	void SetTimeOut();
	void SetOpponent(Player * iOpponent);
	void SetOwner(Player * iOwner);
	void CreateBase();
	int Sense(int Door);
	int WithinVicinity(Player * Who, int Door);
	int GetTicks(int door);

};

extern	Base *						TheBases[2];
#endif

