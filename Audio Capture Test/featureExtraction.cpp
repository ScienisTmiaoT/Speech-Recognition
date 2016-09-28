#include "featureExtraction.h"

int frameNum = 0;
#define COMPARE_SCALE (10)
double backgroundOf10 = 0;
double level = 0;
int continueSpeakTime = 0;
int continueSilenceTime = 0;
int frameCount = 0;

double EnergyPerSampleInDecibel(SAMPLE *audioframe, long framesToCalc)
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

bool classifyFrame(SAMPLE *audioframe, long framesToCalc)
{
	bool isSpeech = false;
	double current = EnergyPerSampleInDecibel(audioframe, framesToCalc);
	double background = backgroundOf10 / 10;

	level = ((level * FORGET_FACTOR) + current) / (FORGET_FACTOR + 1);

	if (current < background) {
		background = current;
	}
	else {
		background = (current - background) * ADJUSTMENT;
	}

	if (level < background) {
		level = background;
	}
	if (level - background > THRESHOLD) {
		isSpeech = true;
	}

	if (!isSpeech) {
		continueSilenceTime += 1;
		//        printf("The Continue Silence Frame is %d\n", continueSilenceTime);
	}

	if (isSpeech && continueSilenceTime != 0) {
		printf("The Continue Silence Frame is %d\n", continueSilenceTime);
		continueSilenceTime = 0;
	}

	cout << "current: " << current << endl;
	cout << "level: " << level << endl;
	cout << "background: " << background << endl;

	return isSpeech;

}

/*delete the silent part of audio record at beginning and end,
**then return the offset of the audio beginning address.
**use two flag start and end search the audio data from
**beginning and end respectively. If start detect the audio then
**prepare to judge whether satisfy a long time.
**if end detect audio from end to beginning, then prepare to 
**discard a piece audio
*/
int pruneFrame(short* dataWave, int& numSamples) {
	int start = 0;
	int end = 0;
	bool startFlag = true;
	bool endFlag = true;
	for (int i = FRAME_IGNORE * SAMPLE_PER_FRAME; i < numSamples; i++) {
		if (i < (FRAME_IGNORE + FRAME_TO_BACKGROUND) * SAMPLE_PER_FRAME) {
			if (i == FRAME_IGNORE * SAMPLE_PER_FRAME) {
				level = EnergyPerSampleInDecibel(dataWave + i, SAMPLE_PER_FRAME);
				printf("The first energy is %f \n", level);
			}
			else if(i % SAMPLE_PER_FRAME == 0){
				double currentTemp = EnergyPerSampleInDecibel(dataWave + i, SAMPLE_PER_FRAME);
				level = ((level * FORGET_FACTOR) + currentTemp) / (FORGET_FACTOR + 1);
				backgroundOf10 += EnergyPerSampleInDecibel(dataWave + i, SAMPLE_PER_FRAME);
			}
		}
		else if(i % SAMPLE_PER_FRAME == 0){
			if (classifyFrame(dataWave + i, SAMPLE_PER_FRAME)) {
				if(startFlag)
					continueSpeakTime++;
			}
			else {
				if(startFlag)
					continueSpeakTime = 0;
			}
			if (classifyFrame(dataWave + numSamples - i, SAMPLE_PER_FRAME)) {
				continueSilenceTime++;
			}
			else {
				continueSilenceTime = 0;
			}
		}
		if (continueSpeakTime > SPEAKTHRESHOLD && startFlag) {
			start = i;
			startFlag = false;
		}
		if (continueSilenceTime > SILENCETHRESHOLD && endFlag) {
			end = numSamples - i;
			endFlag = false;
		}
	}
	numSamples = end - start;
	return start;
}

