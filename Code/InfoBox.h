// #include <ddraw.h>  // Заменено на SDL2_DirectDrawCompat.h через /FI (forced include)

#ifndef __InfoBox__
#define __InfoBox__



class InfoBox
{
private:

	int x,y,w,h, xAdj, yAdj, txAdj,tyAdj;
	HFONT * TextFont;
	COLORREF TextColor;
	char IntText[20];
	int TextAlign;

public:

	InfoBox(int ix, int iy, int iw, int ih, 
		    HFONT * iTextFont, COLORREF iTextColor, int iTextAlign=TA_LEFT);
	~InfoBox();

	int GetTextX();
	int GetTextY();
	int GetX();
	int GetY();
	int GetW();
	int GetH();

	int BlankAndPrepareBox(int ixAdj, int iyAdj=0);
	void CloseBox();
	int OpenBox();

	void SetInfoBoxTextColor(COLORREF iTextColor);
	void SetInfoBoxTextFont(HFONT * iTextFont);
	void SetInfoBoxTextAlign(int iTextAlign);

	// Box must be open when you call these!
	void SetTemporaryInfoBoxTextColor(COLORREF iTextColor);
	void SetTemporaryInfoBoxTextFont(HFONT * iTextFont);
	void SetTemporaryInfoBoxTextAlign(int iTextAlign);

	void Print(int xRel, int yRel, const char * text);  // C++20: string literals are const char*
	void Print(int xRel, int yRel, int text);

};

extern char tstr[100];
#endif 