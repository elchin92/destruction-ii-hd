#include "BorderEndMenuItem.h"
#include "BorderMenuItem.h"
#include "Game.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BorderEndMenuItem::BorderEndMenuItem(BorderMenuItem * iTheBorder){
	TheBorder=iTheBorder;
}

BorderEndMenuItem::~BorderEndMenuItem()
{

}

void BorderEndMenuItem::Blit(int Selected){

}

int BorderEndMenuItem::IsBorder(){return 1;}
int BorderEndMenuItem::IsEndBorder(){return 1;}
void BorderEndMenuItem::SetBottomRightCorner(int ix, int iy){

	TheBorder->SetEndPos(ix,iy);
}