/**

*  do the pre-emphasize   s'[n] = s[n] - alpha * s[n-1]
*
*  @param waveData    input original wave data
*  @param factor      the factor alpha in the formula
*
*  @return            waveData after pre-emphasized
*/
double* preemphasized(short* waveData, double factor, int sampleNum)
{
	//    long sampleNum = sizeof(waveData);

	double* waveDataAfter = new double[sampleNum];

	//    cout << waveData[138008];

	waveDataAfter[0] = waveData[0];

	for (int i = 1; i < sampleNum; i++) {
		waveDataAfter[i] = waveData[i] - waveData[i - 1] * factor;
	}

	return waveDataAfter;
}

/**
*  construct a hamming window
*
*  @param hamWin     hamming window function
*  @param hamWinSize hamming window size
*/
void hamming(double *hamWin, int hamWinSize)
{
	for (int i = 0; i < hamWinSize; i++)
	{
		hamWin[i] = (double)(0.54 - 0.46 * cos(2 * PI * (double)i / ((double)hamWinSize - 1)));
	}
}

/**
*  change the sample data into frame data
*
*  @param waveData          sample data
*  @param numPerframe       the sample point of each frame
*  @param actualNumPerFrame the actual point after zero-padding
*
*  @return  2-d frames, each line represents several points in the frame
*/
double** getFrame(double* waveData, int numPerframe, int actualNumPerFrame, int sampleNum)
{
	//    long sampleNum = sizeof(waveData);
	//    long frameNum;
	double *hamWin = new double[numPerframe];
	hamming(hamWin, numPerframe);

	int uniqueFrameNum = numPerframe / 2;

	if ((sampleNum - uniqueFrameNum) % uniqueFrameNum == 0) {
		frameNum = (sampleNum - uniqueFrameNum) / uniqueFrameNum;
	}
	else {
		frameNum = (sampleNum - uniqueFrameNum) / uniqueFrameNum;
	}
	double** frameData = new double*[frameNum];

	for (int i = 0; i < frameNum; i++) {
		frameData[i] = new double[actualNumPerFrame];
	}
	//    frameData[0][0] = 1;
	for (int i = 0; i < frameNum; i++) {
		for (int j = 0; j < actualNumPerFrame; j++) {
			if ((j >= numPerframe) || ((j + i * uniqueFrameNum) >= sampleNum)) {
				frameData[i][j] = 0;
			}
			else frameData[i][j] = waveData[j + i * uniqueFrameNum] * hamWin[j];
		}
	}
	return frameData;
}



/**
*  for each frame, do the fft and compute the energy for the first half
*
*  @param frameData         sample point in each frame
*  @param actualNumPerFrame the length of the frame after zero-padding
*
*  @return discrete frequency power spectrum
*/
double* getFftEnergy(double* frameData, int actualNumPerFrame) {

	double* frameEnergy = new double[actualNumPerFrame / 2 + 1];
	//    Complex *frameDataComplex = new Complex(actualNumPerFrame);
	Complex *frameDataComplex = new Complex[actualNumPerFrame];

	for (int i = 0; i < actualNumPerFrame; i++) {
		frameDataComplex[i] = frameData[i] * 1.0;
	}

	CArray data(frameDataComplex, actualNumPerFrame);

	fft(data);

	for (int i = 0; i < (actualNumPerFrame / 2 + 1); i++) {
		if (abs(data[i]) <= 1) {
			frameEnergy[i] = 0;
		}
		else {
			frameEnergy[i] = pow(abs(data[i]), 2);
		}
	}
	return frameEnergy;
}


