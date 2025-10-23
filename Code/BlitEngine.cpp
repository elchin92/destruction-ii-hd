#include "ddex4.h"
#include <stdio.h>
// #include <ddraw.h>  // Заменено на SDL2_DirectDrawCompat.h через /FI (forced include)
#include "ddutil.h"

#define DestructionSetPixelSingleColor()	doffset = dX + dsDesc.lPitch*dY;soffset = sX + ssDesc.lPitch*sY;readPixel = *((BYTE* )(((BYTE*)(ssDesc.lpSurface))+soffset));if(readPixel)if(BlitColor)*((BYTE* )(((BYTE*)(dsDesc.lpSurface))+doffset))=BlitColor;else*((BYTE* )(((BYTE*)(dsDesc.lpSurface))+doffset))=readPixel;
#define DestructionSetPixel()				doffset = dX + dsDesc.lPitch*dY;soffset = sX + ssDesc.lPitch*sY;readPixel = *((BYTE* )(((BYTE*)(ssDesc.lpSurface))+soffset));if(readPixel)*((BYTE* )(((BYTE*)(dsDesc.lpSurface))+doffset))=readPixel;
#define DestructionSetPixelNoCC()			doffset = dX + dsDesc.lPitch*dY;soffset = sX + ssDesc.lPitch*sY;readPixel = *((BYTE* )(((BYTE*)(ssDesc.lpSurface))+soffset));*((BYTE* )(((BYTE*)(dsDesc.lpSurface))+doffset))=readPixel;


#define yLoop for(int sY=0,dY,dX;sY<height;sY++)
#define xLoop for(int sX=0;sX<width;sX++)
// BlitEngine.cpp - The GFX Engine

// Raw Blits

