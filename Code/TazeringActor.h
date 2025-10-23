#include "Actor.h"
#ifndef __TAZERINGACTOR__
#define __TAZERINGACTOR__


class TazeringActor : public virtual Actor
{
protected:

	void PlayTazerSound();
	void UpdateTazerSound(int Hit);

public:

	SoundSource * TazerSound;
	void StopTazerSound();

	TazeringActor();
	virtual ~TazeringActor();

};

#endif 