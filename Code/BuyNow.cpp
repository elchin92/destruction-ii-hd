// BuyNow.cpp: implementation of the BuyNow class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "InputEngine.h"
#include "BuyNow.h"
#include "Menu.h"
#include "ButtonMenuItem.h"

void OnOrderOnline(){

	TheGame->GoToURL("http://www.destruction.nu/buynow.html");
}
void OnClickHere(){
	TheGame->GoToURL("http://www.regnow.com/privacy.html");
}

void OnSignUpPayPal(){
	TheGame->GoToURL("https://www.paypal.com/cgi-bin/webscr?cmd=_international-register");
}
void OnViewMailOrderForm(){
	TheGame->GoToURL("Documentation//MailOrderForm.pdf");
	
}
void OnOrderViaXtra(){
	TheGame->GoToURL("http://www.destruction.nu/buynow.html");
}

BuyNow * TheBuyNow;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BuyNow::BuyNow(){
	Initialize();

	ButtonMenu = new Menu();

	ActiveMenuItem * b2 = ButtonMenu->AddMenuItem(new ButtonMenuItem(113,365,181,38, 10, &TheGame->g_pDDSLife, OnOrderOnline));
	ActiveMenuItem * b3 = ButtonMenu->AddMenuItem(new ButtonMenuItem(438,337,320,38, 10, &TheGame->g_pDDSLife, OnSignUpPayPal));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(93,550,222,38, 10, &TheGame->g_pDDSLife, OnViewMailOrderForm));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(416,550,361,38, 10, &TheGame->g_pDDSLife, OnOrderViaXtra));

	ButtonMenu->SetNeighbours(2);

	ActiveMenuItem * b1 = ButtonMenu->AddMenuItem(new ButtonMenuItem(228,255,61,15, 0, &TheGame->g_pDDSLife, OnClickHere));

	b1->SetRightNeighbour(b3);
	b1->SetDownNeighbour(b2);
	b2->SetUpNeighbour(b1);
	b3->SetUpNeighbour(b1);

	BackgroundSurface=&TheGame->g_pDDSBackground;
}

BuyNow::~BuyNow(){

}



void BuyNow::InternalBlit(){
}

void BuyNow::Setup(){

}

void BuyNow::SetSurfaces(){
	AddBitmapAndSurface("BuyNow", &TheGame->g_pDDSBackground);
	AddBitmapAndSurface("BuyNowS", &TheGame->g_pDDSLife);
}

void BuyNow::InternalUpdateInput(){
	if(TrueDown(DIK_ESCAPE)||TrueDown(DIK_SPACE)){
		ReturnToMotherState();
			return;
	}
	ButtonMenu->UpdateInput();     

}
