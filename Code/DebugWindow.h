#ifndef __DebugWindow__ 
#define __DebugWindow__



enum channel {ErrorChannel, Channel1, Channel2, Channel3, noOfChannels};

const int MaxMonitors=20;
const int fh=15;

class Game;
class DebugMonitor;

class DebugWindow
{
private:

	int x,y,width,height,borderwidth;
	int MonitorCount;

	int VISIBLE;

	void AddMonitor(DebugMonitor * dm);
	int getNextX();
	int getNextY();
	int getMaxWidth();


	DebugMonitor * theChannels[(int)noOfChannels];
public:

	char theMessages[(int)noOfChannels][40];
	DebugMonitor * TheMonitors;
	DebugMonitor * TheChannels[(int)noOfChannels];

	DebugWindow();
	~DebugWindow();

	void Blit();

	void AddMonitor(char * name, int * p);
	void AddMonitor(char * name, char * p);
	void AddMonitor(char * name, double * p);

	void Broadcast(channel chan, char * msg);
	void Broadcast(channel chan, int msg); 
	void Broadcast(channel chan, double msg); 
	void Toggle();


};
#endif

