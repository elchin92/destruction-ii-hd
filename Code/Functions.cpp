#include "math.h"
#include "Functions.h"

double toRad(int Angle){
	return 	(pi * (double)Angle) /180.0;
}
void IntSwitch(int *a, int *b){
	int t=(*a);
	(*a)=(*b);
	(*b)=t;
}

void SwitchArray(int * a, int *b, int size){
	for(int i=0;i<size;i++)
		IntSwitch(&a[i],&b[i]);
}
void setMin(int *i, int min){
	if((*i)<min)
		(*i)=min;
}
void setMax(int *i, int max){
	if((*i)>max)
		(*i)=max;
}
void setMin(long *i, long min){
	if((*i)<min)
		(*i)=min;
}
void setMax(long *i, long max){
	if((*i)>max)
		(*i)=max;
}
void SideAdjust(int * X, int *Y, int Dir, int Adjust, int REALADJUST){

	Adjust=(int)dsq2(Adjust,REALADJUST&&Dir%2);

	if(Dir==5||Dir==6||Dir==7)
		(*Y)-=Adjust;
	else if(Dir==1||Dir==2||Dir==3)
		(*Y)+=Adjust;
	if(Dir==3||Dir==4||Dir==5)
		(*X)-=Adjust;
	else if(Dir==7||Dir==0||Dir==1)
		(*X)+=Adjust;
		
}
void LengthAdjust(int * X, int *Y, int Dir, int Length, int REALADJUST){


	Length=(int)dsq2(Length,REALADJUST&&Dir%2);


	if(Dir==5||Dir==6||Dir==7)
		(*X)-=Length;
	else if(Dir==1||Dir==2||Dir==3)
		(*X)+=Length;
	if(Dir==3||Dir==4||Dir==5)
		(*Y)+=Length;
	else if(Dir==7||Dir==0||Dir==1)
		(*Y)-=Length;

}
int Cross(int x1, int y1, int t1, int x2, int y2, int t2){
	return !((x1>x2+t2 || x1+t1<x2) || (y1>y2+t2||y1+t1<y2));
}
int yDir(int Dir){
	if(Dir==3||Dir==4||Dir==5)
		return 1;
	if(Dir==7||Dir==0||Dir==1)
		return -1;
	return 0;
}
int xDir(int Dir){
	if(Dir==5||Dir==6||Dir==7)
		return -1;
	else if(Dir==1||Dir==2||Dir==3)
		return 1;
	return 0;
}
long square(long input){
	return (input*input);
}
double msq2(double Value, int DOIT){

	return Value*sqArray[0][DOIT];
}
double dsq2(double Value, int DOIT){

	return Value*sqArray[1][DOIT];
}
double dist(double x, double y){
	return sqrt(sqsum(x,y));
}
double sqsum(double x, double y){
	return x*x+y*y;
}