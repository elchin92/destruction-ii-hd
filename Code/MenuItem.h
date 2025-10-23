#ifndef __MenuItem__ 
#define __MenuItem__



class MenuStyle;

class MenuItem
{
protected:

	MenuStyle * TheMenuStyle;
	int x,y, fY;
	int XAdjust; // For different text alignments

	const char * Title;

public:

	MenuItem * nextMenuItem;

	virtual ~MenuItem();

	virtual int IsBorder();
	virtual int IsEndBorder();
	virtual int IsActive()=0;
	virtual int IsOption();
	int GetX();
	int GetY();
	int GetFY();
	void Initialize();
	void SetPos(int ix, int iy);
	virtual void Blit(int Selected) = 0;

		// Hack to create Settings Menu
	void SetForcedYPos(int ifY);
};
#endif
