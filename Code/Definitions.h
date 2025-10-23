#ifndef DEFINITIONS_DEFINED
#define DEFINITIONS_DEFINED

//#define __D2PROTECTED__
#define __DIGITALRIVERRELEASE__

//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define NAME                "Destruction"
#define TITLE               "Destruction"

#define PLAYERTHICKNESS		10


#define WT_RIFLE			0
#define WT_PSMG				1
#define WT_DUAL				2
#define WT_THROW			3

#define SMALLEXP			31
#define NORMALEXP			52
#define	BIGEXP				42
#define NOEXP				00


#define	PLAYERLEAVE			2
#define SHOPPING			3

#define ON					0
#define OFF					1

#define RETURN				34
#define	QUIT				54

#define PLAYING				0
#define	ROUNDOVER			1


#define IV60		0
#define SP60		1
#define SPL			2
#define SPR			3
#define SH60		4
#define SHL			5


#define WP_ACTIVE	0
#define WP_SELECTED	1

#define S_BUY		0
#define S_SELL		1

#define SND_SMAMMO	0
#define SND_BAMMO	1
#define SND_SMCASH	2
#define SND_BCASH	3
#define SND_1UP		4
#define SND_INVIS	5
#define SND_SHIELD	6
#define SND_SPEED	7
#define SND_MINE	8
#define SND_MEDI	9
#define SND_CHARGE	10
#define SND_DCHARGE	20
#define SND_NONE	-1

#define KEY_UP		0
#define KEY_DOWN	1
#define KEY_LEFT	2
#define KEY_RIGHT	3
#define KEY_FIRE	4
#define KEY_ALTFIRE	5
#define KEY_CHARGE	6
#define KEY_MINE	7
#define KEY_TMINE	8
#define KEY_SPEC1	9
#define KEY_SPEC2	0
#define KEY_SPEC3	11
#define KEY_SWITCH	12

#define IT_9MM		0
#define IT_556		1
#define IT_762		2
#define IT_12		3
#define IT_SSHELLS	4
#define IT_NRJ		5
#define IT_SHELLS	6
#define IT_APMIS	7
#define IT_CHARGES	8
#define IT_GRENADE	9
#define IT_CMINE	10
#define IT_SDOLLAR	11
#define IT_DDOLLAR	12
#define IT_CASHHEAP	13
#define IT_CHEST	14
#define IT_X2		15
#define IT_SPEED	16
#define IT_SHIELD	17
#define IT_INVIS	18
#define IT_TRINITY	19
#define IT_QUESTION	20
#define IT_SMEDI	21
#define IT_MEDI		22
#define IT_1UP		23
#define IT_RFIRE	24
#define IT_DBLDMG	25
#define IT_STOP		26
#define IT_LENSE	27
#define IT_SUPERMAN	28
#define IT_MINE		29
#define IT_TMINE1	31
#define IT_TMINE2	40
#define IT_MINE1	41
#define IT_MINE2	42



#define OPEN		0
#define CLOSED		1
#define OPENING		2
#define CLOSING		3

#define W_TURN		0
#define W_CLEAR		1
#define W_RUNOVER	2

#define EXP_BIG		0
#define EXP_NORMAL	1
#define EXP_SMALL	2
#define EXP_SINGLE	3
#define EXP_NONE	4


#define WAY_CLEAR	0
#define WAY_TURN	1
#define WAY_STOP	2
#define WAY_DIED	3
#define WAY_KILL	4

// Must be in priority order




#define	DIR_0		0
#define	DIR_4		1
#define	DIR_8		2

#define	AN_AIR		0
#define	AN_EXP		1
#define	AN_GROUND	2

#define GI_NOMINE	0
#define GI_REGULAR	1
#define	GI_MMAZE	2

#define	AT_UNDER	0

#define TT_NOTICK	0
#define TT_PINGPONG 1
#define	TT_ROLLER	2

#define ACCPARTS	1000
#define TAZERRANGE	100
#define TAZERTIME	50
#define LASERTIME	50

