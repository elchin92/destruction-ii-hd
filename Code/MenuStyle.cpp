
#include "MenuStyle.h"
#include "Menu.h"
#include "Game.h"
#include "BorderEndMenuItem.h"
#include "ActiveMenuItem.h"
#include "Debugger.h"

#define UsingSisterStyle (SisterStyle!=NULL)

// Settings Menus

#define stdUnselectedColor	RGB(96,0,159)
#define stdSelectedColor	RGB(154,0,229)
#define stdBorderColor		RGB(0,0,170)
#define stdDiscreteColor		RGB(0,0,200)
#define SetStandardColors() UnselectedColor = stdUnselectedColor;SelectedColor = stdSelectedColor;BorderColor = stdBorderColor;DiscreteColor = stdDiscreteColor;


MenuStyle::MenuStyle(Menu * iOwnerMenu, MenuStyle * iSisterStyle){

	OwnerMenu=iOwnerMenu;
	MenuFont = NULL;
	MarkedPen = NULL;
	BorderPen = NULL;
	MarkedBrush = NULL;
	BorderBrush = NULL;
	MarkedColor=0;
	BorderColor=0;


	MenuAlign=TA_LEFT;
	SlideAdjust=0;
	SisterStyle=NULL;

	for(int d=0;d<2;d++){
		ItemDimension[d]=0;
		NoItemToItemDistance[d]=0;
		ItemToItemDistance[d]=0;
		ItemToEdgeDistance[d]=0;
		MarkedEdgeToTextDistance[d]=0;

	}

	BorderThickness=0; Shadow=0;BorderRound=0;
	EndBorderToNextItemDistance=0;
	XAdjust=0;
	
	if(iSisterStyle){
		SisterStyle=iSisterStyle;
		BorderThickness=SisterStyle->GetBorderThickness();
		BorderRound=SisterStyle->GetBorderRound();
		for(int i=0;i<2;i++){
			ItemToEdgeDistance[i]=SisterStyle->GetItemToEdgeDistance(i);
			MarkedEdgeToTextDistance[i]=SisterStyle->GetMETTD(i);
			MarkedRound[i]=SisterStyle->GetMarkedRound(i);
		}
	}

}

MenuStyle::~MenuStyle(){
	DeleteObject(MenuFont);
	DeleteObject(MarkedPen);  DeleteObject(BorderPen);
	DeleteObject(MarkedBrush); DeleteObject(BorderBrush);
}


int MenuStyle::GetBorderThickness(){
	return BorderThickness;
}

int MenuStyle::GetBorderRound(){
	return BorderRound;
}

int MenuStyle::GetMarkedRound(int i){
	return MarkedRound[i];
}

int MenuStyle::GetItemToEdgeDistance(int i){
	return ItemToEdgeDistance[i];
}
int MenuStyle::GetMETTD(int i){
	return MarkedEdgeToTextDistance[i];
}

void MenuStyle::CreateBorderItems(){

	if(!BorderColor)
		return;

	LOGBRUSH RealBrush;

	DeleteObject(BorderPen);
	DeleteObject(BorderBrush);

	BorderPen = CreatePen(PS_SOLID,BorderThickness,BorderColor);
	RealBrush.lbStyle=BS_HOLLOW;
	BorderBrush = CreateBrushIndirect(&RealBrush);	
		
}
void MenuStyle::CreateMarkedItems(){

	if(!MarkedColor)
		return;

	DeleteObject(MarkedPen);  
	DeleteObject(MarkedBrush);

	LOGBRUSH RealBrush;

	Shadow=FALSE;
	MarkedPen = CreatePen(PS_SOLID,1,MarkedColor);
	RealBrush.lbStyle=BS_SOLID;
	RealBrush.lbColor=MarkedColor;
	MarkedBrush = CreateBrushIndirect(&RealBrush);	

}

