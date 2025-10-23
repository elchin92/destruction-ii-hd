// SaveableItem.cpp: implementation of the SaveableItem class.
//
//////////////////////////////////////////////////////////////////////

#include <fstream>
#include <string.h>
#include "SaveableClass.h"
#include "SaveableItem.h"
#include "definitions.h"
#include "TimeKeeper.h"
#include "Debugger.h"

using namespace std; // C++20: for ios::binary, endl, etc.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SaveableItem::SaveableItem(const char* iKeyName, void * iData, int iBytes) // C++20: const char*
{
	// No Beacons in here!!
	nextSaveableItem=NULL;
	strcpy(KeyName,iKeyName);
	Data=iData;
	Bytes=iBytes;
}

SaveableItem::~SaveableItem()
{
	SAFE_DELETE(nextSaveableItem);
}


ReadResult SaveableItem::Read(SaveableClass * MotherClass, ifstream * savefile, int PerformLoad, ReadResult r){
	DP2("Reading an item....", PerformLoad);
	char iKey[100];
	BYTE KeyLength;

	(*savefile).read(reinterpret_cast<char*>(&KeyLength), 1); // C++20: BYTE* -> char* cast
	(*savefile).read(iKey,KeyLength);
	iKey[KeyLength]='\0';

	DP(iKey);

	return InternalRead(MotherClass, savefile, iKey, PerformLoad,r );

}

ReadResult SaveableItem::InternalRead(SaveableClass * MotherClass, ifstream * savefile, char * ReadKey, int PerformLoad, ReadResult r){
	DP("InternalRead()....");

	DP(ReadKey);
	Beacon((*ReadKey));
	DP(KeyName);
	Beacon((*KeyName));
	if(strcmp(ReadKey,KeyName)){
		DP("Wrong key, checking with next....");

		// Wrong key
		// Inv: This saveable item isn't part of the savefile
		//      because we are reading an old save game from
		//		an earlier version

		if(nextSaveableItem){
			return nextSaveableItem->InternalRead(MotherClass, savefile, ReadKey, PerformLoad, rSomeMissing);
		}
		return rSomeMissing;

	}

	DP("Reading the item....");
	BYTE * temp = new BYTE[Bytes];
	// Correct key, read Data

	(*savefile).read(reinterpret_cast<char*>(temp), Bytes); // C++20: BYTE* -> char* cast

	for(int i=0;i<Bytes;i++){
		BYTE * p = ((BYTE *)Data+i);
		if(PerformLoad)
			(*p)=temp[i];
		MotherClass->ReportData(temp[i]);
	}	

	delete[] temp;
	if(nextSaveableItem){
		return nextSaveableItem->Read(MotherClass, savefile, PerformLoad, r);		
	}
	return r;
		
}
void SaveableItem::Write(SaveableClass * MotherClass, ofstream * savefile){
	DP("Saving an item...");
	BYTE Length = (BYTE)strlen(KeyName);
	(*savefile).write(reinterpret_cast<const char*>(&Length), 1); // C++20: BYTE* -> char* cast
	(*savefile).write(KeyName,Length);
	DP(KeyName);
	for(int i=0;i<Bytes;i++){
		(*savefile).write(reinterpret_cast<const char*>(((BYTE *)Data+i)), 1); // C++20: BYTE* -> char* cast
		MotherClass->ReportData((*((BYTE *)Data+i)));
	}
	if(nextSaveableItem)
		nextSaveableItem->Write(MotherClass, savefile);

}

