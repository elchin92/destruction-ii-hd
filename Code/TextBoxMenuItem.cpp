// TextBoxMenuItem.cpp: implementation of the TextBoxMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "MenuStyle.h"
#include "GeneralConstants.h"
#include "Menu.h"
#include "InfoBox.h"
#include "InputEngine.h"
#include "DIK_CODES.h"
#include "TextBoxMenuItem.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TextBoxMenuItem::TextBoxMenuItem(MenuStyle * iTheMenuStyle, char * iistr)
{
	ActiveMenuItem::Initialize();
	TheMenuStyle=iTheMenuStyle;
	istr=iistr;
	chars=strlen(istr);
	RegularStyle=TRUE;
	InputBox=NULL;
	InputFont=NULL;
	lSelected=FALSE;

}

TextBoxMenuItem::TextBoxMenuItem(int x, int y, int w, int h, int fs, int fw, char * iistr)
{
	ActiveMenuItem::Initialize();
	istr=iistr;
	chars=strlen(istr);

	lSelected=FALSE;

	InputFont=CreateFont(32,
		0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");

	InputBox = new InfoBox(155, 225, 239, 32, &InputFont, mcWhite,TA_CENTER);

	RegularStyle=FALSE;
}
TextBoxMenuItem::~TextBoxMenuItem()
{
	SAFE_DELETE(InputBox);
	DeleteObject(InputFont);
}
void TextBoxMenuItem::InternalBlit(int Selected){

	if(Selected&&!lSelected)
		EditStart=TheGame->thisTickCount;
	lSelected=Selected;

	char c = ' ';
	if(Selected&&(TheGame->thisTickCount-EditStart)%800<400)c='|';
    char op[MNC+2];sprintf(op, "%s%c",istr,c);

	if(RegularStyle){
		TheMenuStyle->SetStyle(Selected);
		TheGame->Text(x,y,op);
	}else{
		InputBox->BlankAndPrepareBox(0, 0);
		InputBox->Print(0,0,op);
		InputBox->CloseBox();
	}


}

void TextBoxMenuItem::SetStr(char * iistr){
	istr=iistr;
	chars=strlen(istr);
}
void TextBoxMenuItem::InternalUpdateInput(){

	if(lSelected){
		CapsLock=Down(DIK_LSHIFT)||Down(DIK_RSHIFT);
		if(TrueDown(DIK_BACKSPACE)){
			EraseChar();
		}
		for(int dik=0;dik<MaxKeys;dik++){
			if(CharNameAvailable(dik)&& TrueDown(dik)){
				AddChar(dik);
			}
		}
	}
}

void TextBoxMenuItem::AddChar(int dik){

	if(chars<MNC){
		int c= DIK2ASCII(dik);

		if(c>=65&&c<=90&&!CapsLock)
			c+=32;
		else if(c>=49&&c<=57&&CapsLock)
			c-=16;
		istr[chars++]=c;
		istr[chars]='\0';
	}else{cgs->Message("You may use max 12 characters",mcRed);}
	
}

void TextBoxMenuItem::EraseChar(){

	if(chars){
		istr[--chars]='\0';
	}
}
