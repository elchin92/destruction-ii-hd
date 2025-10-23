#include "Player.h"
#include "ddex4.h"
#include "Weapon.h"
#include "WallEvent.h"
#include "AimRules.h"
#include "PlayerBlit.h"

const int PlayerBlitAdjust[8][2]={  {0,  -32},
{0,	-16},
{0,	0},
{0,  0},
{0,  0},
{-16,0},
{-32,-0},
{-16,-16} };



Actor::Actor(){}

Actor::~Actor(){

}

int Actor::HitWall(){


	if(TheGame->WorldMap[ GetXs() ][ GetYs() ][1]<=0)
		return FALSE;

	int Piece=TheGame->WorldMap[ GetXs() ][ GetYs() ][0]-1;

	if(Piece>=16)
		// Base becomes too complex special case
		return TRUE;

	int rX = (Pos[0]+ActualCenterPos)%32, rY = (Pos[1]+ActualCenterPos)%32;

	if( (rY >= WallAdjust && rY <= 31-WallAdjust) ){
		// MidX
		int NoLeftPiece=(Piece&2)==0;
		int NoRightPiece=(Piece&1)==0;

		if(rX >= WallAdjust*NoLeftPiece && rX <= 31-WallAdjust*NoRightPiece);
			// Down
			return TRUE;		
	}

	if( (rX >= WallAdjust && rX <= 31-WallAdjust) ){
		// MidY

		int NoUpperPiece=(Piece&8)==0;
		int NoLowerPiece=(Piece&4)==0;

		if(rY >= WallAdjust*NoUpperPiece && rY <= 31-WallAdjust*NoLowerPiece);
			// Down
			return TRUE;
		
	}

	
	
	return FALSE; 


}

BYTE Actor::GetBlitColor(){
	return 0;
}

int Actor::OutOfMap(){
	return (GetXs()>=TheGame->WorldSize*11 || GetXs()<0 || GetYs()>=TheGame->WorldSize*11 || GetYs()<0); 
}


int Actor::IsInvisible(){
	return INVISIBLE;
}

int Actor::GetTeam(){
	return Team;
}
void Actor::Initialize(){
	SoundSource::Initialize();
	SHIELD=FALSE;
	INVISIBLE=FALSE;

	Thickness=0;
	PingPong=1;
	MPRest=0;
	Dir=0;
	TickFrame=0;
	TICKHORIZONTAL=FALSE;
	GoThroughWalls=FALSE;
	GoThroughActors=FALSE;

	// Important for laser beams
	BlitDimension[0]=0;
	BlitDimension[1]=0;
	BreaksWalls=FALSE;

}
void Actor::SetBlitRect(int Mult){
	TheGame->rcRect.left =   BlitStart[0]+(Mult*GetRealDir()+GetTicks()*TICKHORIZONTAL)      *BlitDimension[0];
	TheGame->rcRect.top =    BlitStart[1]+GetTicks()*(!TICKHORIZONTAL)    *BlitDimension[1];
	TheGame->rcRect.right =  TheGame->rcRect.left + BlitDimension[0];//BlitStart[0]+(Mult*GetRealDir()+GetTicks()*TICKHORIZONTAL+1)  *BlitDimension[0];
	TheGame->rcRect.bottom = TheGame->rcRect.top + BlitDimension[1];BlitStart[1]+(GetTicks()*(!TICKHORIZONTAL)+1)*BlitDimension[1];
}
int Actor::MoveThroughWall(int XS, int YS){return 1;}