#define C_RED		0 // Gotta be RGB (for the sounds)
#define C_GREEN		1
#define C_BLUE		2

#define SPECIALATTACK	1000

#define AMMOITEMS		20

#define MISCITEMS		18
#define MISC_XLIFE		17
#define MISC_SLOTDEPTH	14
#define MISC_DUALITY	16

#define WPN_ATSHOT		25

#define XChangeDir4(d) ((d==1) - (d==3))
#define YChangeDir4(d) ((d==2) - (d==0))

#define MovingUp8(p)	(p==7||p==0||p==1)
#define MovingRight8(p)	(p==1||p==2||p==3)
#define MovingDown8(p)	(p==3||p==4||p==5)
#define MovingLeft8(p)	(p==5||p==6||p==7)

#define MovingUp4(p)	(p==0)
#define MovingRight4(p)	(p==1)
#define MovingDown4(p)	(p==2)
#define MovingLeft4(p)	(p==3)

#define MovingUp(d, t) ( (t==DIR_4 && MovingUp4(d)) || (t==DIR_8 && MovingUp8(d)) )
#define MovingDown(d, t) ( (t==DIR_4 && MovingDown4(d)) || (t==DIR_8 && MovingDown8(d)) )
#define MovingLeft(d, t) ( (t==DIR_4 && MovingLeft4(d)) || (t==DIR_8 && MovingLeft8(d)) )
#define MovingRight(d, t) ( (t==DIR_4 && MovingRight4(d)) || (t==DIR_8 && MovingRight8(d)) )

#define CheckQuad(x,y) (x >=0 && y>=0 && x<TheGame->WorldSize && y<TheGame->WorldSize)
#define CheckSquare(x,y) (x >=0 && y>=0 && x<TheGame->WorldSize*11 && y<TheGame->WorldSize*11)
#define CheckPoint(x,y) (x >=0 && y>=0 && x<TheGame->WorldSize*11*32 && y<TheGame->WorldSize*11*32)



enum ActorBlitType{BLT_SHOT, BLT_MORTAL, BLT_PLAYER, BLT_CLONE};

#define DP_NONE			0
#define DP_ACTIVE		1
#define DP_DESTROYED	2



enum ThicknessType{T_PLAYER, T_PACMAN, T_JEEP, T_HUMVEE, T_STEAM, T_WHEEL, T_INFANTRYDROID, T_BATTLEDROID, T_WALKER, T_MINITANK, T_SILVERTANK, T_DOUBLEBARRELEDTANK, T_BIGTANK, T_ROCKETTANK, T_SENTRYGUN, ThicknessTypes};

#define T_CLONE				T_PLAYER
#define T_CAR				T_JEEP
#define T_MACHINE			T_STEAM
#define T_DROID				T_INFANTRYDROID
#define T_TANK				T_MINITANK
#define T_SMALLESTSHOOTER	T_DROID

enum CheatType{cPacman,cPrey,cRocket,cBoom, cSpeed, cMight, NoOfCheats};

enum MortalType{M_PL, M_CL, M_TK, M_DR, M_MA, M_CA, M_SG, M_PM, NumberOfMortals};

const int Units[NumberOfMortals]={1,1,5,3,2,2,1};
const int SmallestUnitThicknessType[NumberOfMortals]={T_PLAYER,T_CLONE,T_TANK,T_DROID,T_MACHINE,T_CAR,T_SENTRYGUN,T_PACMAN};


enum ArmorType{AT_WEST,AT_HELMET,AT_PANTS,AT_BOOTS,AT_AURA, AT_NONE, 
			   NumberOfArmorTypes};

enum DamageType{DT_SHOT, DT_NRJ, DT_RUNOVER, DT_CHARGE, DT_GRENADE, DT_SPECIAL, DT_APMISSILE, DT_ATMISSILE,
				DT_PMINE, DT_TMINE, DT_BASEDOOR, DT_SPECIALUNIT, DT_SHELL, DT_NONE, DT_MORTALEXPLOSION,
				NumberOfDamageTypes};

 // not sufficient information!
