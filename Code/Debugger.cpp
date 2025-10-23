#include "Debugger.h"
#include <fstream>
#include "Game.h"

using std::ofstream;
ofstream * BeaconFile;
#ifdef DEBUGGING
	DebugWindow * dw;
#endif

	#ifdef BETATESTING

void Beacon(int Number){


#ifdef TIMEKEEPING
	if(TheTimeKeeper)
		TheTimeKeeper->EnterArea(taBeacon);
#endif



	if(Number==-9876){
			DeleteFile("Beacon.txt");
			BeaconFile = new ofstream("Beacon.txt", ios::app); // C++20: removed filebuf::openprot
	}


	(*BeaconFile) << Number << endl;

#ifdef TIMEKEEPING
	if(TheTimeKeeper)
	TheTimeKeeper->LeaveArea();
#endif

}

void BeaconRect(){

	DP("BeaconRect:");
	DP2("left",TheGame->rcRect.left);
	DP2("top",TheGame->rcRect.top);
	DP2("right",TheGame->rcRect.right);
	DP2("bottom",TheGame->rcRect.bottom);


}

void DP(const char * output){
		(*BeaconFile) << output << endl;
}
void DP2(const char * output, int i){
		(*BeaconFile) << output << " " << i << endl;
}

void CloseBeaconFile(){
	(*BeaconFile).close();
}
	#endif