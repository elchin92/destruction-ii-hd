#ifndef __Channel__ 
#define __Channel__

#include "DebugMonitor.h"

class Channel : public DebugMonitor
{
private:

	char msg[50];
public:

	Channel( const char * iname, const char * imsg, int ix, int iy,int iw);
	~Channel();

	void Blit();

};
#endif

