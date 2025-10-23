#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define TOSHORTNAME -8

char Name[100];
unsigned __int16 a,b,c,d,e,f;

void main(){
	sprintf(Name,"Johan Torp");
	srand(GetTickCount());
	a=rand()+rand();
	b=rand()+rand();
	int sum=0,prod=1,i;
	for(i=0;i<100&&Name[i]!='\0';i++){
		if(Name[i]%2){
			sum+=Name[i]*3;
			prod*=(Name[i]+1)/2;
		}
		else{
			sum+=Name[i]/2;
			prod*=Name[i];
		}
	}
	if(i<5)
		throw TOSHORTNAME;


}