void MenuStyle::SetItemPositions(){

	int X = OwnerMenu->GetXPos() , 
		Y = OwnerMenu->GetYPos();

	DP2("ownmenu x",OwnerMenu->GetXPos());

	int XPlus=0;
	int XBorderToFirstItem=0;
	if(BorderColor)
		XBorderToFirstItem=BorderThickness+ItemToEdgeDistance[0]+MarkedEdgeToTextDistance[0];

	MenuItem * ItemToPlace = OwnerMenu->GetMenuItems();

	for(int i=0,nbi=0;ItemToPlace;ItemToPlace=ItemToPlace->nextMenuItem,i++){
	
		if(!ItemToPlace->IsBorder()){
			XPlus+=MarkedEdgeToTextDistance[0];
			if(!(i%Cols))
				Y+=MarkedEdgeToTextDistance[1];

			if(ItemToPlace->GetFY())Y=ItemToPlace->GetFY();

			DP2("Setting Pos X",X);
			DP2("Setting Pos XB",XBorderToFirstItem);
			DP2("Setting Pos XPlus",XPlus);
			DP2("Setting Pos XAdjust",XAdjust);
			ItemToPlace->SetPos(X+XBorderToFirstItem+XPlus+XAdjust, Y);

			if((nbi+1)%Cols)
				XPlus+=ItemDimension[0]+ItemToItemDistance[0]+MarkedEdgeToTextDistance[0];
			else{
				XPlus=0;
				Y+=ItemDimension[1]+ItemToItemDistance[1]+MarkedEdgeToTextDistance[1];
			}
			nbi++;

		}else{
			i=0;

			if(ItemToPlace->IsEndBorder()){
				Y+=ItemToEdgeDistance[1]+MarkedEdgeToTextDistance[1];
				int XWidth;
				if(UsingSisterStyle)
					XWidth=SisterStyle->GetTotalWidth();
				else
					XWidth=GetTotalWidth();
				if(ItemToPlace->GetFY())Y=ItemToPlace->GetFY();
				((BorderEndMenuItem *)ItemToPlace)->SetBottomRightCorner(X+XWidth+BorderThickness,Y+BorderThickness);
				Y+=EndBorderToNextItemDistance;
			
			}else{
				if(ItemToPlace->GetFY())Y=ItemToPlace->GetFY();
				ItemToPlace->SetPos(X,Y);
				Y+=ItemToEdgeDistance[1];
			}
			Y+= BorderThickness+MarkedEdgeToTextDistance[1];

			XPlus=0;
		}

	}

	TotalHeight=Y;

}




void MenuStyle::CalculateMenuDimensions(int XPlay, int YPlay, int LockedCols, int LockedFontSize){

	if(LockedCols)
		Cols=LockedCols;

	Beacon(200624);

	int Original[2]={XPlay,YPlay};


	// Får inte överstiga xplay!!!!! måset kunna skriva <== ??
	if(BorderColor){


		if(!UsingSisterStyle){
			ItemToEdgeDistance[0]=XPlay/30;
			ItemToEdgeDistance[1]=YPlay/70;
			BorderThickness=min(XPlay, YPlay)/(45*Cols);
			BorderRound=min(XPlay,YPlay)/25;
		}
		EndBorderToNextItemDistance=2*BorderThickness;
	}
	if(MarkedColor){
		if(!UsingSisterStyle){
			MarkedEdgeToTextDistance[0]=XPlay/25;
			MarkedEdgeToTextDistance[1]=MarkedEdgeToTextDistance[0]/6;
			MarkedRound[0]=XPlay/10;
			MarkedRound[1]=YPlay/5;
			//ItemToEdgeDistance[0]=0;
			//ItemToEdgeDistance[1]=0;
		}
	}



	int NumberOfMenuItems=0;
	int Borders=0;
	int LastItemIsABorder = FALSE;

	for(MenuItem * t = OwnerMenu->GetMenuItems();t;t=t->nextMenuItem){
		if(t->IsBorder()){
			Borders++;
			YPlay-=BorderThickness;
			if(t->IsEndBorder()){
				if(t->nextMenuItem)
					YPlay-=EndBorderToNextItemDistance;
				else
					LastItemIsABorder=TRUE;
			}

		}else{
			NumberOfMenuItems++;
		}
	}


	int Rows = (NumberOfMenuItems-1)/Cols+1;



	XPlay-=Cols*(2*MarkedEdgeToTextDistance[0]) + 2*ItemToEdgeDistance[0] - 2*BorderThickness;
	YPlay-=Rows*(2*MarkedEdgeToTextDistance[1]) + 2*ItemToEdgeDistance[1];


	if(!NoItemToItemDistance[0])
		ItemToItemDistance[0]=XPlay/(11*Cols);
	if(!NoItemToItemDistance[1])
		ItemToItemDistance[1]=YPlay/(11*Rows);	

	XPlay-=ItemToItemDistance[0]*(Cols -1);
	YPlay-=ItemToItemDistance[1]*(Rows -1);

	ItemDimension[0]=XPlay/Cols;
	ItemDimension[1]=YPlay/Rows;
	Beacon(260621);

	if(LockedFontSize)
		ItemDimension[1]=LockedFontSize;

	if(Shadow)
		Shadow=1+ItemDimension[1]/25;
	Beacon(260622);
	if(UsingSisterStyle){
			Beacon(260623);
		while(SisterStyle->GetTotalWidth()>GetTotalWidth())
			ItemDimension[0]--;
	}


	if(GetTotalHeight(NumberOfItems, Borders, Rows, LastItemIsABorder )>Original[1] ||
		ItemDimension[1]<12){

		if(!LockedCols&&!(Cols>=NumberOfItems)){
			Cols++;
			CalculateMenuDimensions(Original[0],Original[1], LockedCols, LockedFontSize) ;
				Beacon(260625);
		}else{
				Beacon(260626);
			while(GetTotalHeight(NumberOfItems, Borders, Rows, LastItemIsABorder )>Original[1]){
					Beacon(260627);
				ItemDimension[1]--;
			}
		}

	}

	if(MenuAlign==TA_CENTER)
		XAdjust=ItemDimension[0]/2;
	else if(MenuAlign==TA_RIGHT)
		XAdjust=ItemDimension[0];

}