/**
*  get the mel-fre power spectrum
*
*  @param fftSample discrete frequency power spectrum after fft(the first half)
*
*  @return mel-fre power spectrum
*/
double* getMelEnergy(double* fftSampleEnergy) {
	int filterNum = MEL_POINT;
	double* melEnergy = new double[filterNum];
	int frameSize = ACTUAL_SAMPLE_PER_FRAME / 2 + 1;
	int sampleRate = SAMPLE_RATE;
	double freMax = sampleRate / 2;
	double melFreMax = 1125 * log(1 + freMax / 700);

	double melGap = melFreMax / (filterNum + 1);   // mel-fre gap

	double* melEdge = new double[filterNum + 2];   // each mel-fre edge
	double* freEdge = new double[filterNum + 2];   // each actual-fre edge
	double* freEdgePoint = new double[filterNum + 2];    // each actual-fre edge point

	for (int i = 0; i < filterNum + 2; i++) {
		melEdge[i] = melGap * i;
		freEdge[i] = 700 * (exp(melEdge[i] / 1125) - 1);
		freEdgePoint[i] = (frameSize * freEdge[i] / freMax);
		//        cout << i <<" =" << freEdgePoint[i]<<endl;
	}

	delete[] melEdge;
	//    delete [] freEdge;

	for (int i = 0; i < filterNum; i++) {
		melEnergy[i] = 0;
	}

	for (int i = 1; i <= filterNum; i++) {
		double temp = 0;

		//        cout << "i-1= " << freEdgePoint[i-1] << endl;
		//        cout << "i = " <<  freEdgePoint[i]<< endl;

		for (int j = 0; j < frameSize; j++) {

			if (j < freEdgePoint[i - 1]) {
				temp = 0;
			}
			else if (j >= freEdgePoint[i - 1] && j <= freEdgePoint[i] && (freEdgePoint[i] != freEdgePoint[i - 1])) {
				temp = (j - freEdgePoint[i - 1]) / (freEdgePoint[i] - freEdgePoint[i - 1]) * 1.0;
			}
			else if (j >= freEdgePoint[i] && j <= freEdgePoint[i + 1] && (freEdgePoint[i + 1] != freEdgePoint[i])) {
				temp = (freEdgePoint[i + 1] - j) / (freEdgePoint[i + 1] - freEdgePoint[i]) * 1.0;
			}
			else if (j > freEdgePoint[i + 1]) {
				temp = 0;
			}
			melEnergy[i - 1] += fftSampleEnergy[j] * temp;
			//            if (temp != 0)
			//                cout << "temp = " << temp <<endl;
		}

	}

	return melEnergy;
}


/**
*  do the log for the mel power spectrum
*
*  @param melEnergy mel-fre power spectrum
*
*  @return log mel-fre power spectrum
*/
double* getMelLogEnergy(double* melEnergy) {
	int filterNum = MEL_POINT;
	double* melLogEnergy = new double[filterNum];
	for (int i = 0; i < filterNum; i++) {
		if (melEnergy[i] <= 1) {
			melLogEnergy[i] = 0;
		}
		else
			melLogEnergy[i] = log(melEnergy[i]);
	}

	return melLogEnergy;
}

/**
*  do the DCT for each frame.
*
*  @param melEnergy        the log mel-fre power spectrum of a frame
*
*  @return         the 13-D DCT result
*/
double* getDCT(double* melLogEnergy) {
	int filterNum = MEL_POINT;
	int dctDimension = DCT_DIMENSION;

	double* dctResult = new double[dctDimension];

	for (int i = 0; i < dctDimension; i++) {
		dctResult[i] = 0;
	}

	for (int i = 0; i < dctDimension; i++) {
		for (int j = 0; j < filterNum; j++) {
			dctResult[i] += melLogEnergy[j] * cos(PI * i / (2 * filterNum) *  (2 * j + 1));
		}
		//        dctResult[i] = dctResult[i] * sqrt(2.0 / filterNum);
	}
	return dctResult;
}

/**
*  do the normalized DCT
*
*  @param dctResult        the 13 * frameNum DCT result
*/
void getNormalizedDCT(double** dctResult) {
	double m[DCT_DIMENSION] = { 0 };
	double sum[DCT_DIMENSION] = { 0 };

	for (int i = 0; i < DCT_DIMENSION; i++) {

		for (int j = 0; j < frameNum; j++) {
			sum[i] += dctResult[j][i];
		}

		m[i] = sum[i] / frameNum;
	}

	for (int i = 0; i < DCT_DIMENSION; i++) {

		for (int j = 0; j < frameNum; j++) {
			dctResult[j][i] -= m[i];
		}
	}

}

