#ifndef FRAME_PRUNE
#define FRAME_PRUNE

#include <iostream>
#include <math.h>
using namespace std;

typedef short SAMPLE;
#define SAMPLE_PER_FRAME        (882)    // in order to make each frame 20ms
#define FORGET_FACTOR   (1)
#define ADJUSTMENT      (0.05)
#define THRESHOLD_F           (13)
#define SPEAKTHRESHOLD      (5)
#define SILENCETHRESHOLD    (7)
#define FRAME_IGNORE        (4)
#define FRAME_TO_BACKGROUND (5)

double EnergyPerSample(SAMPLE *audioframe, long framesToCalc);
bool classifyFrame(SAMPLE *audioframe, long framesToCalc, double& backgroundOf10f, double& levelStart);
int pruneFrameFromStart(short* dataWave, int& numSamples);
int pruneFrameFromEnd(short* dataWave, int& numSamples);










#endif

