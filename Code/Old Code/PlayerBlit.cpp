

void Player::BlitPlayer(){

	int Rotated=Dir%2;
	int TempDest[2]={32*2,32*5};

	// Blank temporary space
	TheGame->BlankSurfaceArea(TempDest[0],TempDest[1]-32,48,64,g_pDDSBlitSource);


	// Blit character to temporary space
	TheGame->rcRect.left =   (4*Team+Rotated)*BlitDimension[0];
	TheGame->rcRect.top =    GetTicks()*BlitDimension[1];
	TheGame->rcRect.right =  TheGame->rcRect.left+BlitDimension[0];
	TheGame->rcRect.bottom = TheGame->rcRect.top +BlitDimension[1];

	TheGame->BlitOntoSetRc(TempDest[0],TempDest[1], g_pDDSBlitSource,g_pDDSBlitSource);

	// Blit aiming, weapon and flame

	TheGame->UserMessage("What the f***", "Unused code");
	if(IsPlayer()&&((Player *)this)->IsShooting()){

		Player * apl = ((Player *)this);

		// Blit Aim
		TheGame->rcRect.left  = BlitDimension[0] * (Team*4+2+Rotated);
		TheGame->rcRect.top   = BlitDimension[1] * apl->GetFireType();  
		TheGame->rcRect.right =	TheGame->rcRect.left+BlitDimension[0];
		TheGame->rcRect.bottom=	TheGame->rcRect.top+BlitDimension[1];

		TheGame->BlitOntoSetRc(TempDest[0],TempDest[1], g_pDDSBlitSource,g_pDDSBlitSource);

		// Blit Weapon
		TheGame->rcRect.left  = BlitDimension[0] * (Team*4+Rotated);
		TheGame->rcRect.top   = BlitDimension[1] * 5;  
		TheGame->rcRect.right =	TheGame->rcRect.left+BlitDimension[0];
		TheGame->rcRect.bottom=	TheGame->rcRect.top+BlitDimension[1];


		// We need to blit the weapon twice if we have dual fire
		for(int side=-1;side<=apl->DualFire();side+=2){

			int X=0,Y=-16;

			if(Rotated){
				X=12,Y=-12;	
			}

			if(apl->UsingSingleHandedWeapon()){
				// Pistol grip

				if(apl->DualFire()){
					LengthAdjust(&X,&Y,Rotated,13,TRUE);
					SideAdjust(&X,&Y,Rotated,(10-Rotated)*side,TRUE);
				}else{
					LengthAdjust(&X,&Y,Rotated,7,TRUE);
				}

			}else if(apl->ShoulderFire()){

				// Bazooka Grip
				if(apl->GetWeapon()==TheGame->ATShot){
					// AT Shot
					LengthAdjust(&X,&Y,Rotated,-6,TRUE);
				}else{
					// Bazooka
					LengthAdjust(&X,&Y,Rotated,-10,TRUE);
				}
			}else{
				// Rifle Grip
				LengthAdjust(&X,&Y,Rotated,2,TRUE);
				SideAdjust(&X,&Y,Rotated,1,TRUE);
			}



			TheGame->BlitOntoSetRc(TempDest[0]+X,TempDest[1]+Y, g_pDDSBlitSource,g_pDDSBlitSource);

			//TheGame->BGOS(X+TheGame->WeaponAdjust[GetRealDir()][0]+Pos[0],Y+TheGame->WeaponAdjust[GetRealDir()][1]+Pos[1],&TheGame->g_pDDSAim);
		/*
			if(int Flame=apl->GetFireFlame()){
			// Blit Flame
			}*/
		}

	}

	int xw = 64-16*Rotated-32*(Dir==0||Dir==4), yw = 64- 16*Rotated-32*(Dir==2||Dir==6);
	TheGame->BlankSurfaceArea(32*6,32*4,xw,yw,g_pDDSBlitSource);

	TheGame->rcRect.left  = TempDest[0];//BlitStart[0]+BlitXAdjust();
	TheGame->rcRect.top   = TempDest[1]-32+16*Rotated;//BlitStart[1]+BlitYAdjust();  
	TheGame->rcRect.right =	TheGame->rcRect.left+32+16*Rotated;
	TheGame->rcRect.bottom=	TempDest[1]+32;

	TheGame->BlitOntoSetRc(32*6,32*4,g_pDDSBlitSource,g_pDDSBlitSource, Get4Dir());

	TheGame->rcRect.left  = 32*6;
	TheGame->rcRect.top   = 32*4;  
	TheGame->rcRect.right =	TheGame->rcRect.left+xw;
	TheGame->rcRect.bottom=	TheGame->rcRect.top +yw;

	TheGame->BGOS(Pos[0]+PlayerBlitAdjust[Dir][0],Pos[1]+PlayerBlitAdjust[Dir][1],g_pDDSBlitSource);
}