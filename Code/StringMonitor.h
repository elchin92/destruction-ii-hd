#ifndef __StringMonitor__ 
#define __StringMonitor__

#include "DebugMonitor.h"

class StringMonitor : public DebugMonitor
{
private:

	char * p;
public:

	StringMonitor( const char * iname, char * ip, int ix, int iy,int iw);
	~StringMonitor();

	void Blit();

};
#endif

