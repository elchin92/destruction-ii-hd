#ifndef __BuyNow__ 
#define __BuyNow__

#include "ButtonGameState.h"

class BuyNow : public ButtonGameState
{
private:



protected:
	void InternalUpdateInput();
	void InternalBlit();
public:

	BuyNow();
	~BuyNow();


	void Setup();

	void SetSurfaces();



};

extern BuyNow * TheBuyNow;

#endif