const ArmorType DamageTypeToArmorType[NumberOfDamageTypes]={
				AT_WEST, AT_AURA, AT_NONE, AT_HELMET, AT_HELMET, AT_NONE, AT_HELMET,
				AT_BOOTS, AT_NONE, AT_NONE, AT_HELMET, AT_NONE};

// Dont't change bullettypes without adjusting SoundEnum.h

enum BulletType{B_9, B_556, B_762, B_12, B_SHELL, B_HESHELL, B_APRKT, B_ATRKT,
			    B_ATSHOT, B_SH, B_NRJ, B_CHARGE, B_MINE, B_TMINE,
				GR_GRENADE, // Player owned bullets end here
				GR_SHELL, GR_ROCKET, B_MISSILE, NumberOfBulletTypes,
				B_NOBULLET};  // Grenades must be subsequent

#define GR_LOWEST GR_GRENADE
#define PlayerOwnedBullets (GR_GRENADE+1)

const int	llimit=9999,		rlimit=99;
const int bbLimit[PlayerOwnedBullets]={llimit, llimit, llimit, llimit, rlimit, rlimit, rlimit, rlimit,
							      llimit, llimit, llimit, llimit, llimit, rlimit,llimit};


const ArmorType BulletTypeToArmorType[NumberOfBulletTypes] = {
				AT_WEST, AT_WEST, AT_WEST, AT_WEST, AT_HELMET, AT_HELMET, AT_HELMET, AT_NONE,
				AT_NONE, AT_WEST, AT_AURA, AT_HELMET, AT_BOOTS, AT_NONE, 
				AT_HELMET, 
				AT_HELMET, AT_HELMET, AT_HELMET};


const DamageType BulletTypeToDamageType[NumberOfBulletTypes] = {
				 DT_SHOT, DT_SHOT, DT_SHOT, DT_SHOT, DT_SHELL, DT_SHELL, DT_APMISSILE, DT_ATMISSILE,
				 DT_ATMISSILE, DT_SHOT, DT_NRJ, DT_CHARGE, DT_PMINE, DT_TMINE, DT_GRENADE,
				 DT_SHELL, DT_APMISSILE, DT_SPECIAL };


#define T_NONE		0
#define T_APMINE	1
#define T_ATEXP		2
#define T_APEXP		4
#define T_REGEXP	8
#define T_BULLET	16	
#define T_NRJ		32
#define T_ATMINE	64

const int DamageTypeToStatusBit[(int)NumberOfBulletTypes]=
		{T_BULLET, T_NRJ, T_NONE, T_REGEXP, T_APEXP, T_APEXP, T_APEXP, T_ATEXP, 
		 T_APMINE, T_ATMINE, T_NONE, T_NONE, T_APEXP, T_NONE, T_APEXP};


enum RegExpType{RET_SWEAK, RET_MWEAK, RET_AIR, RET_CLUSTER, RET_SPLUS, RET_MPLUS, 
				RET_SX, RET_MX, RET_CHARGE, RET_GRENADE, RET_SQUAD, RET_MQUAD, RET_PMINE, RET_TMINE,
				RET_MORTALEXPLOSIONSMALL, RET_MORTALEXPLOSIONNORMAL, RET_MORTALEXPLOSIONBIG,
				RET_COVER, RET_CSMALL, RET_CMEDIUM, RET_CBIG, RET_CHUGE, RET_ENTIREWORLD,RET_NONE,
				NumberOfRegExpTypes}; // RET_C XXX reserved for specials

#define RET_RADIUSMIN RET_CSMALL
#define RET_RADIUSMAX RET_CHUGE
#define RET_RADIUSBLOWS (RET_RADIUSMAX-RET_RADIUSMIN+1)

