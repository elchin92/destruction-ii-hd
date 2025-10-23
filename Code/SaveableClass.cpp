// SaveableClass.cpp: implementation of the SaveableClass class.
//
//////////////////////////////////////////////////////////////////////

#include "TimeKeeper.h"
#include "SaveableItem.h"
#include "SaveableClass.h"
#include "definitions.h"
#include "Debugger.h"
#include <fstream>
using std::ifstream;
using std::ofstream;

SaveableClass * TheSaveableClasses;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void SaveableClass::Reset(){
	DP("SaveableClass::Reset() Called ");
}

SaveableClass::~SaveableClass()
{
	SAFE_DELETE(TheSaveableItems);
}
void SaveableClass::RegisterItem(const char * ItemName, void * Data, int sizeofData ){
	// No Beacons in here!!
	SaveableItem * newItem = new SaveableItem(ItemName, Data, sizeofData);
	AddToLinkedList(TheSaveableItems, nextSaveableItem, newItem);
}


int SaveableClass::LoadClass(ifstream * savefile, int PerformLoad){

	DP2("Loading a class",PerformLoad);
	DP(Topic);

	// Inv :: Topic loaded and checked to match
	savefile->read((char *)&CheckSum,sizeof(CheckSum));
	srand(CheckSum);
	TheSaveableItems->Read(this, savefile, PerformLoad);
	int tCheckSum;
	savefile->read((char *)&tCheckSum,sizeof(tCheckSum));

	srand((int)TheTimeKeeper->GetRealTime());


	if(tCheckSum==CheckSum){
		DP("The following class succeeded in loading");
		DP(Topic);
		return TRUE;
	}
	DP("The following class failed in loading");
	DP(Topic);
	return FALSE;

}
void SaveableClass::SaveClass(ofstream * savefile){
	DP("Saving a class...");
	DP(Topic);
	BYTE NameLength = (BYTE)strlen(Topic);
	savefile->write(&NameLength,1);
	savefile->write(Topic,NameLength);
	CheckSum=rand();
	savefile->write((char *)&CheckSum,sizeof(CheckSum));
	srand(CheckSum);
	TheSaveableItems->Write(this, savefile);
	savefile->write((char *)&CheckSum,sizeof(CheckSum));
	//savefile->write(&CheckSum,1);

	srand((int)TheTimeKeeper->GetRealTime());


}
void SaveableClass::Initialize(const char * iTopic){
	Topic=const_cast<char*>(iTopic);
	TheSaveableItems=NULL;
	nextSaveableClass=NULL;
}

void SaveableClass::ReportData(BYTE Data){
	CheckSum+=((int)Data+2)*rand();
}

int SaveableClass::TopicCheck(const char * PossibleTopic){
	return !strcmp(PossibleTopic, Topic);
}

void SaveableClass::PreLoad(int PerformLoad){
	
}
void SaveableClass::PostLoad(int PerformLoad){

}
void SaveableClass::PreSave(){
	
}
void SaveableClass::PostSave(){

}