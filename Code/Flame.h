#include "definitions.h"

enum Flame{f9, f556, f762, f12, fBazooka, fSpare, fBlueLaser, fRedLaser, fGreenLaser, fYellowFlame, NumberOfFlames, fNoFlame};

const Flame BulletType2Flame[NumberOfBulletTypes]={f9, f556, f762, f12, fBazooka,fBazooka,fBazooka,fBazooka,
				fBazooka, fBazooka, fRedLaser, fNoFlame, fNoFlame, fNoFlame, fNoFlame,
				fBazooka, fYellowFlame, fNoFlame};

const Flame LaserColor2Flame[3]={fRedLaser,fGreenLaser,fBlueLaser};

const int FlameSourcePos[NumberOfFlames][2]={{0,0},{32,0},{0,32},{32,32},{0,64},{32,64},
												{0,96},{16,96},{32,96},{48,96}};

const int FlameThickness[NumberOfFlames]={16,16,16,16,16,16,8,8,8,8};

const int TempFlamePos[2]={32,64};

const int FlameAdjust[8][2]={ {-1,-2},{-1,-1},{0,-1},{-1,-1},{-1,0},{-1,-1},{-2,-1},{-1,-1} };