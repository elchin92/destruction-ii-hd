#ifndef __SaveableClass__ 
#define __SaveableClass__

class ofstream;
class ifstream;
class SaveableItem;

class SaveableClass
{
protected:

	void Initialize(char * iTopic);
	SaveableItem * TheSaveableItems;
	char * Topic;

	int CheckSum;


public:

	virtual ~SaveableClass();
	virtual void Reset();



	// ATTENTION: Once these two functions has been set in a
	// certain order you mustn't change their internal order!
	// you can add items anywhere in the list, before, in or after
	// but no item must come before an item which it was after
	// in a previous version!!

	// Further notice: call this function like; 
	// RegisterItem("NumberOfPlayers",&NumberOfPlayers, sizeof(NumberOfPlayers));
	// RegisterItem("TenElemArray",&teArray[0], sizeof(teArray[0])*10);
	void RegisterItem(char * ItemName, void * Data, int sizeofData);

	int LoadClass(ifstream * savefile, int PerformLoad);
	void SaveClass(ofstream * savefile);

	void ReportData(unsigned char Data);

	int TopicCheck(char * PossibleTopic);

	virtual void PreLoad(int PerformLoad);
	virtual void PostLoad(int PerformLoad);
	virtual void PreSave();
	virtual void PostSave();

	SaveableClass * nextSaveableClass;
};

extern SaveableClass * TheSaveableClasses;
#endif
