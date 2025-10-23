#include "StrConversions.h"
#include <stdio.h>

char * Armor2Str(int ArmorGrade){
	if(ArmorGrade == 0)
		return "-";
	else if(ArmorGrade == 1)
		return "Red";
	else if(ArmorGrade == 2)
		return "Green";
	else if(ArmorGrade == 3)
		return "Blue";
	return "ERROR";
}
char * BodyColor2Str(int BodyColor){

	//return BodyColor2Name[BodyColor];
	if(BodyColor==0)
		return "Green";
	if(BodyColor==1)
		return "Blue";
	if(BodyColor == 2)
		return "Brown";
	if(BodyColor == 3)
		return "Cyan";
	if(BodyColor == 4)
		return "Purple";
	if(BodyColor == 5)
		return "Red";
	if(BodyColor == 6)
		return "Grey";
	if(BodyColor == 7)
		return "Yellow";
	if(BodyColor == 8)
		return "Orange";

	return "ERROR";
}
char * HairColor2Str(int HairColor){

	//return HairColor2Name[HairColor];
	if(HairColor==0)
		return "Brown";
	if(HairColor==1)
		return "Blonde";
	if(HairColor == 2)
		return "Red";
	if(HairColor == 3)
		return "Black";
	if(HairColor == 4)
		return "Whatever";
	if(HairColor == 5)
		return "Grey";


	return "ERROR";
}
char * Bool2Str(BOOL Answer){
	if(Answer)
		return "Yes";
	else 
		return "No";
}
char * Dword2TimeStr(DWORD TTS_Time) {
	TTS_Time/=1000;
	char s[5],mins[5];
	if(TTS_Time%60<10)
		sprintf(s,"0%i",TTS_Time%60);
	else
		sprintf(s,"%i",TTS_Time%60);
	TTS_Time/=60;
	if(TTS_Time%60<10)
		sprintf(mins,"0%i",TTS_Time%60);
	else
		sprintf(mins,"%i",TTS_Time%60);

	TTS_Time/=60;
	static char Returnage[40];

	sprintf(Returnage,"%i:%s:%s",TTS_Time,mins,s);
						
	return Returnage;

}