int MenuStyle::GetTotalHeight(int Items,int Borders, int Rows, int LastItemIsABorder){
	return 
		Rows*(2*MarkedEdgeToTextDistance[1]+ItemToItemDistance[1]+ItemDimension[1])-ItemToItemDistance[1] +
		Borders*(BorderThickness+ItemToEdgeDistance[1])+(Borders/2-LastItemIsABorder)*EndBorderToNextItemDistance;
}
int MenuStyle::GetTotalWidth(){
	return 
		Cols*(2*MarkedEdgeToTextDistance[0]+ItemToItemDistance[0]+ItemDimension[0])-ItemToItemDistance[0] +
		2*(BorderThickness+ItemToEdgeDistance[0]);
}
void MenuStyle::CreatePredefinedStyle(int Width, int Height, PredefinedStyle pdm){

	BorderColor=RGB(0,0,0);
	MarkedColor=RGB(0,0,0);

	Cols=1;

	MenuAlign=TA_LEFT; 
	int FontWeight=FW_NORMAL;

	int LockedFontSize=0, LockedCols=0;
	if(pdm==pLoadSave){
		// Load & Save

		MenuAlign=TA_CENTER;
		DP2("Load save menualign", MenuAlign==TA_CENTER);

		// Always set these
		UnselectedColor = RGB(50,0,200);
		SelectedColor = RGB(120,0,200);

		// Set this for Shadowed Menus
		Shadow=FALSE;

		// Set this for marked menus
		MarkedColor = RGB(40,0,80);


	}else if(pdm==pSettings){

		// Settings
		MenuAlign=TA_CENTER;
		UnselectedColor = stdUnselectedColor;
		SelectedColor = stdSelectedColor;
		DiscreteColor = stdDiscreteColor;

		LockedFontSize=24;
		LockedCols=2;
	
	}else if(pdm==pControls){
		// Always set these
		Beacon(25529);
		LockedCols=1;
		LockedFontSize=21;
		FontWeight=FW_BOLD;
		UnselectedColor = RGB(0,136,0);
		SelectedColor = RGB(0,255,0);

	}else if(pdm==pGameSetup){
		LockedFontSize=22;
		SetStandardColors();
		LockedCols=2;
	
	}else if(pdm==pEnemySetup){

		LockedFontSize=15;

		UnselectedColor = stdUnselectedColor;
		SelectedColor = stdSelectedColor;
		DiscreteColor = RGB(0,0,200);
		MenuAlign=TA_CENTER;
		FontWeight=FW_REGULAR;
		LockedCols=5;
	}else if(pdm==pPlayerSetup){
		Beacon(8900001);
		LockedFontSize=24;

		NoItemToItemDistance[1]=TRUE;
		UnselectedColor = RGB(195,195,195);
		SelectedColor = RGB(255,255,255);
		DiscreteColor = RGB(195,195,195);
		MarkedColor = RGB(53,0,93);
		MenuAlign=TA_CENTER;
		FontWeight=FW_BOLD;
		LockedCols=1;
	}
	Beacon(200623);
	DP2("lCols",LockedCols);
	CalculateMenuDimensions(Width,Height,LockedCols, LockedFontSize);
	if(pdm==pLoadSave){
		if(Cols==1 && ItemDimension[1]>55)
			ItemDimension[1]=55;
		if(ItemDimension[1]*8>ItemDimension[0])
			ItemDimension[1]=ItemDimension[0]/8;
		if(ItemDimension[1]<12)
			ItemDimension[1]=12;
	}
	Beacon(200888);
	DP2("FontSize",ItemDimension[1]);
	DP2("Cols",Cols);
	DP2("ItemToEdgeDistance y",ItemToEdgeDistance[1]);
	DP2("MarkedEdgeToTextDistance y",MarkedEdgeToTextDistance[1]);
//	Beacon(" y",[1]);
//	Beacon(" y",[1]);
	Beacon(200624);
	CreateBorderItems();
	Beacon(200625);
	CreateMarkedItems();
	Beacon(200626);

	MenuFont = CreateFont(ItemDimension[1],
						  0, 0, 0, FontWeight, FALSE, FALSE, FALSE,
						  ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						  DEFAULT_QUALITY, VARIABLE_PITCH,"Arial");

	DP2("Cols",Cols);

	if(pdm==pPlayerSetup){
		MarkedRound[0]=30;
		MarkedRound[1]=30;
	}
	Beacon(25523);
	SetItemPositions();
	Beacon(25524);
	DP2("menualign", MenuAlign==TA_CENTER);

}

