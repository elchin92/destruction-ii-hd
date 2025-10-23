#include <fstream>
#include <stdio.h>
#include <string.h>

using std::ifstream;
using std::ofstream;

#define NAME  0
#define CODE  1
#define WASTE 2
#define WCODE 3

int main(){

	ifstream File("DIK_CODES.txt");
	ofstream oFile("DIK_CODES.cpp");
	char Str[20],ch,OutString[50];
	int Counter=0;
	int Code;
	int Mode=NAME;

	while(File.get(ch)){
	
		if(Mode==NAME&&ch!=' '){
			Str[Counter]=ch;
			Counter++;
		}

		if(Mode==NAME&&ch==' '){
			Mode=WCODE;
			Str[Counter]='\0';
			Counter=0;
			Code=0;
		}

		if(Mode==CODE&&(ch==' '||ch=='\n'))
			Mode=WASTE;

		if(Mode==CODE&&ch!=' '){
			Code*=16;
			if(ch>=48&&ch<=57)
				Code+=ch-48;
			else
				Code+=ch-55;
			
		}
			


		if(Mode==WCODE&&ch=='x')
			Mode=CODE;
		if(Mode==WASTE&&ch=='\n'){
			sprintf(OutString,"if(CharCode==%i)%c	return \"%s\";%c",Code,'\n',Str,'\n');
			oFile << OutString;
			Mode=NAME;
		}
	}

	return 0;
}