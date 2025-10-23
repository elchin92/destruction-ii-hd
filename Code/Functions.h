#include "MathematicalConstants.h"

const double sqArray[2][2] = {{1.0,sq2},{1.0,isq2}};
double toRad(int Angle);
void SideAdjust(int * X, int *Y, int Dir, int Adjust, int REALADJUST=0);
void LengthAdjust(int * X, int *Y, int Dir, int Length, int REALADJUST=0);
void IntSwitch(int *a, int *b);
void SwitchArray(int * a, int *b, int size);
void setMin(int *i, int min);
void setMax(int *i, int max);
void setMin(long *i, long min);
void setMax(long *i, long max);

int Cross(int x1, int y1, int t1, int x2, int y2, int t2);
int xDir(int Dir);
int yDir(int Dir);
double dist(double x, double y);
double sqsum(double x, double y);
long square(long input);
double msq2(double Value, int DOIT);
double dsq2(double Value, int DOIT);