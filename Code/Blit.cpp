#include "ddex4.h"
#include "Missile.h"
#include "Aviator.h"
#include <stdio.h>
#include <math.h>
#define HealthBarX 132
#define HealthBarY 408
#define HealthBarL 252
#define HealthBarH 21
#define TankWarnerX 51
#define TankWarnerY 580
#define TankWarnerW 11
#define TankWarnerH 12
#define	RadarX 300
#define RadarY 497
#define RadarW 85

#define PUPTHICK 16
const int PUP[5][2] = { {275,447},{299,447},{322,447},{347,447},{370,447}};

const int TextPos[6][2] = { {62,460},{62,500},{62,540},{160,470},{160,510},{160,550}};

// Blit.cpp - Ingame graphics is drawn here


void Game::BlitGUI(int player){

		// Blit Bars

	const int barheight=87,barwidth=11,barstart[2]={206,487},barspace[2]={29,8};
	__int64 PowerTime=apl->PowerTime();
	for(int Bars=0;Bars<3;Bars++){

		double Br=barheight;

		if(Bars==1&&apl->IsFast()){
			

			if(!apl->SpL()&&!apl->SpR()){
				Br*=(PowerTime+thisTickCount-apl->GetStop(IT_SPEED));
				Br/=PowerTime;
			}
			else
				Br=0;

		}
		
		if(Bars==2&&apl->IsShielded()){
			
			
			if(!apl->ShL()){
				Br*=(PowerTime+thisTickCount-apl->GetStop(IT_SHIELD));
				Br/=PowerTime;
			}
			else
				Br=0;

		}
		if(Bars==0&&apl->IsInvisible()){
			
			
			Br*=(PowerTime+thisTickCount-apl->GetStop(IT_INVIS));
			Br/=PowerTime;
			
			
		}
						
		lBarz[player][Bars]=Barz[player][Bars];
		Barz[player][Bars]=(int)Br;

		if(UPDATESCREEN||lBarz[player][Bars]!=Barz[player][Bars])
			BLITBAR[player][Bars]=3;

		if(BLITBAR[player][Bars]){

			BlitCoords(0,0,barwidth,barheight,barstart[0]+player*384+barspace[0]*Bars,barspace[1]*Bars+barstart[1], &g_pDDSEmptyBar);
			BlitCoords(barwidth*Bars,Barz[player][Bars],barwidth+barwidth*Bars,barheight,barstart[0]+player*384+barspace[0]*Bars,barspace[1]*Bars+barstart[1]+(int)Barz[player][Bars], &g_pDDSBars);

			BLITBAR[player][Bars]--;
		}
		
	}
		GUIPUPS[player][0]=apl->Stopped();
		GUIPUPS[player][1]=apl->DoubleDamage();
		GUIPUPS[player][2]=apl->Superman();
		GUIPUPS[player][3]=apl->Doubling();
		GUIPUPS[player][4]=apl->FiringFast();
	for(int i=0;i<5;i++){
		if(UPDATESCREEN||GUIPUPS[player][i]!=lGUIPUPS[player][i]){
			BLITPUPS[player][i]=3;	
		}
		lGUIPUPS[player][i]=GUIPUPS[player][i];
		if(BLITPUPS[player][i]){
			if(GUIPUPS[player][i]){
				rcRect.left = PUPTHICK*i;
				rcRect.top = 0;
				rcRect.right= rcRect.left+PUPTHICK;
				rcRect.bottom = rcRect.top+PUPTHICK;
				BlitSetRc(PUP[i][0]+384*player,PUP[i][1], &g_pDDSGUIPowerUps);	
			}else{
				BlankSurfaceArea(PUP[i][0]+384*player,PUP[i][1],PUPTHICK,PUPTHICK, &TheGame->g_pDDSBack);
			}
			BLITPUPS[player][i]--;
		}
	}


	if( UPDATESCREEN || BlitLife[player]>0 || OtherHealth[player] != apl->GetHealth() ){
		// Blit Life

		double L=HealthBarL;
		L*=apl->GetHealth();
		L/=apl->GetMaxHealth();
		
		rcRect.left = (int)L;
		rcRect.top = 0;
		rcRect.right= HealthBarL;
		rcRect.bottom = HealthBarH;

		if(rcRect.left!=HealthBarL){
			BlitSetRcCC(HealthBarX+player*384+rcRect.left,HealthBarY, &g_pDDSEmptyLife);
		}

		rcRect.left=0;
		rcRect.right = (int)L;	


		if(rcRect.right>0)
			BlitSetRc(HealthBarX+player*384,HealthBarY, &g_pDDSLife);	
		
		if(OtherHealth[player]==apl->GetHealth()&&!UPDATESCREEN){
			BlitLife[player]--;
		}else{
			OtherHealth[player]=apl->GetHealth();
			BlitLife[player]=3;
		}
	}
	// Bot Warner, Tank Warner, Enemy Warner, enemywarner

	if(apl->TWN()){

		ZeroMemory(&ddbltfx, sizeof(ddbltfx));
		ddbltfx.dwSize = sizeof(ddbltfx);
		
		int DistCopy=200;
		
		for(Mortal * PresentMortal=TheMortals;PresentMortal;PresentMortal=PresentMortal->nextMortal){
			if(PresentMortal->GetTeam()==2){
			
				// Pythagoras
				long DistMax=(long)(sqrt((WorldSize*32*11-32)*(WorldSize*32*11-32)*2));
				long Xdist = apl->GetActualXPos()-PresentMortal->GetActualXPos();
				long Ydist = apl->GetActualYPos()-PresentMortal->GetActualYPos();
				Xdist*=Xdist;
				Ydist*=Ydist;
				double Dist=sqrt(Xdist+Ydist);
				Dist/=DistMax;
				Dist*=32;
				if(Dist>16)
					Dist=0;
				else
					Dist+=32;
				if(Dist&&DistCopy>Dist)
					DistCopy=(int)Dist;
			}
			
		}
		
		if(DistCopy==200)
			DistCopy=0;

		ddbltfx.dwFillColor=DistCopy;
		rcRect.left = TankWarnerX+384*player;
		rcRect.top = TankWarnerY;
		rcRect.right = rcRect.left+TankWarnerW;
		rcRect.bottom = rcRect.top+TankWarnerH;
		
		
		g_pDDSBack->Blt(&rcRect, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

		rcRect.left = 0;
		rcRect.top = 0;
		rcRect.right = TankWarnerW;
		rcRect.bottom = TankWarnerH;
		
		BlitSetRc(TankWarnerX+player*384,TankWarnerY, &g_pDDSTankWarner);
	}
	DBHI(61352);

	// Radar

	if(apl->RDR()){

		BlankArea(RadarX+384*player, RadarY, RadarW+1, RadarW+1);
		int PLDOT;


		for(Mortal * aMortal=TheDeadMortals;aMortal;aMortal=aMortal->nextDeadMortal){
			PLDOT=aMortal->GetMType()==M_PL||aMortal->GetMType()==M_CL;
			if(PLDOT&&apl->PRDR()||!PLDOT&&apl->URDR()&&!aMortal->IsInvisible())
				BlitRadarDot(aMortal, 0);
		}
		for(Mortal * aMortal=TheMortals;aMortal;aMortal=aMortal->nextMortal){
			PLDOT=aMortal->GetMType()==M_PL||aMortal->GetMType()==M_CL;
			if(PLDOT&&apl->PRDR()||!PLDOT&&apl->URDR()&&!aMortal->IsInvisible())
				BlitRadarDot(aMortal, 1);
		}

	}


}



void Game::BlitMap(int player){
	// Blit map
	DBHI(52374);		
	
	if(ThePlayers[player]->IsDark()){
		DP("IsDark()!");
		TheGame->BlankArea(32+384*player,32,11*32,11*32);
		return;
	}

	for(int y=Ystart,x;y<12+Ystart;y++)
		for(x=Xstart;x<12+Xstart;x++){
			
		
			// Blit ground, blit floor
			rcRect.left = 0;
			rcRect.top = 32*(WorldMap[x][y][1]<0);
			rcRect.right = rcRect.left+32;
			rcRect.bottom = rcRect.top+32;	
			BGOSNC(x*32,y*32,&g_pDDSLevels);
			
			// Blit Destroyed Walls
			if(WorldMap[x][y][0]>0 && WorldMap[x][y][0]<=16&&
				WorldMap[x][y][1]<=0){
				rcRect.left = WorldMap[x][y][0]*32;
				rcRect.top = 32;
				rcRect.right = rcRect.left+32;
				rcRect.bottom = rcRect.top+32;
				BGOS(x*32,y*32,&g_pDDSLevels);
			}	


		}

	TheInputEngine->UpdateInputState(hWnd, TRUE);
	DBHI(52335);

	for(Mortal * PresentMortal=TheDeadMortals;PresentMortal;PresentMortal=PresentMortal->nextDeadMortal){
		if(!PresentMortal->IsPlayer())
			PresentMortal->BlitDead(player);
	}
	DBHI(63457);

	// Blit Charges
	if(TheAnimations)
		TheAnimations->Blit(AN_GROUND);

	DBHI(47844);

	for(int Y=Ystart,X;Y<12+Ystart;Y++)
		for(X=Xstart;X<12+Xstart;X++){

			// Any Items? Blit Items
			if(WorldMap[X][Y][2]){
				int item=WorldMap[X][Y][2]-1;
				if(item==IT_TMINE2)
					item=IT_TMINE1;
				if(item==IT_MINE1||item==IT_MINE2)
					item=IT_MINE;
				if(item==IT_MINE&&apl->MWN())
					item++;
				rcRect.left = (item%8)*32;
				rcRect.top = (item/8)*32;
				rcRect.right = (item%8+1)*32;
				rcRect.bottom = (item/8+1)*32;

				BGOS(X*32, Y*32, &g_pDDSItem);							

			}

			BOOL BLITIT = FALSE;
			// Blit walls, Blit base & Blit doors!
			if(WorldMap[X][Y][0]>0 && WorldMap[X][Y][0]<=16 &&
			   !(WorldMap[X][Y][1]<=0) ){
				rcRect.left = WorldMap[X][Y][0]*32;
				rcRect.top = 0;
				BLITIT=TRUE;
			}else if(WorldMap[X][Y][0]>=41 && WorldMap[X][Y][0]<=46){
				int sprite=(WorldMap[X][Y][0]-41);
				rcRect.left = 544+(sprite%3)*32;
				rcRect.top = 0+(sprite/3)*32;

				BLITIT=TRUE;	
			}else if(WorldMap[X][Y][0]==47||WorldMap[X][Y][0]==48){

				BOOL VERTICAL = WorldMap[X][Y][0]==48;

				rcRect.left = 640;
				rcRect.top = 32*VERTICAL;

				int Ticks=TheBases[apl->GetX()>32*11]->GetTicks(!VERTICAL);

				rcRect.left+=32*Ticks;

				BLITIT=TRUE;

			}

			rcRect.right = rcRect.left+32;
			rcRect.bottom = rcRect.top+32;

			if(BLITIT)
				BGOS(32*X,32*Y, &g_pDDSLevels);
			
		}
	TheInputEngine->UpdateInputState(hWnd, TRUE);
				
	DBHI(45352);	
	if(TheLaserBeams){
		Beacon(88453521);
		TheLaserBeams->Blit(player);
	}
	DBHI(45353);
	if(TheTazerBeams){
		Beacon(88453521);
		TheTazerBeams->Blit(player);
	}
	DBHI(45354);
	for(shot * PresentShot=TheShots;PresentShot;PresentShot=PresentShot->nextshot){
		Beacon(88453541);
		if(!PresentShot->IsInvisible())
			PresentShot->Blit(player);
	}	
	DBHI(62345);
	for(Mortal * aMortal=TheMortals;aMortal;aMortal=aMortal->nextMortal){
		if( aMortal != apl )
			aMortal->Blit(player);
	}
	DBHI(15264);



	if(apl->IsAlive()){
		apl->Blit(player);
	}
	if(TheFireFlames)
		TheFireFlames->Blit();

	DBHI(50923);
	
	if(TheAnimations){
		TheAnimations->Blit(AN_EXP);

	}

	for(Aviator * PresentAviator=TheAviators;PresentAviator;PresentAviator=PresentAviator->nextAviator){
		PresentAviator->Blit(player);
	}
	DBHI(23474);
	for(Grenade * PresentGrenade=TheGrenades;PresentGrenade;PresentGrenade=PresentGrenade->nextGrenade){
		PresentGrenade->Blit(player);
	}

	if(TheAnimations){
		TheAnimations->Blit(AN_EXP);
	}
	for(int pl=0;pl<2;pl++){
		if(ThePlayers[pl]->TheMissile)
			ThePlayers[pl]->TheMissile->Blit(player);
		if(ThePlayers[pl]->ThePlane)
			ThePlayers[pl]->ThePlane->Blit(player);

	}
	DBHI(52343);
/*	if(theUfo->IsActive()){
		theUfo->Blit(player);
		theUfo->BlitBeam(player);
	}
*/		

	
	
	DBHI(56122);
	// Display Figures
	TheInputEngine->UpdateInputState(hWnd, TRUE);
	char Strings[6][8];

	if(ActiveWeapon==Bazooka)
		sprintf(Strings[0],"%i/%i",apl->GetBullets(B_APRKT),apl->GetBullets(B_ATRKT));
	else
		sprintf(Strings[0],"%i",apl->GetBullets((*ActiveWeapon).GetAT(0)));
	sprintf(Strings[1],"%i",apl->GetCharges());
	sprintf(Strings[2],"%i",apl->GetBullets(GR_GRENADE));
	sprintf(Strings[3],"%i",apl->GetLives());
	sprintf(Strings[4],"%i",apl->GetBullets(B_MINE));	
	sprintf(Strings[5],"%i",apl->GetBullets(B_TMINE));

	for(int xxx=0;xxx<6;xxx++){
		if(UPDATETEXT[player][xxx])
			UPDATETEXT[player][xxx]--;
		if(UPDATESCREEN||strcmp(Strings[xxx],Strs[player][xxx]))
			UPDATETEXT[player][xxx]=3;
		strcpy(Strs[player][xxx],Strings[xxx]);
	}

	if(StopTime){
		if(UPDATETIMELIMIT)
			UPDATETIMELIMIT--;
		int TimeLeft = (int)((StopTime-thisTickCount)/1000);
		if(UPDATESCREEN||TimeLeft!=lTimeLeft){
			UPDATETIMELIMIT=3;
			if(TimeLeft==-1&&ThePlay->GetPlayStatus()==PLAYING){
				DP("Calling SuddenOrDraw()");
				SuddenOrDraw();
			}
		
		}
		lTimeLeft=TimeLeft;	
	}

	const int start[2]={50,462},space[2]={115,34};	
	for(int a=0;a<2;a++)
		for(int b=0;b<3;b++){
			if(UPDATETEXT[player][a*3+b]){
				// Primary
				BlankArea(TextPos[a*3+b][0]+384*player, TextPos[a*3+b][1], 54-17*a, 25);
				
			}
		}


	char TimeStr[20];

	if(UPDATETIMELIMIT){
		BlankArea(400-20,10,40,15);
		if(SUDDENDEATH)
			strcpy(TimeStr,"Sudden Death!");
		else
			strcpy(TimeStr,Dword2TimeStr(1000*pos(lTimeLeft)));
	}
	
	// Print messages, display figures
	if (g_pDDSBack->GetDC(&hdc) == DD_OK)
	{
			
		SetBkColor(hdc, TRANSPARENT);
		SetTextColor(hdc, cgs->GetBlue());
		GetClientRect(hWnd, &rcRect);
		
		SelectObject(hdc, g_hFont);	

		for(int a=0;a<2;a++)
			for(int b=0;b<3;b++){
				if(UPDATETEXT[player][a*3+b]){
		
					GetTextExtentPoint(hdc, Strings[3*a+b], lstrlen(Strings[3*a+b]), &size);
					TextOut(hdc, TextPos[a*3+b][0]+384*player, TextPos[a*3+b][1], Strings[3*a+b], lstrlen(Strings[3*a+b]));
				}
			}
		if(UPDATETIMELIMIT){
			SetTextAlign(hdc, TA_CENTER);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, cgs->GetBlue());
			SelectObject(hdc, g_hFontSmall);

			GetTextExtentPoint(hdc, TimeStr, lstrlen(TimeStr), &size);
			TextOut(hdc, 400, 10, TimeStr, lstrlen(TimeStr));
		}

	DBHI(62382);
	
		/*
	for(int y=0,x;y<12;y++)
		for(x=0;x<12;x++){

			if (CheckSquare(x+Xstart,y+Ystart)){	
				int NumberOfMortals=0;
				for(Mortal * temp = TheMortalQuadrants[x+Xstart][y+Ystart]; temp;temp=temp->nextQuadMortal)
					NumberOfMortals++;
				IntText(x*32+14+32+384*player,y*32+12+32,NumberOfMortals);

			}
		}
*/
	}

    g_pDDSBack->ReleaseDC(hdc);

	DBHI(73456);
}	

