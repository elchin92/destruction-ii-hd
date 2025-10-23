#include "Ufo.h"
#include "Game.h"
#include "Player.h"
#include <math.h>

Ufo * TheUfo;

Ufo::Ufo(){
	ActualCenterPos=32;
	Actor::Initialize();
	ACTIVE=FALSE;
	TopSpeed=6000; // 1.000s of Pixles / Frame
	MaxAcc=17,MinAcc=5; // 1.000s of Pixles / (Frame sq)
	BeamDist=140; // Must be smaller than 11*32 /2
	Actor::SetBlit(162,3,58,58,DIR_0,TT_NOTICK,1,&(TheGame->g_pDDSSprites));

}
Ufo::~Ufo(){
}
/*
void Ufo::NewDir(){

	// Not used any more
	for(int xy=0;xy<2;xy++){	
		dSpeed[xy] = rand()%TopSpeed;
		for(int i=0;i<3;i++)
			dSpeed[xy]/=10.0;
		if(rand()%2)
			dSpeed[xy]=-dSpeed[xy];
	}
	NewAcc();
}
*/
int Ufo::MoveThroughWall(int XS, int YS){return 1;}
void Ufo::NewAcc(){
	
	for(int xy=0;xy<2;xy++){	
		Acc[xy]=rand()%(MaxAcc-MinAcc);
		Acc[xy]+=MinAcc;
		if(rand()%2)
			Acc[xy]=-Acc[xy];
		for(int i=0;i<3;i++)
			Acc[xy]/=10.0;
	}
	
	/* OLD

	double Acceleration = rand()%(MaxAcc-MinAcc);
	Acceleration+=MinAcc;
	double a=dSpeed[0]-cSpeed[0];
	double b=dSpeed[1]-cSpeed[1];
	double c=sqrt(a*a+b*b);
	Acc[0]=a*Acceleration;
	Acc[0]/=c;
	Acc[1]=b*Acceleration;
	Acc[1]/=c;
	for(int i=0;i<4;i++){
		Acc[0]/=10.0;
		Acc[1]/=10.0;
	}
*/
}

BOOL Ufo::IsActive(){
	return ACTIVE;
}


int Ufo::GetX(){
	return Pos[0];
}
int Ufo::GetY(){
	return Pos[1];
}

void Ufo::ActivateIt(){
	ACTIVE=TRUE;
	msPF=TheGame->Dlay;

	vWS=TheGame->WorldSize*32*11;

	xPoints=0;
	for(int xy=0;xy<2;xy++){
		cSpeed[xy]=0.0;
		//cSpeed[xy]=rand()%TopSpeed;
		cSpeed[xy]/=1000;
		xPos[xy]=0;
		//Pos[xy]=150;
		Pos[xy]=rand()%(vWS-64);
		BEAM[xy]=FALSE;
	}
	NewAcc();
}

void Ufo::Move(){
	// How many Frames?
	if(ACTIVE){
		double Points = TheGame->TheFrame;


		if(Points>0){
	
			Points/=(double)msPF;
			BeamEm(Points);
			for(int xy=0;xy<2;xy++){
				//Travel
				Pos[xy]+=(int)(Points*cSpeed[xy]+xPos[xy]);
				xPos[xy]=(Points*cSpeed[xy]+xPos[xy]) - (int)(Points*cSpeed[xy]+xPos[xy]);
				// Accelerate
				if( (Acc[xy] > 0 && cSpeed[xy]*1000 < TopSpeed) || (Acc[xy] < 0 && -cSpeed[xy]*1000 < TopSpeed ) )
					cSpeed[xy]+= Points * Acc[xy];
				if(Pos[xy]>vWS+64||Pos[xy]<-64)
					Kill();
			}
				
			while(Points>0){
				if (!(rand()%64)){
					NewAcc();
					break;
				}
				Points--;
			}


			
			
		}
	}
}
BOOL Ufo::BeamActive(int Player){
	return BEAM[Player];
}

void Ufo::BeamEm(double Points){
	// Beam Em b4 moving
	
	int a,b;
	Points*=5;

	for(int pl=0;pl<2;pl++){

		
		a=ThePlayers[pl]->GetActualXPos()-(Pos[0]+32);
		b=ThePlayers[pl]->GetActualYPos()-(Pos[1]);
		if(!ThePlayers[pl]->Frozen()&&sqrt(a*a+b*b)<=BeamDist){
			if( !IsPlaying())
				StartPlaying();
			BEAM[pl]=TRUE;
			ThePlayers[pl]->Damage((int)(Points+xPoints), DT_SPECIALUNIT, B_NRJ, NULL);
		}
		else
			if(BEAM[pl]){
				BEAM[pl]=FALSE;	
			}
	}
	xPoints=(Points+xPoints)-(int)(Points+xPoints);
	
	
}

void Ufo::Kill(){
	ACTIVE=FALSE;
	StopPlaying();
}

void Ufo::BlitBeam(int where){
	if(BeamActive(where)){
		
		int x1=ThePlayers[where]->GetActualXPos()-TheGame->Xstart*32,y1=ThePlayers[where]->GetActualYPos()-TheGame->Ystart*32;
		int x2=GetX()+32-TheGame->Xstart*32,y2=GetY()+32-TheGame->Ystart*32;
		double a = x1-x2;
		double b = y1-y2;
		double c = sqrt(a*a + b*b);
		
		// here
		if(c>34){
			x2 += (int)((32.0 * a)/c);
			y2 += (int)((32.0 * b)/c);
			HDC                         hdc;
			//SIZE                        size;
			
			// Ufo beam
			if (TheGame->g_pDDSTemporary->GetDC(&hdc) == DD_OK)
			{
				
				SelectObject(hdc,TheGame->bPen);
				SelectObject(hdc,TheGame->theBrush);
				//CPoint p = (*HWND).MoveTo(x2,y2);
	
/*				if(TheGame->ThePlayers[where]->GetX()>160&&TheGame->ThePlayers[where]->GetX()<TheGame->WorldSize*352-192){
					x1-=TheGame->ThePlayers[where]->GetX()%32;
					x2-=TheGame->ThePlayers[where]->GetX()%32;
				}
				if(TheGame->ThePlayers[where]->GetY()>160&&TheGame->ThePlayers[where]->GetY()<TheGame->WorldSize*352-192){
					y1-=TheGame->ThePlayers[where]->GetY()%32;
					y2-=TheGame->ThePlayers[where]->GetY()%32;
				}
				x1+=32;
				x2+=32;
				y1+=32;
				y2+=32;*/

				if(x1<=32-32)
					x1=33-32;
				if(x2<=32-32)
					x2=33-32;
				if(y1<=32-32)
					y1=33-32;
				if(y2<=32-32)
					y2=33-32;
				if(x1>=383)
					x1=382;
				if(x2>=383)
					x2=382;
				if(y1>=383)
					y1=382;
				if(y2>=383)
					y2=382;
				
				x1+=384*where;
				x2+=384*where;
				if(abs(x1-x2)<160 && abs(y1-y2)<160 ){
					MoveToEx(hdc,x2,y2,0);
					LineTo(hdc,x1,y1);	
				}
				//(*HWND).MoveTo(p);
				
			}			
			TheGame->g_pDDSTemporary->ReleaseDC(hdc);
		}
	}
}