void MenuStyle::SetBorderTitleStyle(){
	return;
	SetTextColor(TheGame->hdc, cgs->ContrastAdjust(SelectedColor));
	SetBkMode(TheGame->hdc, OPAQUE);
}
void MenuStyle::SetBorderStyle(){
	SetBkMode(TheGame->hdc, TRANSPARENT);
	SelectObject(TheGame->hdc,BorderPen);
	SelectObject(TheGame->hdc,BorderBrush);
}
void MenuStyle::SetMarkedStyle(){
	SetBkMode(TheGame->hdc, OPAQUE);
	SelectObject(TheGame->hdc,MarkedPen);
	SelectObject(TheGame->hdc,MarkedBrush);
}
void MenuStyle::SetSelectedStyle(){SetStyle(TRUE);}
void MenuStyle::SetUnselectedStyle(){SetStyle(FALSE);}
void MenuStyle::SetStyle(int Selected){

	SetTextAttributes();
	if(Selected){
		SetTextColor(TheGame->hdc, cgs->ContrastAdjust(SelectedColor));
	}else{
		SetTextColor(TheGame->hdc, cgs->ContrastAdjust(UnselectedColor));	
	}
}

void MenuStyle::SetDiscreteStyle(){
	SetTextAttributes();
	SetTextColor(TheGame->hdc, cgs->ContrastAdjust(DiscreteColor));

}
void MenuStyle::SetTextAttributes(){
	SetBkMode(TheGame->hdc, TRANSPARENT);
	SelectObject(TheGame->hdc,MenuFont);
	SetTextAlign(TheGame->hdc, MenuAlign );	

}
void MenuStyle::BlitMark(ActiveMenuItem * TheItem, int Selected){

	if(!Selected||!MarkedColor)
		return;

	int X=TheItem->GetX()-XAdjust-MarkedEdgeToTextDistance[0];
	int Y=TheItem->GetY()-MarkedEdgeToTextDistance[1];

	SetMarkedStyle();
	RoundRect(TheGame->hdc,X,Y, X + ItemDimension[0] + 2*MarkedEdgeToTextDistance[0], 
								Y + ItemDimension[1] + 2*MarkedEdgeToTextDistance[1],
								MarkedRound[0], MarkedRound[1]);
}

int MenuStyle::GetColumns(){return Cols;}
int MenuStyle::GetNextMenuYPos(){
	return TotalHeight;
}
int MenuStyle::GetMenuAlign(){return MenuAlign;}
void MenuStyle::SetTemporaryMenuAlign(int ta){	SetTextAlign(TheGame->hdc, ta );	}
void MenuStyle::SetSlideAdjust(int iSlideAdjust){SlideAdjust=iSlideAdjust;}
int MenuStyle::GetSlideAdjust(){return SlideAdjust;}
void MenuStyle::SetSelectKeyAdjust(int iSelectKeyAdjust){SelectKeyAdjust=iSelectKeyAdjust;}
int MenuStyle::GetSelectKeyAdjust(){return SelectKeyAdjust;}	
int MenuStyle::GetXAdjust(){return XAdjust;}