
#include "TextMenuItem.h"
#include "Game.h"

void TextMenuItem::Initialize(){
	nextTextMenuItem=NULL;
}
TextMenuItem::~TextMenuItem(){
	if(nextTextMenuItem)
		delete nextTextMenuItem;
}
int TextMenuItem::IsBorder(){return 0;}
int TextMenuItem::IsEndBorder(){return 0;}
int TextMenuItem::IsOption(){return 0;}
