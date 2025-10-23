#ifndef __TextMenuItem__ 
#define __TextMenuItem__



class MenuStyle;

class TextMenuItem
{
protected:

	MenuStyle * TheMenuStyle;
	int XAdjust; // For different text alignments

	const char * Title;

public:

	TextMenuItem * nextTextMenuItem;

	virtual ~TextMenuItem();

	virtual int IsBorder();
	virtual int IsEndBorder();
	virtual int IsActive()=0;
	virtual int IsOption();
	void Initialize();
	virtual void Blit(int Selected) = 0;
};
#endif

