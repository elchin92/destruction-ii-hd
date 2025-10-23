#ifndef __SavableItem__
#define __SavableItem__

#include <fstream>

// C++20: ifstream/ofstream cannot be forward declared (they are template specializations)
// Use std:: prefix consistently with SaveableClass.h

enum ReadResult{rSomeMissing, rAllOK};

class SaveableClass;

class SaveableItem
{
private:

	void * Data;
	int Bytes;
	char KeyName[100];

	ReadResult InternalRead(SaveableClass * MotherClass, std::ifstream * savefile, char * ReadKey, int PerformLoad, ReadResult r);

public:

	SaveableItem(const char* iKeyName, void * iData, int Bytes); // C++20: const char*
	~SaveableItem();

	ReadResult Read(SaveableClass * MotherClass, std::ifstream * savefile, int PerformLoad, ReadResult=rAllOK);

	void Write(SaveableClass * MotherClass, std::ofstream * savefile);

	SaveableItem * nextSaveableItem;

};
#endif
