#include "ActiveMenuItem.h"

#ifndef __SliderMenuItem__ 
#define __SliderMenuItem__



class SliderMenuItem : public ActiveMenuItem
{
protected:

	int tSelection;
	int SlideInterval;
	int Multiplier;
	int Min;
	int Max;
	int * MaxPointer;

	__int64 SlideStop;

	void InternalUpdateInput();

public:

	virtual ~SliderMenuItem();

	void Initialize();

	int GetMax();

};
#endif