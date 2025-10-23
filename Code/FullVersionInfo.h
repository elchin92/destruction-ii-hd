#ifndef __FullVersionInfo__ 
#define __FullVersionInfo__

#include "ButtonGameState.h"

class InfoBox;

class FullVersionInfo : public ButtonGameState
{
private:
	__int64 LockStop;
	InfoBox * CountDownMsgBox;
protected:
	void InternalUpdateInput();
	void InternalBlit();
public:

	FullVersionInfo();
	~FullVersionInfo();

	void Setup();

	void SetSurfaces();
	void SetCountDown();
};

extern FullVersionInfo * TheFullVersionInfo;

#endif
