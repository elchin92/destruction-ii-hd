#ifndef __IntegerMonitor__ 
#define __IntegerMonitor__

#include "DebugMonitor.h"

class IntegerMonitor : public DebugMonitor
{
private:

	int * p;
public:

	IntegerMonitor(char * iname, int * ip, int ix, int iy,int iw);
	~IntegerMonitor();

	void Blit();

};
#endif

