#include "SliderMenuItem.h"

#ifndef __NumericalSliderMenuItem__ 
#define __NumericalSliderMenuItem__

class NumericalSliderMenuItem : public SliderMenuItem
{
private:

	int Pre;
	char * Unit;
public:

	NumericalSliderMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle, int iStartNumber, int iMin, int iMax, int iMulitiplier, char * unit=NULL, int pre=0);
	NumericalSliderMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle,  int iStart, int iMin, int * iMaxPointer, int iMulitiplier);

	~NumericalSliderMenuItem();

	void InternalBlit(int Selected);
};
#endif