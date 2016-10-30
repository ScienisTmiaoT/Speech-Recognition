#include "framePrune.h"

double EnergyPerSampleInDecibelf(SAMPLE *audioframe, long framesToCalc)
{
	double sum = 0;
	double decibel = 0;
	double pointValue = 0; // the value of each point of the frame

	for (int i = 0; i < framesToCalc; i++) {
		pointValue = *audioframe;
		sum += pow(pointValue, 2);
		audioframe++;
	}

	decibel = 10 * log10(sum);

	return decibel;
}

bool classifyStartFramef(SAMPLE *audioframe, long framesToCalc, double& backgroundOf10f, double& levelStart)
{
	bool isSpeech = false;
	double current = EnergyPerSampleInDecibelf(audioframe, framesToCalc);
	double background = backgroundOf10f / FRAME_TO_BACKGROUND;

	levelStart = ((levelStart * FORGET_FACTOR) + current) / (FORGET_FACTOR + 1);

	//    if (flag) {
	//        cout << "backgroundORIRIDIDID " << background << endl;
	//    }


	if (current < background) {
		background = current;
	}
	else {
		background += (current - background) * ADJUSTMENT;
	}

	if (levelStart < background) {
		levelStart = background;
	}
	if (levelStart - background > THRESHOLD_F) {
		isSpeech = true;
	}
	//    if (flag) {
	//        cout << "current: " << current << endl;
	//        cout << "level: " << levelStart << endl;
	//        cout << "background: " << background << endl;
	//    }
	return isSpeech;
}


bool classifyEndFramef(SAMPLE *audioframe, long framesToCalc, double& backgroundOf10f, double& levelEnd)
{
	bool isSpeech = false;
	double current = EnergyPerSampleInDecibelf(audioframe, framesToCalc);
	double background = backgroundOf10f / FRAME_TO_BACKGROUND;

	levelEnd = ((levelEnd * FORGET_FACTOR) + current) / (FORGET_FACTOR + 1);

	if (current < background) {
		background = current;
	}
	else {
		background += (current - background) * ADJUSTMENT;
	}

	if (levelEnd < background) {
		levelEnd = background;
	}
	if (levelEnd - background > THRESHOLD_F) {
		isSpeech = true;
	}

	return isSpeech;
}


int pruneFrame(short* dataWave, int& numSamples) {
	double backgroundOf10f = 0;
	double levelStart = 0;
	double levelEnd = 0;
	int start = 0;
	int end = 0;
	bool startFlag = true;
	bool endFlag = true;

	int continueSpeakTimef = 0;
	int continueSilenceTimef = 0;

	for (int i = FRAME_IGNORE * SAMPLE_PER_FRAME; i < numSamples; i += SAMPLE_PER_FRAME) {
		if (i < (FRAME_IGNORE + FRAME_TO_BACKGROUND) * SAMPLE_PER_FRAME) {
			double current = EnergyPerSampleInDecibelf(dataWave + i, SAMPLE_PER_FRAME);
			backgroundOf10f += current;
		}
		else {
			if (classifyStartFramef(dataWave + i, SAMPLE_PER_FRAME, backgroundOf10f, levelStart)) {
				if (startFlag) {
					continueSpeakTimef++;
				}
			}
			else {
				if (startFlag) {
					continueSpeakTimef = 0;
				}
			}
			if (classifyEndFramef(dataWave + numSamples - i + (FRAME_IGNORE + FRAME_TO_BACKGROUND - 1) * SAMPLE_PER_FRAME, SAMPLE_PER_FRAME, backgroundOf10f, levelEnd)) {
				if (endFlag) {
					continueSilenceTimef++;
				}
			}
			else {
				if (endFlag) {
					continueSilenceTimef = 0;
				}
			}

		}

		if (continueSpeakTimef > SPEAKTHRESHOLD && startFlag) {
			start = i - SPEAKTHRESHOLD * SAMPLE_PER_FRAME;
			startFlag = false;
		}
		if (continueSilenceTimef > SILENCETHRESHOLD_F && endFlag) {
			end = numSamples - i + (FRAME_IGNORE + FRAME_TO_BACKGROUND + SILENCETHRESHOLD_F) * SAMPLE_PER_FRAME;
			endFlag = false;
		}
	}

	numSamples = end - start;
	//    dataWave += start;
	cout << "END    " << end << endl;
	cout << "START   " << start << endl;
	return start;
}