void Game::BlitIt(){

	Beacon(-2);
	DBHI(49575);
	PAINTED=TRUE;
		
	if(MONEYMAZE)
		UPDATESCREEN=3;

	// Blit Background
	if(UPDATESCREEN){
		BlitOntoBack(&g_pDDSBackground);
		UPDATESCREEN--;
	}
			
	// Blitting maps
	for(player=0;player!=2;player++){

		//BlankArea(32+384*player,32,352,352);		
		
		apl=ThePlayers[player];
		ActiveWeapon=apl->GetWeapon();
		DBHI(35473);
		SetCamera();

		BlitGUI(player);
		DBHI(23563);
		
		BlitMap(player);
	}
	
	PrintText();
	

}
void Game::PrintText(){
	if(MONEYMAZE){
		int Blink=(int)((thisTickCount-StopTime))/500;
		if(Blink%2){

			
			// Print messages
			if (g_pDDSBack->GetDC(&hdc) == DD_OK)
			{
				
				SelectObject(hdc,g_hFontHuge);
				GetClientRect(hWnd, &rcRect);
				
				SetBkMode(hdc, TRANSPARENT);
				SetTextColor(hdc, cgs->GetBlack());
				
				GetTextExtentPoint(hdc, "Money Maze", lstrlen("Money Maze"), &size);
				TextOut(hdc, 130, 460, "Money Maze", lstrlen("Money Maze"));
				
				SetTextColor(hdc, cgs->ContrastAdjust(255, 255, 0));
				
				GetTextExtentPoint(hdc, "Money Maze", lstrlen("Money Maze"), &size);
				TextOut(hdc, 120, 450, "Money Maze", lstrlen("Money Maze"));
				
			}
			g_pDDSBack->ReleaseDC(hdc);		
		}
		
		
	}
	


}



