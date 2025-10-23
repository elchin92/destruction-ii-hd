// LoadSaveState.cpp: implementation of the LoadSaveState class.
//
//////////////////////////////////////////////////////////////////////

#include "LoadSaveState.h"
#include "Menu.h"
#include <fstream>
#include <stdio.h>
#include "definitions.h"
#include "debugger.h"
#include "InfoBox.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


LoadSaveState::~LoadSaveState()
{
	SAFE_DELETE(LoadSaveMenu);
}
void LoadSaveState::Initialize(){
	GameState::Initialize();
	MsgFont=CreateFont(34,
		0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");

	UserMsgBox = new InfoBox(400,300-17, 0, 0, &MsgFont, mcWhite,TA_CENTER);
}

void LoadSaveState::BuildMenu(int ExtraEmpty){

	
	SAFE_DELETE(LoadSaveMenu);
	LoadSaveMenu=new Menu();

	char FileName[41];
	int LowestFree=0;
	int Free[5]={TRUE,TRUE,TRUE,TRUE,TRUE};
	int items=0;
	for(int i=0;i<MAXSAVEGAMES;i++){
		sprintf(FileName,"Save\\Save%i.DSG",i);
		ifstream File(FileName, ios::binary);
		// Modern C++: ios::binary replaces ios::nocreate | filebuf::openprot | setmode

		BYTE NameLength=0;
		File.read( reinterpret_cast<char*>(&NameLength), 1 );
		if(	NameLength ){
			File.read( FileName, NameLength );
			FileName[NameLength]='\0';
			LoadSaveMenu->AddNumberedStaticItem(FileName,i);
			items++;
			if(i<5)
				Free[i]=FALSE;
			if(LowestFree==i)
				LowestFree++;
		}
		File.close();
	}
	int x=LowestFree;
	for(;items<5;items++){
		if(x==5){
			Beacon(666142);
			DP("Dammit!!");
		}
		LoadSaveMenu->AddNumberedStaticItem("Empty",x);
		ExtraEmpty=FALSE;
		while(!Free[x]&&x<5)x++;
	}
	if(items>=5&&ExtraEmpty&&items<MAXSAVEGAMES){
		LoadSaveMenu->AddNumberedStaticItem("Empty",LowestFree);	
	}
	

	DP("Build Load save menu");
	LoadSaveMenu->CreatePredefinedMenu(62,167, 742, 546,pLoadSave);
	DP("Build complete");

}