#ifndef SOUNDPRIORITYENUM
#define SOUNDPRIORITYENUM
enum SoundPriority{spLowest,spLow,spNormal,spHigh,spCritical};
#define NumberOfPriorityLevels (int)spCritical
enum DistanceType{dWhisper,dClose,dNormal,dFar,dInfinite};
#define NumberOfDistances ((int)dInfinite+1)
const int Distances[NumberOfDistances][2]={
	//	MIN		MAX
		  8,	 80,	// Whisper
		 40,	250,	// Close
		100,   2000,	// Normal
		500,  15000,	// Far
		  0,	  0		// Doesn't matter, Infinite means regular sound
};

#define DIST_MIN	Distances[(int)TheDistance][0]
#define DIST_MAX	Distances[(int)TheDistance][1]
#define DIST_DIFMAX	(DIST_MAX-DIST_MIN)

#endif