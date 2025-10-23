
#include "MenuItem.h"
#include "Game.h"

void MenuItem::Initialize(){
	nextMenuItem=NULL;
	TheMenuStyle=NULL;
	fY=0;
}
MenuItem::~MenuItem(){
}
int MenuItem::IsBorder(){return 0;}
int MenuItem::GetX(){return x;}
int MenuItem::GetY(){return y;}
int MenuItem::GetFY(){return fY;}
int MenuItem::IsEndBorder(){return 0;}
int MenuItem::IsOption(){return 0;}
void MenuItem::SetPos(int ix, int iy){x=ix;y=iy;}


void MenuItem::SetForcedYPos(int ifY){fY=ifY;}