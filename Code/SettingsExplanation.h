#ifndef __SettingsExplanation__ 
#define __SettingsExplanation__

#include "SlideShowGameState.h"

class SettingsExplanation : public SlideShowGameState
{
private:


protected:
	void InternalUpdateInput();

public:

	SettingsExplanation();
	~SettingsExplanation();

	void Blit();
	void Setup();

	void SetSurfaces();

};

extern SettingsExplanation * TheSettingsExplanation;

#endif