void Game::BlitBack(){

	



	if(COPYBACK){

		// Print messages
		if (g_pDDSBack->GetDC(&hdc) == DD_OK){
			DP("Print Game Paused");
			SelectObject(hdc,g_hFontBigPlus);
			//GetClientRect(hWnd, &rcRect);
				
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, cgs->GetBlack());
			SetTextAlign(hdc, TA_CENTER);
			
			Text(16*11+32+3,200+3,"Game");
			Text(16*11+32+3+384,200+3,"Paused");

			SetTextColor(hdc, cgs->GetBlue());

			Text(16*11+32-3,200-3,"Game");
			Text(16*11+32-3+384,200-3,"Paused");

		}g_pDDSBack->ReleaseDC(hdc);
	
		BlitOnto(&g_pDDSBackground, &g_pDDSBack);

		COPYBACK=FALSE;
	}else
		BlitOntoBack(&g_pDDSBackground);


}


void Game::BlitRadarDot(Mortal * PresentMortal, int ALIVE){


	if(!ALIVE&&!PresentMortal->BLITDEAD)
		return;

	if(PresentMortal->IsInvisible()&&apl!=PresentMortal)
		return;

	double XP=RadarW,YP=RadarW;

	XP*=PresentMortal->GetActualXPos();
	YP*=PresentMortal->GetActualYPos();
	XP/=(WorldSize*11*32);	
	YP/=(WorldSize*11*32);				
	
	rcRect.left = RadarX+384*player+(int)XP;
	rcRect.top = RadarY+(int)YP;
	rcRect.right = rcRect.left+2;
	rcRect.bottom = rcRect.top+2;

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = RadarColors[PresentMortal->GetMType()][!ALIVE];
	g_pDDSBack->Blt(&rcRect,0,0, DDBLT_COLORFILL| DDBLT_WAIT, &ddbltfx);
		
}

