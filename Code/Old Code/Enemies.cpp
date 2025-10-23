#include "PLSetup.h"
#include "Game.h"

PLSetup::PLSetup(){}
PLSetup::~PLSetup(){}

void PLSetup::Blit(){

}
void PLSetup::Input(int Key){
	if(Key==VK_ESCAPE)
		SwitchState(SETTINGS);
}
void PLSetup::Setup(){

}
