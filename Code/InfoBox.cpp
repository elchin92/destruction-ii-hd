// InfoBox.cpp: implementation of the InfoBox class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "InfoBox.h"
#include <stdio.h>
#include "Debugger.h"

char tstr[100]; // temp string

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


InfoBox::InfoBox(int ix, int iy, int iw, int ih, 
				 HFONT * iTextFont, COLORREF iTextColor, int iTextAlign){
	x=ix;y=iy;w=iw;h=ih;
	TextFont=iTextFont;
	TextColor=iTextColor;
	TextAlign=iTextAlign;
	txAdj=0;
	tyAdj=0;
}
int InfoBox::GetW(){return w;}
int InfoBox::GetH(){return h;}
int InfoBox::GetX(){return x+xAdj;}
int InfoBox::GetY(){return y+yAdj;}
int InfoBox::GetTextX(){return x+xAdj+txAdj;}
int InfoBox::GetTextY(){return y+yAdj+tyAdj;}


InfoBox::~InfoBox()
{

}
void InfoBox::SetInfoBoxTextAlign(int iTextAlign){
	TextAlign=iTextAlign;
}
void InfoBox::SetInfoBoxTextColor(COLORREF iTextColor){
	TextColor=iTextColor;
}
void InfoBox::SetInfoBoxTextFont(HFONT * iTextFont){
	TextFont=iTextFont;
}
void InfoBox::SetTemporaryInfoBoxTextAlign(int iTextAlign){
	SetTextAlign(TheGame->hdc, iTextAlign);
}
void InfoBox::SetTemporaryInfoBoxTextColor(COLORREF iTextColor){
	SetTextColor(TheGame->hdc, cgs->ContrastAdjust(iTextColor));
}
void InfoBox::SetTemporaryInfoBoxTextFont(HFONT * iTextFont){
	SelectObject(TheGame->hdc, (*iTextFont));	
}
void InfoBox::CloseBox(){
	TheGame->g_pDDSBack->ReleaseDC(TheGame->hdc);
}
int InfoBox::OpenBox(){
	if (TheGame->g_pDDSBack->GetDC(&TheGame->hdc) == DD_OK)
	{
			
		SetBkMode(TheGame->hdc, TRANSPARENT);
		SetTextColor(TheGame->hdc, cgs->ContrastAdjust(TextColor));
		SetTextAlign(TheGame->hdc, TextAlign);
		SelectObject(TheGame->hdc, (*TextFont));			

		return 1;
	}
	return 0;
}
int InfoBox::BlankAndPrepareBox(int ixAdj, int iyAdj){

	xAdj=ixAdj;
	yAdj=iyAdj;

	TextColor=TextColor;
	TextFont=TextFont;

	TheGame->BlankSurfaceArea(x+xAdj,y+yAdj,w,h, &TheGame->g_pDDSBack);	

	if(TextAlign==TA_CENTER)
		txAdj=w/2;

	return OpenBox();
}
void InfoBox::Print(int xRel, int yRel, int text){
	sprintf(IntText,"%i",text);
	Print(xRel,yRel,IntText);

}

void InfoBox::Print(int xRel, int yRel, char * text){
	TheGame->Text(x+xAdj+xRel+txAdj,y+yAdj+yRel+tyAdj,text);
	DP("Printed!");
}