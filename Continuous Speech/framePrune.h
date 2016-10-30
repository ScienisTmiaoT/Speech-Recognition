#ifndef FRAME_PRUNE
#define FRAME_PRUNE

#include <iostream>
using namespace std;

typedef short SAMPLE;
#define SAMPLE_PER_FRAME        (882)    // in order to make each frame 20ms
#define FORGET_FACTOR   (1)
#define ADJUSTMENT      (0.05)
#define THRESHOLD_F           (10)
#define SPEAKTHRESHOLD      (5)
#define SILENCETHRESHOLD_F    (5)
#define FRAME_IGNORE        (4)
#define FRAME_TO_BACKGROUND (5)

double EnergyPerSampleInDecibelf(SAMPLE *audioframe, long framesToCalc);
bool classifyStartFramef(SAMPLE *audioframe, long framesToCalc, double& backgroundOf10f, double& levelStart);
bool classifyEndFramef(SAMPLE *audioframe, long framesToCalc, double& backgroundOf10f, double& levelEnd);
int pruneFrame(short* dataWave, int& numSamples);










#endif

