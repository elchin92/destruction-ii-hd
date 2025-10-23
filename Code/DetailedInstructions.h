#ifndef __DetailedInstructions__ 
#define __DetailedInstructions__

#include "SlideShowGameState.h"

class DetailedInstructions : public SlideShowGameState
{
private:


protected:
	void InternalUpdateInput();

public:

	DetailedInstructions();
	~DetailedInstructions();

	void Blit();
	void Setup();

	void SetSurfaces();

};

extern DetailedInstructions * TheDetailedInstructions;

#endif