// Animation Blitting

Animation * Game::CreateStaticAnimation(int X, int Y, int BlitX, int BlitY){
	return NewAnimation(1,0,AN_GROUND,X,Y,BlitX,BlitY,FALSE);
}

Animation * Game::NewAnimation(int iFrames, int iPlayTime, int iAnimType,int x, int y, int BlitX, int BlitY, int Reverse){

	DP("NewAnim()");
	Animation * NewAnim = new Animation(iFrames,iPlayTime,iAnimType,x,y,BlitX,BlitY,Reverse);
	
	NewAnim->nextAnimation=TheAnimations;
	TheAnimations=NewAnim;
	DP("NewAnim() end");
	return NewAnim;
}

void Game::KillDeadAnims(){
	if(TheAnimations)
		TheAnimations->KillDead();
}

void Game::PreparePlayerSprites(){

	LoadBitmap(&g_pDDSPlayer, TempPlayersBitmap);
	//CreateBlankSurface(32*8,32*6,&g_pDDSPlayer);

	for(int pl=0;pl<2;pl++){
		ThePlayers[pl]->LoadCharacterSprites();
		ThePlayers[pl]->LoadWeaponSprites();
		ThePlayers[pl]->LoadCorpseSprites();

	}


}

void Game::InternalSetupSurfaces(){

	UPDATESCREEN=3;
	PreparePlayerSprites();

	// Load correct level

	TheGame->LoadBitmapToSurface(&TheGame->g_pDDSTemporary,LevelBitmap);

	rcRect.left = 0;
	rcRect.top = 64*WorldType;
	rcRect.right = 800;
	rcRect.bottom = rcRect.top+64;	

	TheGame->BlitOntoSetRc(0,0, &TheGame->g_pDDSLevels ,&TheGame->g_pDDSTemporary);

	TheGame->g_pDDSTemporary->Release();
	TheGame->g_pDDSTemporary=NULL;
}