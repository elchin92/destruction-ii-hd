#ifndef __DoubleMonitor__ 
#define __DoubleMonitor__

#include "DebugMonitor.h"

class DoubleMonitor : public DebugMonitor
{
private:

	double * p;
public:

	DoubleMonitor(char * iname, double * ip, int ix, int iy,int iw);
	~DoubleMonitor();

	void Blit();

};
#endif

