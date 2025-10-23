#include "ActiveMenuItem.h"

#ifndef __TextBoxMenuItem__ 
#define __TextBoxMenuItem__

class Menu;

class TextBoxMenuItem : public ActiveMenuItem
{
private:

	char * istr;
	int chars;
	int CapsLock, RegularStyle, lSelected;
	__int64 EditStart;

	void InternalUpdateInput();

	InfoBox * InputBox;
	HFONT InputFont;
public:

	TextBoxMenuItem(MenuStyle * iTheMenuStyle, const char * iistr); // C++20: const char*
	TextBoxMenuItem(int x, int y, int w, int h, int fs, int fw, const char * iistr); // C++20: const char*
	~TextBoxMenuItem();

	void InternalBlit(int Selected);

	void AddChar(int dik);
	void EraseChar();

	void SetStr(char * iistr);
};
#endif