void Actor::Blit(int where){


	if(IsPlayer()||IsClone()){
		if(IsInvisible()&&Team!=where)
			return;
		// Players require special blit
		BlitPlayer();
		return;
	}
	
	
	if(where<2){
		SetBlitRect(DirType!=DIR_0);
	}else{
		// 4 = TickMax
		where-=2;
		TheGame->rcRect.left =   BlitStart[0]+GetRealDir()      *BlitDimension[0];
		TheGame->rcRect.top =    BlitStart[1]+TickFrames*BlitDimension[1];  
		TheGame->rcRect.right=TheGame->rcRect.left+BlitDimension[0];
		TheGame->rcRect.bottom=TheGame->rcRect.top+BlitDimension[1];
	}
	if(IsMortal()){
		if(IsMortal()->GetMType()==M_PL){
			DP("Wazaa");
			BeaconRect();
			DP2("teamm",Team);
			Beacon(BlitStart[1]);
			Beacon(GetTicks());
			Beacon(!TICKHORIZONTAL);
		}
	}
	TheGame->BGOS(Pos[0],Pos[1],g_pDDSBlitSource, GetBlitColor());
}
double Actor::GetMPRest(){return MPRest;}
void Actor::BlitPlayer(){


	DBHI(87293);
	DP("BlitPlayer()");
	int Rotated=0;
	int BlitDir=2*Dir;
	int Firing = 0;
	if(IsPlayer()){
		Firing=this->IsPlayer()->GetAimType();
		BlitDir=Dir;
		Rotated=Dir%2;
	}else{
		DP("Blitting Clone");
	}

	DP2("Team",Team);
	DP2("BlitDir",BlitDir);
	DP2("Dir",Dir);
	DP2("Rotated",Rotated);
	DP2("IsPlayer()",(int)IsPlayer());	
	DP2("Firing",Firing);

	// Blank temporary space
	TheGame->BlankSurfaceArea(0,0,64,64,&TheGame->g_pDDSPlayerAssembly);

	TheGame->rcRect.left =   PlayerBlitStartX+(2*Firing+Rotated)*BlitDimension[0];
	TheGame->rcRect.top =    PlayerBlitStartY+GetTicks()*BlitDimension[1];
	TheGame->rcRect.right =  TheGame->rcRect.left+BlitDimension[0];
	TheGame->rcRect.bottom = TheGame->rcRect.top +BlitDimension[1];

	DP("BeaconRect, blitontosetrc");
	BeaconRect();

	TheGame->ColorKey=DDBLTFAST_NOCOLORKEY;
	TheGame->BlitOntoSetRc(16,16, &TheGame->g_pDDSPlayerAssembly,g_pDDSBlitSource);
	TheGame->ColorKey=DDBLTFAST_SRCCOLORKEY;

	// Blit aiming, weapon and flame
	if(Firing){
			DBHI(87294);
		DP("IsFiring");
		Player * apl = IsPlayer();

		// Blit Weapon

	


		// We need to blit the weapon twice if we have dual fire
		for(int side=-1;side<=apl->DualFire();side+=2){

			TheGame->rcRect.left  = WeaponBlitX+BlitDimension[0]*Rotated;
			TheGame->rcRect.top   = WeaponBlitY;  
			TheGame->rcRect.right =	TheGame->rcRect.left+BlitDimension[0];
			TheGame->rcRect.bottom=	TheGame->rcRect.top+BlitDimension[1];
			DP("BeaconRect, BlitWeapon");
			BeaconRect();

			int X=0,Y=-16;

			if(Rotated){
				X=12,Y=-12;	
			}

			int SideAdj=AimSideAdjust[Firing][Rotated]+
				+apl->GetWeapon()->GetBazookaSideAdjust(Rotated)+
				apl->DualFire()*(this->IsShooter()->GetPipeDistance(Rotated))*side;
			int LengthAdj=AimLengthAdjust[Firing][Rotated]+apl->GetWeapon()->GetBazookaLengthAdjust(Rotated);
;
			
			SideAdjust(&X,&Y,Rotated,SideAdj);
			LengthAdjust(&X,&Y,Rotated,LengthAdj);

			DP("Look here!");
			DP2("X",X);
			DP2("Y",Y);

			if(X>16){
				TheGame->rcRect.right-=X-16;
			}else if(X+16<0){
				TheGame->rcRect.left-=X+16;
				X=-16;
			}
			if(Y>16){
				TheGame->rcRect.bottom-=Y-16;
			}else if(Y+16<0){
				TheGame->rcRect.top-=Y+16;
				Y=-16;
			}
			DP2("X",X);
			DP2("Y",Y);
			BeaconRect();
			TheGame->BlitOntoSetRc(16+X,16+Y, &TheGame->g_pDDSPlayerAssembly,g_pDDSBlitSource);

		}

	}

	if(GetBlitColor()){
		TheGame->BlankSurfaceArea(RotatedX,RotatedY,64,64,g_pDDSBlitSource);
	}

	TheGame->rcRect.left  = 0;
	TheGame->rcRect.top   = 0;
	TheGame->rcRect.right =	64;
	TheGame->rcRect.bottom=	64;
	DBHI(87296);

	TheGame->ColorKey=DDBLTFAST_NOCOLORKEY;
	TheGame->BlitOntoSetRc(RotatedX,RotatedY,g_pDDSBlitSource,&TheGame->g_pDDSPlayerAssembly, GetBlitColor(), Get4Dir()); // Rotate-Blit
	TheGame->ColorKey=DDBLTFAST_SRCCOLORKEY;

	TheGame->rcRect.left  = RotatedX;
	TheGame->rcRect.top   = RotatedY;  
	TheGame->rcRect.right =	TheGame->rcRect.left+64;
	TheGame->rcRect.bottom=	TheGame->rcRect.top +64;
	DBHI(87297);

	TheGame->BGOS(Pos[0]-16,Pos[1]-16,g_pDDSBlitSource);
	DBHI(87298);
}


void Actor::SetBlit(int BlitX, int BlitY, int BlitWidth, int BlitHeight, int dirType, int ticktype, int tickframes, LPDIRECTDRAWSURFACE7 * BlitSource){
	BlitDimension[0]=BlitWidth;
	BlitDimension[1]=BlitHeight;
	BlitStart[0]=BlitX;
	BlitStart[1]=BlitY;
	DirType=dirType;
	g_pDDSBlitSource=BlitSource;
	Tick=0;
	TickType=ticktype;
	TickFrames=tickframes;
}
shot * Actor::IsShot(){return NULL;}
Player * Actor::IsPlayer(){return NULL;}
Mortal * Actor::IsMortal(){return NULL;}
Shooter * Actor::IsShooter(){return NULL;}
Clone * Actor::IsClone(){return NULL;}

int Actor::GetTicks(){
	return Tick;
}
int Actor::Get4Dir(){

	if(DirType==DIR_8)
		return Dir/2;
	
	return Dir;
}
int Actor::Get8Dir(){

	if(DirType==DIR_4)
		return Dir*2;
	return Dir;
}
int Actor::GetRealDir(){

	return Dir;
}

int Actor::GetThickness(){
	return Thickness;
}
int Actor::GetDirType(){
	return DirType;
}

int Actor::GoesThroughActors(){return GoThroughActors;}
int Actor::GoesThroughWalls(){return GoThroughWalls;}

void Actor::AddMeToCurrentQuadrant(){DP2("Team",Team);}
void Actor::RemoveMeFromLastQuadrant(){}
int Actor::HasLeftScreen(){
	return Pos[0] >= TheGame->WorldSize*11*32 ||
		   Pos[1] >= TheGame->WorldSize*11*32 ||
		   Pos[0]+BlitDimension[0]<0 ||
		   Pos[1]+BlitDimension[1]<0;
}
