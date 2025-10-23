

#ifndef __InfoArea__
#define __InfoArea__


class InfoArea
{
private:

	void (*BlitFunction) ();

public:

	InfoArea(int x, int y, int w, int h, void (*iBlitFunction));
	~InfoArea();

	void BlitArea(int xAdj, int yAdj);

};

#endif 