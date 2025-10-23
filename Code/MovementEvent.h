#ifndef __MovementEvent__ 
#define __MovementEvent__

enum StopType{ST_SAVEMP,ST_TOTALSTOP,ST_CONTINUE,ST_SLIDEPRIMARY, ST_SLIDESECONDARY};

class MovementEvent
{
protected:

	int MP;
	StopType st;

public:

	MovementEvent * nextEvent;
	
	virtual ~MovementEvent();

	void Initialize();
	void insertEvent(MovementEvent * newEvent);
	virtual void Act()=0;
	virtual int KamikazeEvent();
	MovementEvent * ActLoop();
	StopType GetStopType();

	int GetMP();

};
#endif