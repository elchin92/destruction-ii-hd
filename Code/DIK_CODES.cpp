#include "DIK_CODES.h"
#include "debugger.h"
#include <string.h>
#include <stdio.h>

char						DikName[MaxKeys][17];
char						DikCharName[MaxKeys];

const char * DIK2STR(int CharCode){  // const char* для C++20 совместимости

	Beacon(523444);
	DP2("baba",CharCode);
	if(CharCode<MaxKeys)
		return DikName[CharCode];
	return "Illegal Code";
}
char DIK2ASCII(int CharCode){


	if(CharCode>=MaxKeys)
		return ' ';

	return DikCharName[CharCode];
}

int CharNameAvailable(int CharCode){
	return DikCharName[CharCode]!=0;
}
void SetDikNames(){

	for(int i=0;i<MaxKeys;i++){
		sprintf(DikName[i],"UNKNOWN KEY %i",i+1);
	}

	sprintf(DikName[0]
		,"(unassigned)");
	sprintf(DikName[1]
		,"ESCAPE");
	sprintf(DikName[2]
		,"1");
	sprintf(DikName[3]
		,"2");
	sprintf(DikName[4]
		,"3");
	sprintf(DikName[5]
		,"4");
	sprintf(DikName[6]
		,"5");
	sprintf(DikName[7]
		,"6");
	sprintf(DikName[8]
		,"7");
	sprintf(DikName[9]
		,"8");
	sprintf(DikName[10]
		,"9");
	sprintf(DikName[11]
		,"0");
	sprintf(DikName[12]
		,"-");
	sprintf(DikName[13]
		,"=");
	sprintf(DikName[14]
		,"BACKSPACE");
	sprintf(DikName[15]
		,"TAB");
	sprintf(DikName[16]
		,"Q");
	sprintf(DikName[17]
		,"W");
	sprintf(DikName[18]
		,"E");
	sprintf(DikName[19]
		,"R");
	sprintf(DikName[20]
		,"T");
	sprintf(DikName[21]
		,"Y");
	sprintf(DikName[22]
		,"U");
	sprintf(DikName[23]
		,"I");
	sprintf(DikName[24]
		,"O");
	sprintf(DikName[25]
		,"P");
	sprintf(DikName[26]
		,"[");
	sprintf(DikName[27]
		,"]");
	sprintf(DikName[28]
		,"RETURN");
	sprintf(DikName[29]
		,"LEFT CTRL");
	sprintf(DikName[30]
		,"A");
	sprintf(DikName[31]
		,"S");
	sprintf(DikName[32]
		,"D");
	sprintf(DikName[33]
		,"F");
	sprintf(DikName[34]
		,"G");
	sprintf(DikName[35]
		,"H");
	sprintf(DikName[36]
		,"J");
	sprintf(DikName[37]
		,"K");
	sprintf(DikName[38]
		,"L");
	sprintf(DikName[39]
		,";");
	sprintf(DikName[40]
		,"'");
	sprintf(DikName[41]
		,"�");
	sprintf(DikName[42]
		,"LEFT SHIFT");
	sprintf(DikName[43]
		,"\\");
	sprintf(DikName[44]
		,"Z");
	sprintf(DikName[45]
		,"X");
	sprintf(DikName[46]
		,"C");
	sprintf(DikName[47]
		,"V");
	sprintf(DikName[48]
		,"B");
	sprintf(DikName[49]
		,"N");
	sprintf(DikName[50]
		,"M");
	sprintf(DikName[51]
		,",");
	sprintf(DikName[52]
		,".");
	sprintf(DikName[53]
		,"/");
	sprintf(DikName[54]
		,"RIGHT SHIFT");
	sprintf(DikName[55]
		,"*");
	sprintf(DikName[56]
		,"ALT");
	sprintf(DikName[57]
		,"SPACE");
	sprintf(DikName[58]
		,"CAPS LOCK");
	sprintf(DikName[59]
		,"F1");
	sprintf(DikName[60]
		,"F2");
	sprintf(DikName[61]
		,"F3");
	sprintf(DikName[62]
		,"F4");
	sprintf(DikName[63]
		,"F5");
	sprintf(DikName[64]
		,"F6");
	sprintf(DikName[65]
		,"F7");
	sprintf(DikName[66]
		,"F8");
	sprintf(DikName[67]
		,"F9");
	sprintf(DikName[68]
		,"F10");
	sprintf(DikName[69]
		,"NUMLOCK");
	sprintf(DikName[70]
		,"SCROLL");
	sprintf(DikName[71]
		,"NUMPAD 7");
	sprintf(DikName[72]
		,"NUMPAD 8");
	sprintf(DikName[73]
		,"NUMPAD 9");
	sprintf(DikName[74]
		,"NUMPAD -");
	sprintf(DikName[75]
		,"NUMPAD 4");
	sprintf(DikName[76]
		,"NUMPAD 5");
	sprintf(DikName[77]
		,"NUMPAD 6");
	sprintf(DikName[78]
		,"NUMPAD +");
	sprintf(DikName[79]
		,"NUMPAD 1");
	sprintf(DikName[80]
		,"NUMPAD 2");
	sprintf(DikName[81]
		,"NUMPAD 3");
	sprintf(DikName[82]
		,"NUMPAD 0");
	sprintf(DikName[83]
		,"DECIMAL");
	sprintf(DikName[86]
		,">");
	sprintf(DikName[87]
		,"F11");
	sprintf(DikName[88]
		,"F12");
	sprintf(DikName[100]
		,"F13");
	sprintf(DikName[101]
		,"F14");
	sprintf(DikName[102]
		,"F15");
	sprintf(DikName[112]
		,"KANA");
	sprintf(DikName[115]
		,"ABNT_C1");
	sprintf(DikName[121]
		,"CONVERT");
	sprintf(DikName[123]
		,"NOCONVERT");
	sprintf(DikName[125]
		,"YEN");
	sprintf(DikName[126]
		,"ABNT_C2");
	sprintf(DikName[141]
		,"NUMPAD EQUALS");
	sprintf(DikName[144]
		,"PREVTRACK");
	sprintf(DikName[145]
		,"AT");
	sprintf(DikName[146]
		,":");
	sprintf(DikName[147]
		,"__");
	sprintf(DikName[148]
		,"KANJI");
	sprintf(DikName[149]
		,"STOP");
	sprintf(DikName[150]
		,"AX");
	sprintf(DikName[151]
		,"UNLABELED");
	sprintf(DikName[153]
		,"NEXTTRACK");
	sprintf(DikName[156]
		,"NUMPAD ENTER");
	sprintf(DikName[157]
		,"RIGHT CTRL");
	sprintf(DikName[160]
		,"MUTE");
	sprintf(DikName[161]
		,"CALCULATOR");
	sprintf(DikName[162]
		,"PLAYPAUSE");
	sprintf(DikName[164]
		,"MEDIASTOP");
	sprintf(DikName[174]
		,"VOLUMEDOWN");
	sprintf(DikName[176]
		,"VOLUMEUP");
	sprintf(DikName[178]
		,"WEBHOME");
	sprintf(DikName[179]
		,"NUMPAD ,");
	sprintf(DikName[181]
		,"DIVIDE");
	sprintf(DikName[183]
		,"SYS RQ");
	sprintf(DikName[184]
		,"ALT GR");
	sprintf(DikName[197]
		,"PAUSE");
	sprintf(DikName[199]
		,"HOME");
	sprintf(DikName[200]
		,"UP");
	sprintf(DikName[201]
		,"PAGE UP");
	sprintf(DikName[203]
		,"LEFT");
	sprintf(DikName[205]
		,"RIGHT");
	sprintf(DikName[207]
		,"END");
	sprintf(DikName[208]
		,"DOWN");
	sprintf(DikName[209]
		,"PAGE DOWN");
	sprintf(DikName[210]
		,"INSERT");
	sprintf(DikName[211]
		,"DELETE");
	sprintf(DikName[219]
		,"LEFT WIN");
	sprintf(DikName[220]
		,"RIGHT WIN");
	sprintf(DikName[221]
		,"START");
	sprintf(DikName[222]
		,"POWER");
	sprintf(DikName[223]
		,"SLEEP");
	sprintf(DikName[227]
		,"WAKE");
	sprintf(DikName[229]
		,"WEBSEARCH");
	sprintf(DikName[230]
		,"WEBFAVORITES");
	sprintf(DikName[231]
		,"WEBREFRESH");
	sprintf(DikName[232]
		,"WEBSTOP");
	sprintf(DikName[233]
		,"WEBFORWARD");
	sprintf(DikName[234]
		,"WEBBACK");
	sprintf(DikName[235]
		,"MYCOMPUTER");
	sprintf(DikName[236]
		,"MAIL");


	sprintf(DikName[288]
		,"Digital Right");
	sprintf(DikName[289]
		,"Digital Left");
	sprintf(DikName[290]
		,"Digital Down");
	sprintf(DikName[291]
		,"Digital Up");
	sprintf(DikName[292]
		,"Z-Axis +");
	sprintf(DikName[293]
		,"Z-Axis -");
	sprintf(DikName[294]
		,"Joystick Right");
	sprintf(DikName[295]
		,"Joystick Left");
	sprintf(DikName[296]
		,"Joystick Up");
	sprintf(DikName[297]
		,"Joystick Down");
	sprintf(DikName[298]
		,"X-Rotation +");
	sprintf(DikName[299]
		,"X-Rotation -");
	sprintf(DikName[300]
		,"Y-Rotation +");
	sprintf(DikName[301]
		,"Y-Rotation -");
	sprintf(DikName[302]
		,"Z-Rotation +");
	sprintf(DikName[303]
		,"Z-Rotation -");

	for(int a=0;a<32;a++){
		sprintf(DikName[256+a],"Button %i",a+1);
	}
	for(int c=0;c<KeysPerJoy;c++){
		strcpy(DikName[JoyKeyStart+KeysPerJoy+c],DikName[JoyKeyStart+c]);
	}

	for(int w=0;w<MaxKeys;w++){
		DikCharName[w]=0;
	}

	DikCharName[2]
		='1';
	DikCharName[3]
		='2';
	DikCharName[4]
		='3';
	DikCharName[5]
		='4';
	DikCharName[6]
		='5';
	DikCharName[7]
		='6';
	DikCharName[8]
		='7';
	DikCharName[9]
		='8';
	DikCharName[10]
		='9';
	DikCharName[11]
		='0';
	DikCharName[12]
		='-';
	DikCharName[13]
		='=';
	DikCharName[16]
		='Q';
	DikCharName[17]
		='W';
	DikCharName[18]
		='E';
	DikCharName[19]
		='R';
	DikCharName[20]
		='T';
	DikCharName[21]
		='Y';
	DikCharName[22]
		='U';
	DikCharName[23]
		='I';
	DikCharName[24]
		='O';
	DikCharName[25]
		='P';
	DikCharName[26]
		='[';
	DikCharName[27]
		=']';
	DikCharName[30]
		='A';
	DikCharName[31]
		='S';
	DikCharName[32]
		='D';
	DikCharName[33]
		='F';
	DikCharName[34]
		='G';
	DikCharName[35]
		='H';
	DikCharName[36]
		='J';
	DikCharName[37]
		='K';
	DikCharName[38]
		='L';
	DikCharName[39]
		=';';
	DikCharName[40]
		='\'';
	DikCharName[41]
		='�';
	DikCharName[44]
		='Z';
	DikCharName[45]
		='X';
	DikCharName[46]
		='C';
	DikCharName[47]
		='V';
	DikCharName[48]
		='B';
	DikCharName[49]
		='N';
	DikCharName[50]
		='M';
	DikCharName[51]
		=',';
	DikCharName[52]
		='.';
	DikCharName[53]
		='/';
	DikCharName[57]
		=' ';
	DikCharName[71]
		='7';
	DikCharName[72]
		='8';
	DikCharName[73]
		='9';
	DikCharName[74]
		='-';
	DikCharName[75]
		='4';
	DikCharName[76]
		='5';
	DikCharName[77]
		='6';
	DikCharName[78]
		='+';
	DikCharName[79]
		='1';
	DikCharName[80]
		='2';
	DikCharName[81]
		='3';
	DikCharName[82]
		='0';
	DikCharName[83]
		=',';
	DikCharName[86]
		='>';
	DikCharName[146]
		=':';
	DikCharName[147]
		='_';


}