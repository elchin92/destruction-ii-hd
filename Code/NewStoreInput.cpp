#include "NewStore.h"
#include "InputEngine.h"
#include "Game.h"
#include "Player.h"

void NewStore::InternalUpdateInput(){

	if(DoubleShop){
		SwitchPlayer();
		StoreInput();
		SwitchPlayer();
		StoreInput();
	}else{
		StoreInput();
	}

}

void NewStore::RegisterKeys(){
	if(DoubleShop){
		for(int pl=0;pl<2;pl++){
			SwitchPlayer();
			esc[cpl]=apl->GetKey(10);
			action[cpl]=apl->GetKey(4);
			left[cpl]=apl->GetKey(2);
			up[cpl]=apl->GetKey(0);
			down[cpl]=apl->GetKey(1);
			right[cpl]=apl->GetKey(3);

		}
	}else{
		for(int s=0;s<2;s++){
			esc[s]=DIK_ESCAPE;
			action[s]=DIK_RETURN;
			left[s]=DIK_LEFT;
			up[s]=DIK_UP;
			down[s]=DIK_DOWN;
			right[s]=DIK_RIGHT;
		}
	}
}

void NewStore::StoreInput(){


	if(TrueDown(esc[cpl])){

		if(Section[cpl]==0&&Choice[cpl]<8){
			// continue, let DIK_ESCAPE catch us (perhaps)

		}else{

			if(Choice[cpl]>=8){
				Choice[cpl]=Choice[cpl]-2;
			}else{
				LeaveStore();		
			}
			return;
		}

	}

	if(TrueDown(DIK_ESCAPE)){
	
		SwitchState(START, TRUE);
		return;
	
	}




	if(TrueDown(action[cpl])){
		(this->*(ButtonFunctions[cpl][Choice[cpl]]))();
		return;
	}

	if(Choice[cpl]<8){
		if(TrueDown(left[cpl])){
			if(Choice[cpl]%2)
				Choice[cpl]--;
			return;
		}
		if(TrueDown(up[cpl])){
			if(Choice[cpl]>=2)
				Choice[cpl]-=2;
			return;
		}
		if(TrueDown(right[cpl])){
			if(!(Choice[cpl]%2))
				Choice[cpl]++;
			return;
		}
		if(TrueDown(down[cpl])){
			if(Choice[cpl]<=5)
				Choice[cpl]+=2;
			return;					
		}
	}

}

void NewStore::LeaveStore(){
	// LeaveStore, or leave detailedinfo
	Selection[cpl]=Section[cpl]-1;
	SetSection(0);
	ButtonFunctions[cpl][2]=&NewStore::OnEnterShop; // C++20: & required
	ButtonFunctions[cpl][3]=&NewStore::OnEnterShop;
}