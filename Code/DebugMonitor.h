#ifndef __DebugMonitor__ 
#define __DebugMonitor__

class DebugWindow;

class DebugMonitor
{
protected:

	int x,y,w;
	char name[40];

	void Initialize();

public:

//	~DebugMonitor();
	DebugMonitor * nextMonitor;

	virtual void Blit()=0;

};
#endif

