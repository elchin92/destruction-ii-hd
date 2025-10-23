#ifndef __SavableItem__ 
#define __SavableItem__



class ofstream;
class ifstream;

enum ReadResult{rSomeMissing, rAllOK};

class SaveableClass;

class SaveableItem
{
private:

	void * Data;
	int Bytes;
	char KeyName[100];

	ReadResult InternalRead(SaveableClass * MotherClass, ifstream * savefile, char * ReadKey, int PerformLoad, ReadResult r);

public:

	SaveableItem(char* iKeyName, void * iData, int Bytes);
	~SaveableItem();

	ReadResult Read(SaveableClass * MotherClass, ifstream * savefile, int PerformLoad, ReadResult=rAllOK);

	void Write(SaveableClass * MotherClass, ofstream * savefile);

	SaveableItem * nextSaveableItem;

};
#endif
