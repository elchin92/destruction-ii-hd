#include "Weapon.h"

//													 Rf    Gun   Dual   Throw
// Blitta Vapen, Pilla bli!
const AimLengthAdjust[AimTypes][2]=			{ {0,0},{5,6},{9,9},{9,10},{0,0} };
const AimSideAdjust[AimTypes][2]=			{ {0,0},{0,2},{0,0},{0,0},{0,0} };

const ShoulderLengthAdjust[2] = {-12,-9};
const ShoulderSideAdjust[2]   = {  0, 0};
// Blitt Flames & kulor
const AimLengthDifference[AimTypes][2]=		{ {0,0},{1,5},{0,2},{0,2},{0,0} };
const AimSideDifference[AimTypes][2]=		{ {0,0},{3,-1},{0,0},{0,0},{0,0} };