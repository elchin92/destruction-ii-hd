#include "Game.h"
#include "Menu.h"
#include "Save.h"
#include "SaveableClass.h"
#include "InputEngine.h"
#include "NumberedStaticMenuItem.h"
#include "Debugger.h"
#include <stdio.h>
#include <time.h>
#include <fstream>

using namespace std;

#include "GameKeeper.h"
Save::Save(){
	Initialize();

}
Save::~Save(){

}

void Save::Blit(){
	TheGame->BlitOntoBack(&TheGame->g_pDDSBackground);
	LoadSaveMenu->BlitMenu();
}


void Save::InternalUpdateInput(){
	if(TrueDown(DIK_RETURN)){
#ifdef __D2PROTECTED__
		if(!TheGame->ISREGISTERED){
			Message("You can only load and save in the full version!",mcRed);
			return;
		
		}
#endif
				
			char Name[40];
			struct tm *newtime;
			time_t long_time;
			
			time( &long_time );                /* Get time as long integer. */
			newtime = localtime( &long_time ); /* Convert to local time. */
			
			sprintf(Name, "%.16s", asctime( newtime ));


			
			WriteData(((NumberedStaticMenuItem *)(*(LoadSaveMenu->pSelectedItem)))->GetID(),Name);
			SwitchState(ReturnState);
			return ;	
	}
	if(TrueDown(DIK_ESCAPE)){
		SwitchState(ReturnState);
		return;
	}
	LoadSaveMenu->UpdateInput();
}
void Save::Setup(){

	BuildMenu(TRUE);

	
}
const char * gtNames[3]= {"1 on 1", "Cup", "League"};
void Save::WriteData(int which, char * Name){

	DP2("Saving to slot",which);
	CreateDirectory("Save",0);

	//for(int pl=0;pl<TheGameKeeper->GetNumberOfPlayers();pl++);
	

	char FileName[41];
	sprintf(FileName,"Save\\Save%i.DSG",which);
	ofstream File(FileName, ios::binary);
	DP("SaveName");
	DP(FileName);
	// Modern C++: ios::binary replaces setmode(filebuf::binary)

	sprintf(FileName,"%s - %s",gtNames[TheGameKeeper->GetGameType()], Name );
	DP(FileName);

	BYTE NameLength = (BYTE)strlen(FileName);
	File.write(reinterpret_cast<char*>(&NameLength),1);
	File.write(FileName,NameLength);
	for(SaveableClass *sc=TheSaveableClasses; sc ; sc=sc->nextSaveableClass){
		sc->PreSave();
		sc->SaveClass(&File);
		sc->PostSave();
		
	}
	File.close();

}
void Save::SetSurfaces(){
	AddBitmapAndSurface("Save", &TheGame->g_pDDSBackground);
}