void featureExtraction() {
	short* dataWave;    // store the original wave data
	double* waveDataAfter; // store the wave data after pre-emphasize
	double** frameData;   // store the wave data in each frame
	int numSample;
	int sampleRate;
	//    long frameNum;



	const char *wavFile = "record.wav";

	// read in the wave data
	dataWave = ReadWavFile(wavFile, &numSample, &sampleRate);
	int ost = pruneFrame(dataWave, numSample);
	dataWave += ost;
	//    cout << dataWave;

	// do the preemphasize
	waveDataAfter = preemphasized(dataWave, PREEMPHASIZED_FACTOR, numSample);
	// divide the wave data into frame (Here represent as 2-D)
	frameData = getFrame(waveDataAfter, SAMPLE_PER_FRAME, ACTUAL_SAMPLE_PER_FRAME, numSample);
	// get the frame number
	//    frameNum = sizeof(frameData);

	// do the fft and get each frame's power spectrum
	// get the mel-fre power spectrum, do the log and DCT
	double** frameEnergy = new double*[frameNum];
	double** melEnergy = new double*[frameNum];
	double** melLogEnergy = new double*[frameNum];
	double** frameDCT = new double*[frameNum];

	for (int i = 0; i < frameNum; i++) {

		frameEnergy[i] = getFftEnergy(frameData[i], ACTUAL_SAMPLE_PER_FRAME);
		melEnergy[i] = getMelEnergy(frameEnergy[i]);
		melLogEnergy[i] = getMelLogEnergy(melEnergy[i]);
		frameDCT[i] = getDCT(melLogEnergy[i]);
	}

	ofstream fileSample("sample.txt");
	ofstream filePreemphasized("sample_after_preemphasize.txt");
	ofstream fileFrame("frameData.txt");
	ofstream fileFrameEnergy("frameEnergy.txt");
	ofstream fileMelEnergy("melEnergy.txt");
	ofstream fileMelLogEnergy("melLogEnergy.txt");
	ofstream fileDCT("DCT.txt");
	ofstream fileNormDCT("NormDCT.txt");

	for (int i = 0; i < numSample; i++) {
		fileSample << dataWave[i];
		fileSample << " ";
		filePreemphasized << waveDataAfter[i];
		filePreemphasized << " ";
	}
	delete[] dataWave;
	delete[] waveDataAfter;

	for (int i = 0; i < frameNum; i++) {

		for (int j = 0; j < ACTUAL_SAMPLE_PER_FRAME; j++) {
			fileFrame << frameData[i][j];
			fileFrame << " ";
		}

		for (int k = 0; k < ACTUAL_SAMPLE_PER_FRAME / 2 + 1; k++) {
			fileFrameEnergy << frameEnergy[i][k];
			fileFrameEnergy << " ";
		}

		for (int l = 0; l < MEL_POINT; l++) {
			fileMelEnergy << melEnergy[i][l];
			fileMelEnergy << " ";
			fileMelLogEnergy << melLogEnergy[i][l];
			fileMelLogEnergy << " ";
		}

		for (int m = 0; m < DCT_DIMENSION; m++) {
			fileDCT << frameDCT[i][m];
			//            cout << frameDCT[i][m] << endl;
			fileDCT << " ";
		}

		fileFrame << endl;
		fileFrameEnergy << endl;
		fileMelEnergy << endl;
		fileMelLogEnergy << endl;
		fileDCT << endl;
	}

	getNormalizedDCT(frameDCT);

	for (int i = 0; i < frameNum; i++) {

		for (int j = 0; j < DCT_DIMENSION; j++) {
			fileNormDCT << frameDCT[i][j] << " ";
		}
		fileNormDCT << endl;
	}
	cout << "frameNum =  " << frameNum;
}