void Game::BGOSL(int x,int y, int life, LPDIRECTDRAWSURFACE7 * BlitSource, BYTE BlitColor){
	

	int xth=rcRect.right-rcRect.left, yth=rcRect.bottom-rcRect.top;

	tcRect.left = FX;
	tcRect.top = FY;
	tcRect.right=FX+xth;
	tcRect.bottom=FY+yth;

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = 0;
	g_pDDSSprites->Blt(&tcRect, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	BlitOntoSetRc(FX,FY, &g_pDDSSprites,BlitSource, BlitColor);

	rcRect=tcRect;

	if (g_pDDSSprites->GetDC(&hdc) == DD_OK)
	{
		SelectObject(hdc,lifePen);
		SelectObject(hdc,lifeBrush);
		Rectangle(hdc,FX+xth-16,FY+yth-3,FX+xth,FY+yth);
		SelectObject(hdc,greenPen);
		Rectangle(hdc,FX+xth-15,FY+yth-1,FX+xth-15+life,FY+yth-2);
	}
	g_pDDSSprites->ReleaseDC(hdc);
	
	BGOS(x,y, &g_pDDSSprites );
	
}


void Game::BGOSNC(int x,int y, LPDIRECTDRAWSURFACE7 * BlitSource){
	ColorKey=DDBLTFAST_NOCOLORKEY;
	BGOS(x, y, BlitSource);
	ColorKey=DDBLTFAST_SRCCOLORKEY;

}
void Game::BGO(int x, int y, BYTE BlitColor){
	BGOS(x,y,&g_pDDSSprites, BlitColor);
}

void Game::BGOS(int x,int y, LPDIRECTDRAWSURFACE7 * BlitSource, BYTE BlitColor){

	DP2("BGOS(), side",player);
	x-=apl->GetCameraX();
	y-=apl->GetCameraY();

	sWidth  = rcRect.right - rcRect.left;
	sHeight = rcRect.bottom - rcRect.top;


	if( 1 - sWidth  <= x &&
		351+sWidth  >= x &&
		1 - sHeight <= y &&
		351+sHeight >= y){


		OverRun=x+sWidth-352;
		if(OverRun>0){

			rcRect.right-=OverRun;

		}

		OverRun=y+sHeight - 352;
		if(OverRun>0){

			rcRect.bottom-=OverRun;

		}

		if(x<0){
			rcRect.left-=x;
			x=0;
		}

		if(y<0){
			rcRect.top-=y;

			y=0;
		}

		BlitSetRc(32+player*384+x, 32+y, BlitSource, BlitColor);
		
	}
}

void Game::BlitOnto(LPDIRECTDRAWSURFACE7 * BlitDestination, LPDIRECTDRAWSURFACE7 * BlitSource){

	tcRect.left = 0;
	tcRect.top = 0;
	tcRect.right = 800;
	tcRect.bottom = 600;

	while (TRUE)
	{

		hRet = (*BlitDestination)->BltFast(0, 0, (*BlitSource),
			&tcRect, DDBLTFAST_NOCOLORKEY);

		if (hRet == DD_OK)
			break;
		if (hRet == DDERR_SURFACELOST){
			hRet = RestoreAll();
			if (hRet != DD_OK)
				return;
		}
		if (hRet != DDERR_WASSTILLDRAWING)
			return;
	}
}
void Game::BlitOntoBack(LPDIRECTDRAWSURFACE7 * BlitSource){
	BlitOnto(&g_pDDSBack,BlitSource);
}
void Game::BlitCoords(int l, int t, int r, int b , int x, int y, LPDIRECTDRAWSURFACE7 * BlitSource){

		tcRect.left = l;
		tcRect.top = t;
		tcRect.right = r;
		tcRect.bottom = b;


		while (TRUE)
		{
			hRet = g_pDDSBack->BltFast(x, y, (*BlitSource),
				&tcRect, DDBLTFAST_NOCOLORKEY);
			if (hRet == DD_OK)
				break;
			if (hRet == DDERR_SURFACELOST){
				hRet = RestoreAll();
				if (hRet != DD_OK)
					return;
			}
			if (hRet != DDERR_WASSTILLDRAWING)
				return;
		}
}

void Game::BlitOntoSetRc(int x, int y, LPDIRECTDRAWSURFACE7 * Destination, LPDIRECTDRAWSURFACE7 * BlitSource, BYTE BlitColor, DWORD rot){
	
	if(rot == 0 && !BlitColor){

		while (TRUE)
		{
			hRet = (*Destination)->BltFast(x, y, (*BlitSource),
				&rcRect, ColorKey);
			if (hRet == DD_OK)
				break;
			if (hRet == DDERR_SURFACELOST){
				hRet = RestoreAll();
				if (hRet != DD_OK)
					return;
			}
			if (hRet != DDERR_WASSTILLDRAWING)
				return;
		}

	}else{
		if(BlitColor){
			CopyRoutineSingleColor(x,y,Destination,BlitSource,rot, BlitColor);	
		}else if(ColorKey==DDBLTFAST_NOCOLORKEY){
			CopyRoutineNoCC(x,y,Destination,BlitSource,rot);
		}else{
			CopyRoutine(x,y,Destination,BlitSource,rot);
		}
	}
}

void Game::CopyRoutineSingleColor(int x, int y, LPDIRECTDRAWSURFACE7 * Destination, LPDIRECTDRAWSURFACE7 * BlitSource,int rot, BYTE BlitColor){
	int width=rcRect.right-rcRect.left;
	int height = rcRect.bottom-rcRect.top;

	RECT destRect={x,y,
				x + width,
				y + height};
	if(rot%2){
		destRect.right=destRect.left+height;
		destRect.bottom=destRect.top+width;
	}



	DDSURFACEDESC2	dsDesc,ssDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.dwSize = sizeof( dsDesc);
	ZeroMemory(&ssDesc, sizeof(ssDesc));
	ssDesc.dwSize = sizeof( ssDesc);
	HRESULT dr=(*Destination)->Lock(&destRect,&dsDesc, DDLOCK_WAIT | DDLOCK_WRITEONLY  | DDLOCK_SURFACEMEMORYPTR ,NULL);
	HRESULT sr=(*BlitSource) ->Lock(&rcRect  ,&ssDesc, DDLOCK_WAIT | DDLOCK_READONLY |DDLOCK_SURFACEMEMORYPTR ,NULL );
	if( dr == DD_OK &&
		sr == DD_OK ){
		int soffset,doffset;
		BYTE readPixel;

		if(rot==0){
			yLoop{
				dY=sY;
				xLoop{
					dX=sX;
					DestructionSetPixelSingleColor();

				}
			}
		}
		else if(rot==1){
			yLoop{
				dX=height-sY-1;
				xLoop{
					dY=sX;
					DestructionSetPixelSingleColor();
				}
			}
		}
		else if(rot==2){
			yLoop{
				dY=height-sY-1;
				xLoop{
					dX=width-sX-1;
					DestructionSetPixelSingleColor();
				}
			}
		}
		else if(rot==3){
			yLoop{
				dX=sY;
				xLoop{
					dY=width-sX-1;
					DestructionSetPixelSingleColor();
				}
			}

		}				
	}
	(*Destination)->Unlock(&destRect);
	(*BlitSource)->Unlock(&rcRect);
}

void Game::CopyRoutine(int x, int y, LPDIRECTDRAWSURFACE7 * Destination, LPDIRECTDRAWSURFACE7 * BlitSource,int rot){
	int width=rcRect.right-rcRect.left;
	int height = rcRect.bottom-rcRect.top;

	RECT destRect={x,y,
				x + width,
				y + height};
	if(rot%2){
		destRect.right=destRect.left+height;
		destRect.bottom=destRect.top+width;
	}



	DDSURFACEDESC2	dsDesc,ssDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.dwSize = sizeof( dsDesc);
	ZeroMemory(&ssDesc, sizeof(ssDesc));
	ssDesc.dwSize = sizeof( ssDesc);
	HRESULT dr=(*Destination)->Lock(&destRect,&dsDesc, DDLOCK_WAIT | DDLOCK_WRITEONLY  | DDLOCK_SURFACEMEMORYPTR ,NULL);
	HRESULT sr=(*BlitSource) ->Lock(&rcRect  ,&ssDesc, DDLOCK_WAIT | DDLOCK_READONLY |DDLOCK_SURFACEMEMORYPTR ,NULL );
	if( dr == DD_OK &&
		sr == DD_OK ){
		int soffset,doffset;
		BYTE readPixel;

		if(rot==0){
			yLoop{
				dY=sY;
				xLoop{
					dX=sX;
					DestructionSetPixel();

				}
			}
		}
		else if(rot==1){
			yLoop{
				dX=height-sY-1;
				xLoop{
					dY=sX;
					DestructionSetPixel();
				}
			}
		}
		else if(rot==2){
			yLoop{
				dY=height-sY-1;
				xLoop{
					dX=width-sX-1;
					DestructionSetPixel();
				}
			}
		}
		else if(rot==3){
			yLoop{
				dX=sY;
				xLoop{
					dY=width-sX-1;
					DestructionSetPixel();
				}
			}

		}				
	}
	(*Destination)->Unlock(&destRect);
	(*BlitSource)->Unlock(&rcRect);
}

void Game::CopyRoutineNoCC(int x, int y, LPDIRECTDRAWSURFACE7 * Destination, LPDIRECTDRAWSURFACE7 * BlitSource,int rot){
	int width=rcRect.right-rcRect.left;
	int height = rcRect.bottom-rcRect.top;

	RECT destRect={x,y,
				x + width,
				y + height};
	if(rot%2){
		destRect.right=destRect.left+height;
		destRect.bottom=destRect.top+width;
	}

	DDSURFACEDESC2	dsDesc,ssDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.dwSize = sizeof( dsDesc);
	ZeroMemory(&ssDesc, sizeof(ssDesc));
	ssDesc.dwSize = sizeof( ssDesc);
	HRESULT dr=(*Destination)->Lock(&destRect,&dsDesc, DDLOCK_WAIT | DDLOCK_WRITEONLY  | DDLOCK_SURFACEMEMORYPTR ,NULL);
	HRESULT sr=(*BlitSource) ->Lock(&rcRect  ,&ssDesc, DDLOCK_WAIT | DDLOCK_READONLY |DDLOCK_SURFACEMEMORYPTR ,NULL );
	if( dr == DD_OK &&
		sr == DD_OK ){
		int soffset,doffset;
		BYTE readPixel;

		if(rot==0){
			yLoop{
				dY=sY;
				xLoop{
					dX=sX;
					DestructionSetPixelNoCC();

				}
			}
		}
		else if(rot==1){
			yLoop{
				dX=height-sY-1;
				xLoop{
					dY=sX;
					DestructionSetPixelNoCC();
				}
			}
		}
		else if(rot==2){
			yLoop{
				dY=height-sY-1;
				xLoop{
					dX=width-sX-1;
					DestructionSetPixelNoCC();
				}
			}
		}
		else if(rot==3){
			yLoop{
				dX=sY;
				xLoop{
					dY=width-sX-1;
					DestructionSetPixelNoCC();
				}
			}

		}				
	}
	(*Destination)->Unlock(&destRect);
	(*BlitSource)->Unlock(&rcRect);
}

void Game::BlitSetRcCC(int x, int y, LPDIRECTDRAWSURFACE7 * BlitSource, DWORD rot){
	ColorKey=DDBLTFAST_NOCOLORKEY;
	BlitOntoSetRc(x, y, &g_pDDSBack, BlitSource, 0, rot);
	ColorKey=DDBLTFAST_SRCCOLORKEY;
}
void Game::BlitSetRc(int x, int y, LPDIRECTDRAWSURFACE7 * BlitSource, BYTE BlitColor, DWORD rot){
	BlitOntoSetRc(x, y, &g_pDDSBack, BlitSource, BlitColor, rot);
}


// Beams

void Game::BlitBeamC(int X, int Y, int EndX, int EndY, int Color,int where){
	int Pos[2],EndPos[2];

	Pos[0]=X;
	Pos[1]=Y;
	EndPos[0]=EndX;
	EndPos[1]=EndY;
	BlitBeam(&Pos[0],&EndPos[0],Color,where);
}

void Game::BlitBeam(int * Pos, int * EndPos, int Color,int where){


		Beacon(8855560);
	int ScreenPos[2],xy, bonus, bonus2;
	int Coords[2][2], Step[2], S[4][3];

	ScreenPos[0]=ThePlayers[where]->GetCameraX();
	ScreenPos[1]=ThePlayers[where]->GetCameraY();


	Step[0]=(EndPos[0]-Pos[0]);
	Step[1]=(EndPos[1]-Pos[1]);

	int pointer=0, GOTEND = FALSE,GOTSTART=FALSE;
		Beacon(8855561);
	Coords[pointer][0]=Pos[0]-ScreenPos[0];
	Coords[pointer][1]=Pos[1]-ScreenPos[1];
	
	if(Coords[pointer][0]>=0&&Coords[pointer][0]<352&&
		Coords[pointer][1]>=0&&Coords[pointer][1]<352){
		GOTSTART=TRUE;
		pointer++;
	}
		
		Beacon(8855562);
	Coords[pointer][0]=EndPos[0]-ScreenPos[0];
	Coords[pointer][1]=EndPos[1]-ScreenPos[1];
	
	if(Coords[pointer][0]>=0&&Coords[pointer][0]<352&&
		Coords[pointer][1]>=0&&Coords[pointer][1]<352){
		pointer++;
		GOTEND=TRUE;
	}
		
		Beacon(8855563);
	for(int side=0;side<4&&pointer<2;side++){

		xy=side%2;
		bonus=(side/2)*351;

		S[side][0]=-1;
		S[side][1]=bonus;
		S[side][2]=ScreenPos[xy]+S[side][0]-Pos[xy];

		if(Step[!xy]){
			S[side][0]=(Pos[xy]-ScreenPos[xy])+(Step[xy]*(ScreenPos[!xy]+bonus-Pos[!xy]))/Step[!xy];
		}else{
			if(Pos[xy]==ScreenPos[xy]+bonus){
				S[side][0]=0;
			}

		}

	}
		Beacon(8855566);

	if(pointer==1){

				Beacon(8855567);
		int PossibleSides[2];

		if(!GOTEND&&Step[0]>=0 || !GOTSTART&&Step[0]<=0)
			PossibleSides[0]=3;
		else
			PossibleSides[0]=1;

		if(!GOTEND&&Step[1]>=0 || !GOTSTART&&Step[1]<=0)
			PossibleSides[1]=2;
		else
			PossibleSides[1]=0;

		for(int p=0;p<2 && pointer<2;p++){
			xy=PossibleSides[p]%2;
			bonus2=PossibleSides[p]==1||PossibleSides[p]==2;
			if(S[PossibleSides[p]][0]>=0+bonus2&&
			   S[PossibleSides[p]][0]<=350+bonus2){
					Coords[pointer][xy]=S[PossibleSides[p]][0];
					Coords[pointer][!xy]=S[PossibleSides[p]][1];
				
					pointer++;			
			}
		}

	}else{
				Beacon(8855568);
		for(int side=0;side<4&&pointer<2;side++){

			xy=side%2;


			bonus2=side==1||side==2;
			if(S[side][0]>=0+bonus2&&S[side][0]<=350+bonus2 && // Hit side
			   ((ScreenPos[!xy]>=Pos[!xy])==(Step[!xy]>=0)) && // Right Sign
			   abs(ScreenPos[!xy]-Pos[!xy])<=abs(Step[!xy]) ){ // Reach
					Coords[pointer][xy]=S[side][0];
					Coords[pointer][!xy]=S[side][1];
				
					pointer++;			
			}
/*

			if(S[side][0]>=0&&S[side][0]<=352&&S[side][2]>0&&S[side][2]<Step[xy]){

					Coords[pointer][xy]=S[side][0];
					Coords[pointer][!xy]=S[side][1];

					pointer++;

			}*/
		}

	} 

		

	if(pointer!=2){
				Beacon(8855569);
		return;
	}


	for(int ab=0;ab<2;ab++){
		Coords[ab][0]+=32+384*where;
		Coords[ab][1]+=32;
	}
		Beacon(8855570);
	HDC hdc;
	if (g_pDDSBack->GetDC(&hdc) == DD_OK)
	{
				Beacon(8855571);
				DP2("Color",Color);
		SelectObject(hdc,laserPens[Color]);

		DP2("Coords[0][0]",Coords[0][0]);
		DP2("Coords[0][1]",Coords[0][1]);
		DP2("Coords[1][0]",Coords[1][0]);
		DP2("Coords[1][1]",Coords[1][1]);
		MoveToEx(hdc,Coords[0][0],Coords[0][1],0);
		LineTo(hdc,Coords[1][0],Coords[1][1]);


	}			
	g_pDDSBack->ReleaseDC(hdc);
	
}


// Screen Control



void Game::BlankScreen(){

	tcRect.left = 0;
	tcRect.top = 0;
	tcRect.right = 800;
	tcRect.bottom = 600;

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = 0;
	g_pDDSBack->Blt(&tcRect, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);	
}


void Game::BlankArea(int l, int t, int w, int h){

	BlankSurfaceArea(l,t,w,h,&g_pDDSBack);
}
void Game::BlankSurfaceArea(int l, int t, int w, int h, LPDIRECTDRAWSURFACE7 * Surface){

	RECT bsaRect={l,t,l+w,t+h};
	//tcRect.left = l;
	//tcRect.top = t;
	//tcRect.right = l+w;
	//tcRect.bottom = t+h;

	BlankSurfaceArea(&bsaRect, Surface);

}
void Game::BlankSurfaceArea(RECT * bRect, LPDIRECTDRAWSURFACE7 * Surface){	

	DP("BlankSurfaceArea()");

	DP2("bRect.left",bRect->left);
	DP2("bRect.top",bRect->top);
	DP2("bRect.right",bRect->right);
	DP2("bRect.bottom",bRect->bottom);


	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = 0;
	(*Surface)->Blt(bRect, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

}
void Game::BlankArea(RECT * iRect){
	BlankSurfaceArea(iRect,&g_pDDSBack);
}



// Surface Handling



void Game::SetSurfaces(){
	Beacon(470);
	for(int k=0;k<NumberOfGameStates;k++){
		Beacon(5600+k);
		GameStates[k]->SetSurfaces();
	}
	Beacon(471);
	BOOL ADD;
	for(int j=0, Pointer=0;j<NumberOfGameStates;j++)
		for(int i=0; i<GameStates[j]->GetNumberOfSurfaces();i++){
			ADD=TRUE;
			for(int x=0;x<Pointer;x++)
				if(AllSurfaces[x]==GameStates[j]->GetSurface(i))
					ADD=FALSE;

			if(ADD){
				AllSurfaces[Pointer]=GameStates[j]->GetSurface(i);
				Pointer++;
				if(Pointer>=TotalNumberOfSurfaces)
					ErrorReport(101);
			}
		}
	Beacon(472);
}


void Game::ReleaseAllSurfaces() {

	for(int i=0; i<Pointer;i++)
		if( AllSurfaces[i] != 0)
		{
			(*AllSurfaces[i])->Release();
			(*AllSurfaces[i])= 0;
		}
	
}

// Menu Text

void Game::MenuText(int x, int y, LPCSTR mText, BOOL SELECTED){
	if(SELECTED){
		SetTextColor(hdc,cgs->ContrastAdjust(SelectedColor));
		SetBkColor(hdc, SelectedBGColor);
		SetBkMode(hdc,OPAQUE);
	}
	Text(x,y,mText);
	if(SELECTED){
		SetTextColor(hdc,cgs->ContrastAdjust(UnSelectedColor));
		SetBkMode(hdc,TRANSPARENT);		
	}
}
void Game::Text(int x, int y, LPCSTR Text){
	GetTextExtentPoint(hdc,  Text , lstrlen( Text ), &size);
	TextOut(hdc, x, y,  Text , lstrlen( Text ));
}
void Game::IntText(int x, int y, int number){
	sprintf(Message4, "%i", number);
	Text(x,y,Message4);
}




void Game::SetCamera(){
		// Ge fan i det h�r ( Draw correct map )
	apl->UpdateCamera();
	Xstart=apl->GetCameraX()/32;
	Ystart=apl->GetCameraY()/32;
}


void Game::LoadBitmapToSurface(LPDIRECTDRAWSURFACE7 * Surface, const char * Bitmap){
	printf("[DEBUG] LoadBitmapToSurface() START, Surface=%p, Bitmap=%s\n",
	       Surface, Bitmap ? Bitmap : "(null)"); fflush(stdout);

	printf("[DEBUG] Checking if *Surface is not NULL\n"); fflush(stdout);
	// FIX: Check for uninitialized memory (0xCDCDCDCD debug pattern)
	// Only call Release() if pointer looks valid (not NULL and not uninitialized)
	uintptr_t surfaceValue = (uintptr_t)(*Surface);
	if(surfaceValue != 0 &&
	   surfaceValue != (uintptr_t)0xCDCDCDCDCDCDCDCD &&
	   surfaceValue != (uintptr_t)0xCDCDCDCD) {
		printf("[DEBUG] *Surface=%p looks valid, calling Release()\n", *Surface); fflush(stdout);
		(*Surface)->Release();
		printf("[DEBUG] Release() OK\n"); fflush(stdout);
	} else {
		printf("[DEBUG] *Surface=%p looks uninitialized, skipping Release()\n", *Surface); fflush(stdout);
	}

	printf("[DEBUG] About to call DDLoadBitmap(g_pDD=%p, Bitmap=%s)\n",
	       g_pDD, Bitmap ? Bitmap : "(null)"); fflush(stdout);
	(*Surface) = DDLoadBitmap(g_pDD, Bitmap, 0, 0);
	printf("[DEBUG] DDLoadBitmap() returned: %p\n", *Surface); fflush(stdout);

	if(*Surface==NULL){
		char BitmapName[256];
		strcpy(BitmapName,Bitmap);
		printf("[DEBUG] DDLoadBitmap() returned NULL, calling UserMessage\n"); fflush(stdout);
		UserMessage(" DDLoadBitmap() failed",BitmapName, FALSE);
	}

	printf("[DEBUG] About to call DDSetColorKey(*Surface=%p, RGB(0,0,0))\n", *Surface); fflush(stdout);
	DDSetColorKey((*Surface), RGB(0, 0, 0));
	printf("[DEBUG] DDSetColorKey() OK\n"); fflush(stdout);
	printf("[DEBUG] LoadBitmapToSurface() COMPLETE\n"); fflush(stdout);

}

void Game::CreateBlankSurface(int width, int height, LPDIRECTDRAWSURFACE7 * Surface){

	if((*Surface))
		(*Surface)->Release();

	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
	ddsd.dwHeight=height;
	ddsd.dwWidth=width;
	ddsd.dwFlags=DDSD_HEIGHT | DDSD_WIDTH;

	g_pDD->CreateSurface(&ddsd,Surface,NULL);

	DDSetColorKey((*Surface), RGB(0, 0, 0));

}/*
int SetTransparency(LPDIRECTDRAWSURFACE7 Surface, double Tr){

	if(Tr<0||Tr>1)
		return TRUE;
	HDC                         hdc;
	if (Surface->GetDC(&hdc) == DD_OK){

		COLORREF Color;

		BYTE Red,Green,Blue;
		for(int y=120;y<370;y++)
			for(int x=70;x<720;x++){
				Color=GetPixel(hdc,x,y);
				if(Color!=CLR_INVALID){
					Red=GetRValue(Color);
					Green=GetGValue(Color);
					Blue=GetBValue(Color);
					if(Red)
						Red=(BYTE)((double)Red*Tr);
					if(Blue)
						Blue=(BYTE)((double)Blue*Tr);
					if(Green)
						Green=(BYTE)((double)Green*Tr);
					if(Red+Green+Blue)
					{
						SetPixelV(hdc,x,y,RGB((BYTE)Red,(BYTE)Green,(BYTE)Blue));		
					}
				}
			}

	}
	Surface->ReleaseDC(hdc);
	return TRUE;
}
*/

void Game::ScaleSurface(LPDIRECTDRAWSURFACE7 * Surface,
				  int OriginalColor, int DesiredColor,
						int Skip, int NumberOfColors){
	// Replaces a color scale with another color scale.
	// The scale starts at color nr <OriginalColor> and contains <NumberOfColors> colors.

	if(OriginalColor==DesiredColor)
		// No need to scale
		return;
	/*if( (OriginalColor<DesiredColor && OriginalColor + NumberOfColors*Skip >= DesiredColor) ||
		(DesiredColor>OriginalColor && DesiredColor + NumberOfColors*Skip >= OriginalColor) ){
			DP("Scale collision");
		// Scale Collision, scaling temporary
		// Using one as start since color 0 (black) is the color key
		if(NumberOfColors*Skip>=127){
			// Switching these large ranges aren't possible with 256 colors
			DP("Aborting scaling...");
			return;
		}
		if(NumberOfColors*Skip>=OriginalColor){
			DP("Scale case 1");
			ScaleSurface(Surface,OriginalColor,255-NumberOfColors*Skip,Skip,
						 NumberOfColors);
			OriginalColor=255-NumberOfColors*Skip;
		}else{
			DP("Scale case 2");
			ScaleSurface(Surface,OriginalColor,1,Skip,
						 NumberOfColors);
			OriginalColor=1;
		}
	}
*/


	DDSURFACEDESC2 ddDesc;
	ZeroMemory(&ddDesc, sizeof(ddDesc));
	ddDesc.dwSize = sizeof( ddDesc);

	HRESULT hr = (*Surface)->Lock(&rcRect,&ddDesc, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR ,NULL);
	
	int width=rcRect.right-rcRect.left;
	int height=rcRect.bottom-rcRect.top;

	if (hr == DD_OK ){

	
		int offset, Color;
		for(int FindColor=OriginalColor,ReplaceColor=DesiredColor;
				FindColor<OriginalColor+NumberOfColors;
				FindColor++,ReplaceColor+=Skip)
			for(int x=0;x<width;x++)
				for(int y=0;y<height;y++){
					offset = x + ddDesc.lPitch*y;
					Color = *((BYTE* )(((BYTE*)(ddDesc.lpSurface))+offset));
					if( Color == FindColor){
						*((BYTE* )(((BYTE*)(ddDesc.lpSurface))+offset))=ReplaceColor;
					}
				}
	}

	(*Surface)->Unlock(&rcRect);

}

void Game::FixImage(LPDIRECTDRAWSURFACE7 * Surface){
	BlitOntoBack(Surface);
	FlipAndWait();
	BlitOntoBack(Surface);			
}
void Game::FlipAndWait(){
	while(g_pDDSBack->GetFlipStatus( DDGFS_ISFLIPDONE ) == DDERR_WASSTILLDRAWING );
	while(g_pDDSPrimary->Flip(0, DDFLIP_WAIT) != DD_OK );
	while(g_pDDSBack->GetFlipStatus( DDGFS_ISFLIPDONE ) == DDERR_WASSTILLDRAWING );
}

void Game::ClearScreen(){
	BlankScreen();
	FlipAndWait();
	BlankScreen();
}

void Game::PrepareNewScreen(){
	ClearScreen();	
	ReleaseAllSurfaces();
}