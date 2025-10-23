#ifndef __MovementObject__ 
#define __MovementObject__

class Game;
class Actor;
class Mortal;
class shot;
class MovementEvent;

class MovementObject
{
private:

	int MP, MovingTwice,  xy, Way[2];
	int MoverPos[3][2];
	int CollidePos[3][2];
	int CollisionDist[2];
	Actor * Mover;
	
public:

	MovementEvent * nextStop;

	MovementObject();
	~MovementObject();

	void SetMover(Actor * iMover);
	void SetXYWay4();
	void SetXYWay8();
	void SetMP(int MP);

	void MoveAskew(int aMP);
	void Reset();
	void SaveMP(int MovedMP);

	MovementEvent * MoveAndAct();

	int GetMP();
	int CalculateNewMP();

	// Collision Related

	void SaveSquareMP();
	void DetectCollisions(int CollideWithWalls=TRUE);
	void DetectMortalCollisions();
	void DetectShotCollisions();
	void DetectWallCollisions();
	void DetectOutOfMapCollisions();
	void DetectOutOfSquareCollisions();
	void DetectNewCenterSquareDistance();
	void AddEvent(MovementEvent * me);
	void CalculateCollisionDistances();
	void SetColliderCollisionPos(shot * Collider);
	void SetColliderCollisionPos(Mortal * Collider);
	void SetMoverCollisionPos();

	int WallBit(int wxy, int wWay);



};
extern	MovementObject *			mo;
#endif