#define RET_MISSILE RET_CHUGE
#define RET_DETPACK RET_CBIG
/* Can't gp RegExp->Damate
const DamageType RegExpTypeToDamageType[NumberOfRegExpTypes] = {
				DT_SHELL, DT_MISSILE, DT_SPECIAL, DT_SPECIAL, DT_SHELL, DT_MISSILE,
				DT_SHELL, DT_MISSILE, DT_CHARGE, DT_GRENADE, DT_SHELL, DT_MISSILE, DT_PMINE, DT_TMINE,
				DT_MORTALEXPLOSION, DT_MORTALEXPLOSION, DT_MORTALEXPLOSION, 
				DT_SPECIAL, DT_SPECIAL, DT_SPECIAL, DT_SPECIAL, DT_SPECIAL, DT_NONE
};*/

const int RegExpRadiuses[RET_RADIUSBLOWS]={3,4,5,7};
#define RegularSpritedBullets (B_ATSHOT+1)
#define PlayerBulletTypes (GR_GRENADE+1)
//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define XYW2DIR4(xy, way) ((way+(xy==0) + 4)%4)
#define KillLinkedList(type, start, cond, next)			{ type * prev=NULL;type * it=(start);while(it){type * kill=NULL;if(it->cond){if(prev)	{prev->next=it->next;}else{start=it->next;}kill=it;}if(!kill)prev=it;it=it->next;if(kill)delete kill;}}
#define KillLinkedListIndCond(type, start, cond, next)	{ type * prev=NULL;type * it=(start);while(it){type * kill=NULL;if(cond){if(prev)		{prev->next=it->next;}else{start=it->next;}kill=it;}if(!kill)prev=it;it=it->next;if(kill)delete kill;}}
#define KillEntireLinkedList(type,start,next) { type * prev=NULL;type * it=(start);while(it){type * kill=NULL;		 if(prev)		{prev->next=it->next;}else{start=it->next;}kill=it; if(!kill)prev=it;it=it->next;if(kill)delete kill;}}
#define KillEntireCircularLinkedList(type, start, next)	{type * ckill = start->next;	start->next=NULL;KillEntireLinkedList(type, ckill, next);}
#define RemoveFromLinkedList(type, list,next,item) {if(list==item)list=item->next;else{type * temp; for(temp=list;temp->next!=item;temp=temp->next);	temp->next=temp->next->next;}}  // C++20: declare temp before for-loop
#define AddLastToLinkedList(type, list, next, item) {if(!list)list=item;else{type * last=list;while(last->next){last=last->next;}last->next=item;}}
#define AddToLinkedList(list, next,item) {item->next=list;list=item;}
#define RemoveThisFromLinkedList(type, list, next) RemoveFromLinkedList(type, list,next,this)
#define AddThisToLinkedList(list, next) AddToLinkedList(list, next,this)
#define AddThisLastToLinkedList(type, list, next) AddLastToLinkedList(type, list, next, this)
#define pos(a) (a>0?a:0)
#define CopyArray(a1, a2, sz) for(int ca=0;ca<sz;ca++)a1[ca]=a2[ca]; 
#define UZ() int ziq;
#define UZ2() UZ();int zaq;
#define Zero(array, max)  for(ziq=0;ziq<max; ziq++)array[ziq]=0;
#define Zero2d(array, maxin, maxout) for(ziq=0;ziq<maxin; ziq++)for(zaq=0;zaq<maxout; zaq++)array[ziq][zaq]=0;
#define CalculateZum(array, elems)int Zum=0;for(int yue=0;yue<elems;yue++)Zum+=array[yue];	

// STATUS BITS
/*
#define INVISIBILITY	0x0001
#define SHIELD			0x0002
#define SUPER			0x0004
#define SEEKER			0x0008
#define WALLBREAKER		0x000F
#define DUALFIRE		0x0010*/

#define PseudoQuit() {DP("PseudoQuit()");if(!(rand()%5)){PostMessage(TheInputEngine->hDlg,2,0,0);}}

#endif