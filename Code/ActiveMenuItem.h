#include "MenuItem.h"

#ifndef __ActiveMenuItem__ 
#define __ActiveMenuItem__

#define LOCK_NEEDTOREGISTER					0x0000002
#define LOCK_TAKEEFFECTNEXTROUND			0x0000004
#define LOCK_TAKEEFFECTNEXTGAME				0x0000008
#define LOCK_NOTAVAILABLEINTHISVERSION		0x0000010
#define LOCK_UNLIMITEDZERO					0x0000020
#define LOCK_NOTWHILEGAMEACTIVE				0x0000040


#define UnlimitedZero() (IsLocked(LOCK_UNLIMITEDZERO))

class ActiveMenuItem : public MenuItem
{
private:
	int StatusBits;


	ActiveMenuItem * LeftNeighbour;
	ActiveMenuItem * RightNeighbour;
	ActiveMenuItem * UpNeighbour;
	ActiveMenuItem * DownNeighbour;

	virtual void InternalUpdateInput()=0;


protected:

	int ReturnNext;

	int InternalSelection, ExternalSelection;
	int MoveToNeighbours, CatchingEscape;

	int IsLocked(int LockType);

public:

	ActiveMenuItem * nextActiveMenuItem;

	virtual ~ActiveMenuItem();

	void Initialize();
	void Lock(int LockType);
	ActiveMenuItem * UpdateInput();
	void ValidateDataLoop(int LockType);
	void SetSelection(int newSelection);
	void AddToSelection(int addition);

	void Blit(int Selected);
	virtual void InternalBlit(int Selected)=0;



	ActiveMenuItem * GetLeftNeighbour();
	ActiveMenuItem * GetRightNeighbour();
	ActiveMenuItem * GetUpNeighbour();
	ActiveMenuItem * GetDownNeighbour();

	void SetLeftNeighbour(ActiveMenuItem * l);
	void SetRightNeighbour(ActiveMenuItem * r);
	void SetUpNeighbour(ActiveMenuItem * u);
	void SetDownNeighbour(ActiveMenuItem * d);

	void ActivateNeighbourLinkage();

	int CatchesEscape();
	int GetExternalSelection();
	int GetInternalSelection();
	int IsActive();
	int Unlocked(int DisplayMessage=1); // Also prints out messages if locked

	int * GetISP();


};
#endif