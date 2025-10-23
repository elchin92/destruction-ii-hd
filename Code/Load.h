#ifndef __Load__ 
#define __Load__

class Game;
class Menu;
#include "LoadSaveState.h"

class Load : public LoadSaveState
{
private:

	int LOADED;

protected:
	void InternalUpdateInput();

public:
	Load();
	~Load();
	void Blit();

	void Setup();
	int GetData(int which);
	void GetFileNames();
	void SetSurfaces();
};
#endif

