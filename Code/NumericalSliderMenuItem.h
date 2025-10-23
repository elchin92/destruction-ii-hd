#include "SliderMenuItem.h"

#ifndef __NumericalSliderMenuItem__ 
#define __NumericalSliderMenuItem__

class NumericalSliderMenuItem : public SliderMenuItem
{
private:

	int Pre;
	const char * Unit; // C++20: const char*
public:

	NumericalSliderMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle, int iStartNumber, int iMin, int iMax, int iMulitiplier, const char * unit=NULL, int pre=0); // C++20: const char*
	NumericalSliderMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle,  int iStart, int iMin, int * iMaxPointer, int iMulitiplier);

	~NumericalSliderMenuItem();

	void InternalBlit(int Selected);
};
#endif