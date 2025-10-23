#include "SliderMenuItem.h"

#ifndef __GraphicalSliderMenuItem__ 
#define __GraphicalSliderMenuItem__

class GraphicalSliderMenuItem : public SliderMenuItem
{
private:

public:

	GraphicalSliderMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle,int iStart, int iMax);
	~GraphicalSliderMenuItem();

	void InternalBlit(int Selected);
};
#endif