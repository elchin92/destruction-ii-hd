#include "Load.h"
#include "Game.h"
#include "Menu.h"
#include "SoundEngine.h"
#include "InputEngine.h"
#include "NumberedStaticMenuItem.h"
#include "SaveableClass.h"
#include <stdio.h>
#include <io.h>
#include <fstream>

using namespace std;

#include "Debugger.h"
#include "GameKeeper.h"
#include "Start.h"

Load::Load(){
	Initialize();

}
Load::~Load(){

}

void Load::Blit(){
	TheGame->BlitOntoBack(&TheGame->g_pDDSBackground);
	LoadSaveMenu->BlitMenu();
}
void Load::InternalUpdateInput(){
	if(TrueDown(DIK_RETURN)){

#ifdef __D2PROTECTED__
		if(!TheGame->ISREGISTERED){
			Message("You can only load and save in the full version!",mcRed);
			return;
		
		}
#endif
			if(GetData(((NumberedStaticMenuItem *)(*(LoadSaveMenu->pSelectedItem)) )->GetID())){
				LOADED=TRUE;
				TheGame->GAMEACTIVE=TRUE;
				TheStart->SetResumeSurfaces();
				TheGame->InterpretSettings();
				TheGameKeeper->SortPlayerArray();
				SwitchState(GETREADY);			
			}
			return;		
	}
	if(TrueDown(DIK_ESCAPE)){
		SwitchState(START);
		return;
	}
	LoadSaveMenu->UpdateInput();
}
void Load::Setup(){
	BuildMenu(FALSE);
	LOADED=FALSE;

}

int Load::GetData(int which){
	
	char FileName[41];

	sprintf(FileName,"Save\\Save%i.DSG",which);
	ifstream File(FileName, ios::binary);
	// Modern C++: ios::binary replaces ios::nocreate | filebuf::openprot | setmode

	char TempName[41];
	BYTE KeyLength=0;
	File.read(reinterpret_cast<char*>(&KeyLength), 1);
	File.read(TempName,KeyLength);
	TempName[KeyLength]='\0';

	char tTopic[100];

	int err=FALSE;
	// Test load of game&gamekeeper
	for(SaveableClass *sc=TheSaveableClasses; sc ; sc=sc->nextSaveableClass){
		File.read(&KeyLength, 1);
		File.read(tTopic,KeyLength);
		tTopic[KeyLength]='\0';
		DP2("KeyLength",KeyLength);
		DP("tTopic");
		DP(tTopic);
		if(sc->TopicCheck(tTopic)){
			DP("Topic checked ok!");
			sc->PreLoad(0);
			err=!sc->LoadClass(&File,FALSE);
			sc->PostLoad(0);
		}else{ err=TRUE;}

		if(err){
			DP("Topic check & load Failed!");
			Message("Corrupt save file!",mcRed);
			File.close();
			return FALSE;
		}
	}
	File.close();

	DP("Test load OK!");

	File.open(FileName, ios::binary);
	File.read(reinterpret_cast<char*>(&KeyLength), 1);
	File.read(TempName,KeyLength);
	TempName[KeyLength]='\0';

	int loaded=0;
	for(sc=TheSaveableClasses; sc ; sc=sc->nextSaveableClass){
		File.read(&KeyLength, 1);
		File.read(tTopic,KeyLength);
		tTopic[KeyLength]='\0';
		if(File.fail()){
			// Old save game, data missing
			break;
		}

		if(sc->TopicCheck(tTopic)){
			sc->PreLoad(1);
			sc->Reset();
			err = ! sc->LoadClass(&File,TRUE);
			sc->PostLoad(1);
		}else{ err=TRUE;}

		if(err){
			// Shouldn't be able to happen
			Beacon(66601);
			DP("Impossible: Load failed second run");
			TheGame->Quit();
			File.close();
			return FALSE;
		}
		loaded++;
	}
	DP2("Loaded SaveableClasses #",loaded);
	File.close();

	return TRUE;
}



void Load::SetSurfaces(){
	AddBitmapAndSurface("Load", &TheGame->g_pDDSBackground);
}