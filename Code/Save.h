#ifndef __Save__ 
#define __Save__


class Menu;
class Game;

#include "LoadSaveState.h"
class Save : public LoadSaveState
{
private:


protected:
	void InternalUpdateInput();

public:
	Save();
	~Save();
	void Blit();

	void Setup();
	void WriteData(int which, char * Name);
	void SetSurfaces();
};
#endif

